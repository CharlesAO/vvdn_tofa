///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Task to be run on FPGA
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "mv_types.h"
#include "system.h"
#include <bsp.h>
#include <icb_defines.h>
#include <fcntl.h>

#include "OsDrvSpiBus.h"
#include "DrvTimer.h"
#include "DrvGpio.h"

#include <bsp/irq.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include "OsDrvCpr.h"

#include <SpiFlashN25QDevice.h>

#include "swcShaveLoaderPrivate.h"
#include "swcMemoryTransfer.h"
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>
#include <DrvLeonL2CDefines.h>
#include <swcCrc.h>


// Retry if the flash read fails
#define RETRY_COUNT_FLASH_READ  (3)

//External function executed before the main function is called byt Common/drivers/myriad2/system/leon/bm/asm/Crt0.S
extern void start(void);

// declare external function written in ASM
extern void __boot_execute(void(*entry)(void)) __attribute__((noreturn));

// ELF file header
static const u8 elfmagic[] = {0x7F, 'E', 'L', 'F'};

// Temporary storage for one flash sub-sector
static u8 flash_buff[SUBSECTOR_SIZE] = {0};

#ifdef USBFLASH_APP_DEBUG
// Fill test buffers
static void fill_tx_buffers(u32 * buff, u32 byteSize)
{
    uint32_t i;
    for (i = 0; i < (uint32_t)(byteSize/sizeof(*buff)); i++)
    {
        buff[i] = i;
    }
}
#endif

/// Check if a buffer is a valid ELF
///@param sAddr starting address from where to load the app image
///@return 0 if the buffer is a valid ELF
static u32 isInValidElf(u8 *sAddr)
{
    Elf32Ehdr ElfHeader;

    //get the section header
    memcpy((u32*)&ElfHeader, (u32*)sAddr, sizeof(Elf32Ehdr));

    // check if the buffer is a valid elf
    return !!memcmp(ElfHeader.eIdent, elfmagic, sizeof(elfmagic));
}

static inline uint32_t getUnsigned(const u8 * src)
{
    uint32_t val=0;
    for(uint32_t i=0;i<4;i++){
        val=val*10+src[i]-'0';
        //TODO: if(src[i]<'0' || src[i]>'9') treat this also
    }
    return val;
}

/// Load an elf file - These are elf object files stripped of any symbols
///@param sAddr starting address from where to load the app image
///@return the application entry point
static u32 loadAppImg(u8 *sAddr)
{
    Elf32Ehdr ElfHeader;
    u32 SecHeaders, i;
    u32 phAddr;
    u32 *srcAddr;
    u32 *dstAddr;
    u32 SecSize;

    // check if the loaded context has debug information at the beginning of the ELF
    // the first byte from the magic number of the ELF header
    if(*(u32*)sAddr != 0x464c457f){
        unsigned int stringLength;

        // get the length of the name from the loaded library
        stringLength = getUnsigned(sAddr);
        // set the address to the beginning of the elf section header
        sAddr += 4 + ((stringLength + 3) & 0xFFFFFFFC);
    }

    //get the section header
    memcpy((u32*)&ElfHeader, (u32*)sAddr, sizeof(Elf32Ehdr));

    //Reading section headers table offset
    phAddr = (u32)sAddr + ElfHeader.eShoff;

    // Print the ELF Header
    printf("\nELF Info\n");
    printf("Image at 0x%08X\n", (unsigned int)sAddr);
    printf("Header:\n");
    printf("eIdent[]   ");
    for (i = 0; i < EI_NIDENT; i++) {
        if ((ElfHeader.eIdent[i] >= 0x20) && (ElfHeader.eIdent[i] <= 0x7E)) {   // Only printable
            printf("%c ",ElfHeader.eIdent[i]);
        } else {
            printf("0x%02X ",ElfHeader.eIdent[i]);
        }
    }
    printf("\n");
    printf("eType      %d\n",ElfHeader.eType);
    printf("eMachine   %d\n",ElfHeader.eMachine);
    printf("eVersion   %u\n",(unsigned int)ElfHeader.eVersion);
    printf("eEntry     0x%08X\n",(unsigned int)ElfHeader.eEntry);
    printf("ePhoff     %u\n",(unsigned int)ElfHeader.ePhoff);
    printf("eShoff     %u\n",(unsigned int)ElfHeader.eShoff);
    printf("eFlags     0x%08X\n",(unsigned int)ElfHeader.eFlags);
    printf("eEhsize    %d\n",ElfHeader.eEhsize);
    printf("ePhentsize %d\n",ElfHeader.ePhentsize);
    printf("ePhnum     %d\n",ElfHeader.ePhnum);
    printf("eShentsize %d\n",ElfHeader.eShentsize);
    printf("eShnum     %d\n",ElfHeader.eShnum);
    printf("eShstrndx  %d\n",ElfHeader.eShstrndx);

    // check if the buffer is a valid elf
    if (isInValidElf(sAddr)) {
        printf("\nNot a valid ELF!\n");
        return (u32)NULL;
    }

    //parse section headers:
    for (SecHeaders = 0; SecHeaders < ElfHeader.eShnum; SecHeaders++) {
        Elf32Section ElfSecHeader;
        u32 SecOffset;
        u32 SecDataOffset;

        SecOffset = phAddr + sizeof(Elf32Section) * SecHeaders;
        memcpy((u32*)&ElfSecHeader, (u32*)SecOffset, sizeof(Elf32Section));
        SecDataOffset = (u32)sAddr + ElfSecHeader.shOffset;

        srcAddr = (u32*)SecDataOffset;
//        dstAddr = (u32*)swcSolveShaveRelAddr(ElfSecHeader.shAddr, targetS);
        dstAddr = (u32 *)ElfSecHeader.shAddr;
        SecSize = ElfSecHeader.shSize;
        //Only load PROGBITS sections
        if (ElfSecHeader.shType == SHT_PROGBITS) {
            memcpy((u32*)dstAddr, (u32*)srcAddr, SecSize);
        }
    }

    return ElfHeader.eEntry;
}

