/* system.h
*
* This include file contains information that is included in every
* function in the test set.
*
* COPYRIGHT (c) 1989-2009.
* On-Line Applications Research Corporation (OAR).
*
* The license and distribution terms for this file may be
* found in the file LICENSE in this distribution or at
* http://www.rtems.com/license/LICENSE.
*
* $Id$
*/

#ifndef _SYS_H
#define _SYS_H

#include "mv_types.h"
#include <rtems.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SPI_BUS_NAME   "/dev/spi"
#define SPI_FLASH_NAME "flash"

// Flash chip specific defines
#define FLASH_CHIP_SIZE     (8 * 1024 * 1024)
#define SUBSECTOR_SIZE          (4096)

// Macro to align sizes and addresses to the flash subsector size
#define ALIGN_TO_SUBSECTOR(S)   ((((S) + (SUBSECTOR_SIZE) - 1)) & ~((SUBSECTOR_SIZE) - 1))

// Bootloader application defines
// Magic numbers to identify if an image is present
#define VALID_HEADER_MAGIC   (0xABCD1234)
#define INVALID_HEADER_MAGIC (0x00000000)
// Global Flash flag values
#define FLAG_APP_EXECUTE    (1)
#define FLAG_APP_UPDATE     (0)
// Maximum number of applications that can be stored
#define MAX_APPLIACIONS_IN_FLASH  (1)
// This is a reserved part for the booloader
#define LAUNCHER_RESERVED_SIZE  (512 * 1024)


// Information about each of the application images
typedef struct {
    u32 header_magic;
    u32 app_crc;
    u32 bootImg_offset;
    u32 bootImg_size;
} tAppInfo;

typedef struct {
    u32         execute_app_flag;   // Set to FLAG_APP_EXECUTE to execute from flash, set to FLAG_APP_UPDATE to update
    u32         loader_entry;       // entry point of the bootloader
    tAppInfo    app_info[MAX_APPLIACIONS_IN_FLASH];
} tFlashHeader;

struct tFlashParams
{
    const char * devName;
    void *inBuff;   // this buffer is saved to flash
    void *outBuff;  // data from flash
    u32 offset;     // flash start address for read/write operations
    u32 size;       // size for read/write operations
    u32 imgId;      // active image - 0/1
    u32 writeImg;   // should the flash be written
};

// booloader errors
typedef enum {
    ERR_NONE = 0,
    ERR_IO_DRV,
    ERR_RW,
    ERR_INV_APP_ID,
    ERR_NO_SPACE,
    ERR_ELF_ENTRY,
    ERR_INV_HEADER,
    ERR_INV_FLAG,
    ERR_MEM,
    ERR_INV_CRC
} bl_errors_t;

// Maximum size of the applications
#define APP_MAX_FILE_SIZE   ((FLASH_CHIP_SIZE) - (LAUNCHER_RESERVED_SIZE))
// Start of the header section in flash
#define FLAGS_SECTION_OFFSET ((LAUNCHER_RESERVED_SIZE) - ALIGN_TO_SUBSECTOR(sizeof(tFlashHeader)))

/* functions */
rtems_task flash_app_task(rtems_task_argument unused);
int32_t flash_execute(struct tFlashParams *p);
/* global variables */

/*
* Keep the names and IDs in global variables so another task can use them.
*/
extern struct tFlashParams flash_params;

#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