static int erase_spi_flash(int fd, struct tFlashParams *p)
{
    int status;

    struct spiFlashN25QEraseArgs_t eraseArgs =
            {
                .offset = p->offset,
                .size   = p->size,
            };

    status = ioctl(fd, FLASH_CMD_ERASE, &eraseArgs);

    if (status) {
        printf("Unable to erase device, err = %d\n", status);
    }

    return status;
}

static int write_spi_flash(int fd, struct tFlashParams *p)
{
    int status;

    status =  lseek(fd, p->offset, SEEK_SET);
    if (status != (int)p->offset) {
        perror("seek error: ");
        return -1;
    }

    status = write(fd, p->inBuff, p->size);
    if (status != (int)p->size) {
        printf("Unable to write all %u bytes to device. %d sent \n", (unsigned int)p->size, status);
        perror("");
        return -1;
    }

    return 0;
}

static int read_spi_flash(int fd, struct tFlashParams *p)
{
    int status = 0;

    status =  lseek(fd, p->offset, SEEK_SET);
    if (status != (int)p->offset) {
        perror("seek error: ");
        return -1;
    }

    status = read(fd, p->outBuff, p->size);
    if (status != (int)p->size) {
        printf("Unable to read all %u bytes from device. %d sent \n", (unsigned int)p->size, status);
        perror("");
        return -1;
    }

    return 0;
}

rtems_task flash_app_task(rtems_task_argument unused)
{
    struct tFlashParams *p = (struct tFlashParams *)unused;

	if (p) {
        rtems_task_wake_after(50);
		flash_execute(p);
	}

    rtems_task_delete(RTEMS_SELF);
}

int32_t flash_execute(struct tFlashParams *p)
{
    rtems_status_code status;
    void (*app_entry)(void) = NULL;
    tFlashHeader *flash_header;
    uint32_t app_idx;
    uint32_t app_size;
    uint32_t app_offset;
    void *   app_in_buff;
    void *   app_out_buff;
    uint32_t apps_total_space;
    int32_t ret = ERR_NONE;
    uint32_t calculated_crc;
    uint32_t retries = 0;
    int fd;

    // Check if function was called with valid parameter's
    if (NULL == p)
    {
        printf("\n Flash_execute started whit NULL pointer as a parameter !!! FATAL ERROR !!! \n");
        ret = ERR_IO_DRV;
        goto exit_func;
    }

    // Creating and configuring spi handle
    printf("Opening %s ...\n", p->devName);
    fd = open(p->devName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        printf("spi open failed\n");
        ret = ERR_IO_DRV;
        goto exit_func;
    }

    printf("\nFlash App...\n\n");

    // ID represents a counter of applications loaded in RAM. Check if there is still space available
    if (p->imgId >= MAX_APPLIACIONS_IN_FLASH)
    {
        printf("Invalid App ID: %lu\n", p->imgId);
        ret = ERR_INV_APP_ID;
        goto exit_deinit;
    }

    printf("App ID: %lu\n", p->imgId);

    // clear any data from the previous run
    memset(flash_buff, 0x00, sizeof(flash_buff));
    flash_header = (tFlashHeader *)&flash_buff;

    // save
    app_size = p->size;
    app_in_buff = p->inBuff;
    app_out_buff = p->outBuff;

    // get the flash header
    p->offset = ALIGN_TO_SUBSECTOR(FLAGS_SECTION_OFFSET);
    p->size = sizeof(tFlashHeader);
    p->outBuff = flash_header;

    printf("Reading Header: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);

    if (RTEMS_SUCCESSFUL != read_spi_flash(fd, p))
    {
        printf("Error reading flash !!!\n");
        ret = ERR_RW;
        goto exit_deinit;
    }

    // Check and initialize the header data
    for (app_idx = 0; app_idx < MAX_APPLIACIONS_IN_FLASH; app_idx++)
    {
        if (VALID_HEADER_MAGIC != flash_header->app_info[app_idx].header_magic)
        {
            flash_header->app_info[app_idx].header_magic = 0;
            flash_header->app_info[app_idx].app_crc = 0;
            flash_header->app_info[app_idx].bootImg_offset = 0;
            flash_header->app_info[app_idx].bootImg_size = 0;
        }
    }

    // Write the application image to the flash
    if (p->writeImg != FALSE)
    {
        // Check if the image contains a valid ELF
        if (isInValidElf(app_in_buff))
        {
            printf("Exiting. Not a valid ELF!\n");
            ret = ERR_ELF_ENTRY;
            goto exit_deinit;
        }

        // Calculate the total space needed for the applications
        apps_total_space = 0;
        for (app_idx = 0; app_idx < MAX_APPLIACIONS_IN_FLASH; app_idx++)
        {
            if (app_idx != p->imgId)
            {
                apps_total_space += flash_header->app_info[app_idx].bootImg_size;
            }
        }
        apps_total_space += app_size;

        printf("Total space needed %lu, available %d\n", ALIGN_TO_SUBSECTOR(apps_total_space), (APP_MAX_FILE_SIZE));

        // Check for overlapping images
        if (ALIGN_TO_SUBSECTOR(apps_total_space) > (APP_MAX_FILE_SIZE))
        {
            printf("Not enough space!\n");
            ret = ERR_NO_SPACE;
            goto exit_deinit;
        }

        // Erase the header
        p->offset = ALIGN_TO_SUBSECTOR(FLAGS_SECTION_OFFSET);
        p->size = ALIGN_TO_SUBSECTOR(sizeof(tFlashHeader));

        printf("Erasing Header: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
        if (RTEMS_SUCCESSFUL != erase_spi_flash(fd, p))
        {
            ret = ERR_RW;
            goto exit_deinit;
        }

        // calculate the CRC of the input application buffer
        calculated_crc = swcCalcCrc32(app_in_buff, app_size, NATIVE_POINTER_TYPE);
        printf("Calculated CRC from App: %08X\n", (unsigned int)calculated_crc);

        // update the global flag
        flash_header->execute_app_flag = FLAG_APP_EXECUTE;
        // save the boot loader entry point
        flash_header->loader_entry = (u32)start;
        // Fill the header information
        flash_header->app_info[p->imgId].header_magic = VALID_HEADER_MAGIC;
        flash_header->app_info[p->imgId].app_crc = calculated_crc;
        flash_header->app_info[p->imgId].bootImg_size = app_size;

        // the second image is at the end of the flash
        switch (p->imgId)
        {
            default:
            case 0:
                app_offset = ALIGN_TO_SUBSECTOR(LAUNCHER_RESERVED_SIZE);
                break;
            case 1:
                app_offset = FLASH_CHIP_SIZE - ALIGN_TO_SUBSECTOR(app_size);
                break;
        }

        flash_header->app_info[p->imgId].bootImg_offset = app_offset;

        // restore
        p->inBuff = app_in_buff;
        p->outBuff = app_out_buff;

        p->size = ALIGN_TO_SUBSECTOR(app_size);
        p->offset = ALIGN_TO_SUBSECTOR(app_offset);

        // Erase before writing
        printf("Erasing %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
        if (RTEMS_SUCCESSFUL != erase_spi_flash(fd, p))
        {
            ret = ERR_RW;
            goto exit_deinit;
        }

        // Save the Application image to flash
        printf("Writing Application: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
        if (RTEMS_SUCCESSFUL != write_spi_flash(fd, p))
        {
            ret = ERR_RW;
            goto exit_deinit;
        }
    }  // end of if (p->writeImg)

    // check the global flash flag
    if (FLAG_APP_EXECUTE == flash_header->execute_app_flag) {

        if (VALID_HEADER_MAGIC == flash_header->app_info[p->imgId].header_magic) {
            // Read the Application image
            p->inBuff = app_in_buff;
            p->outBuff = app_out_buff;
            p->offset = flash_header->app_info[p->imgId].bootImg_offset;
            p->size = flash_header->app_info[p->imgId].bootImg_size;

#ifdef USBFLASH_APP_DEBUG
            printf("\n");
            printf("global app flag  %d\n", flash_header->execute_app_flag);
            printf("bootloader entry 0x%08X\n", flash_header->loader_entry);
            for (app_idx = 0; app_idx < MAX_APPLIACIONS_IN_FLASH; app_idx++) {
                printf("header%d_magic   %08X\n", app_idx, flash_header->app_info[app_idx].header_magic);
                printf("header%d_crc     %08X\n", app_idx, flash_header->app_info[app_idx].app_crc);
                printf("bootImg%d_offset %08X\n", app_idx, flash_header->app_info[app_idx].bootImg_offset);
                printf("bootImg%d_size   %d\n",   app_idx, flash_header->app_info[app_idx].bootImg_size);
            }
            printf("\n");
#endif

            // Try to read the application
            do {

                printf("Reading Application: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
                if (RTEMS_SUCCESSFUL != read_spi_flash(fd, p)) {
                    ret = ERR_RW;
                    goto exit_deinit;
                }

                // calculate the CRC of the input application buffer
                calculated_crc = swcCalcCrc32(app_out_buff, flash_header->app_info[p->imgId].bootImg_size, NATIVE_POINTER_TYPE);
                printf("Calculated CRC from Flash: %08X\n", (unsigned int)calculated_crc);

                if (calculated_crc != flash_header->app_info[p->imgId].app_crc) {
                    printf("CRC mismatch! (Expected %08X, calculated %08X)\n", (unsigned int)flash_header->app_info[p->imgId].app_crc, (unsigned int)calculated_crc);
                    ret = ERR_INV_CRC;
                    retries++;
                } else {
                    printf("CRC OK!\n");
                    // If the CRC is good, but the image does not contain a valid ELF, exit
                    if (isInValidElf(p->outBuff)) {
                        printf("App Corrupt. Not a valid ELF!\n");
                        ret = ERR_ELF_ENTRY;
                    } else {
                        ret = ERR_NONE;
                    }

                    break;
                }

            } while(RETRY_COUNT_FLASH_READ > retries);

            // if the flash is corrupt, invalidate the header
            if (ERR_NONE != ret) {
                printf("Invalidating the header.\n");
                flash_header->execute_app_flag = FLAG_APP_UPDATE;
                // invalidate the corrupt application
                flash_header->app_info[p->imgId].header_magic = INVALID_HEADER_MAGIC;
            }

            // update the header if the image is newly written or there was an error reading it
            if ((p->writeImg) || (ERR_NONE != ret)) {
                // Save the the header
                p->inBuff = flash_header;
                p->size = ALIGN_TO_SUBSECTOR(sizeof(tFlashHeader));
                p->offset = ALIGN_TO_SUBSECTOR(FLAGS_SECTION_OFFSET);

                printf("Writing Header: %u Bytes from %08X\n", (unsigned int)p->size, (unsigned int)p->offset);
                if (RTEMS_SUCCESSFUL != write_spi_flash(fd, p)) {
                    ret = ERR_RW;
                    goto exit_deinit;
                }
            }

            // if the flash is corrupt, exit
            if (ERR_NONE != ret) {
                printf("Flash is corrupt! Exiting after %u attempts to read it!\n", (unsigned int)retries);
                goto exit_deinit;
            }

        } else {
            printf("Invalid image header magic number (%08X).\n", (unsigned int)flash_header->app_info[p->imgId].header_magic);
            ret = ERR_INV_HEADER;
            goto exit_deinit;
        }

    } else {
        printf("No valid application in memory !!!\n");
        ret = ERR_INV_FLAG;
        goto exit_deinit;
    }

    printf("Flash OK\n");

    // Handle the data as an elf and execute it
    app_entry = (void (*)(void))loadAppImg(p->outBuff);

    if (app_entry)
    {
        printf("----------------------------now jump to app 0x%x\n", (unsigned int)(app_entry));
        swcLeonFlushDcache();
        swcLeonFlushIcache();
        swcLeonDisableCaches();
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 0);
        // reset peripherals
        DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, -1);
        __boot_execute(app_entry);
    } else {
        // Invalid Entry point
        printf("Invalid Entry point!\n");
        ret = ERR_ELF_ENTRY;
        goto exit_deinit;
    }

exit_deinit:
    // Jump here to close the spi driver before exiting
    printf("Close and exit!\n");
    if ((status = close(fd)) != 0)
    {
        printf("spi close failed with sc %u\n", (unsigned int)status);
        ret = ERR_IO_DRV;
    }

exit_func:
    // Just exit
    return ret;
}
