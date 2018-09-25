#include <assert.h>
#include <rtems.h>
#include <rtems/libi2c.h>

#include <stdio.h>

#include <SpiFlashN25QDevice.h>
#include <rtems/libio.h>

#define SPI_MEM_CMD_WREN  0x06
#define SPI_MEM_CMD_WRDIS 0x04
#define SPI_MEM_CMD_RDID  0x9F
#define SPI_MEM_CMD_RDSR  0x05
#define SPI_MEM_CMD_WRSR  0x01
#define SPI_MEM_CMD_READ  0x03
#define SPI_MEM_CMD_FREAD 0x0B
#define SPI_MEM_CMD_PP    0x02  /* page program                       */
#define SPI_MEM_CMD_SSE   0x20  /* subsector erase                       */
#define SPI_MEM_CMD_SE    0xD8  /* sector erase                       */
#define SPI_MEM_CMD_BE    0xC7  /* bulk erase                         */
#define SPI_MEM_CMD_DP    0xB9  /* deep power down                    */
#define SPI_MEM_CMD_RES   0xAB  /* release from deep power down       */

#define FLASH_CMD_SIZE 1
#define FLASH_SR_SIZE  1
#define FLASH_ID_SIZE 20

#define MAX_N25Q_BAUDRATE (54 * 1000 * 1000)

static rtems_status_code spi_n25q_lock(rtems_id semId)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    // TODO: Timeout
    assert(rtems_semaphore_obtain(semId, RTEMS_WAIT, RTEMS_NO_TIMEOUT) == RTEMS_SUCCESSFUL);

    return sc;
}

static rtems_status_code spi_n25q_unlock(rtems_id semId)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = rtems_semaphore_release(semId);

    return sc;
}

static rtems_status_code spi_N25Q_minor2param_ptr(rtems_device_minor_number minor,
                                                  spi_n25q_param_t **param_ptr,
                                                  spi_n25q_local_t **local_ptr)
{
    rtems_status_code   rc = RTEMS_SUCCESSFUL;
    spi_n25q_t *drv_ptr;

    if (rc == RTEMS_SUCCESSFUL) {
        rc = -rtems_libi2c_ioctl(minor,
                                 RTEMS_LIBI2C_IOCTL_GET_DRV_T,
                                 &drv_ptr);
    }

    if ((rc == RTEMS_SUCCESSFUL) &&
        (drv_ptr->libi2c_drv_entry.size != sizeof(spi_n25q_t))) {
        rc = RTEMS_INVALID_SIZE;
    }
    if (rc == RTEMS_SUCCESSFUL) {
        if(param_ptr)
            *param_ptr = &(drv_ptr->spi_memdrv_param);
        if(local_ptr)
            *local_ptr = &(drv_ptr->localParams);
    }
    return rc;
}

static rtems_status_code spi_N25Q_read_sr( rtems_device_major_number major,
                                           rtems_device_minor_number minor,
                                           uint8_t *sr_val)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code rc = RTEMS_SUCCESSFUL;
    rtems_libi2c_read_write_t rwop;
    int ret_cnt;
    uint8_t rxbuf[FLASH_CMD_SIZE + FLASH_SR_SIZE];
    uint8_t txbuf[FLASH_CMD_SIZE + FLASH_SR_SIZE];


    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_start(minor);
    }

    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_addr(minor,TRUE);
    }

    txbuf[0] = SPI_MEM_CMD_RDSR;
    txbuf[1] = 0;

    rwop.byte_cnt = FLASH_CMD_SIZE + FLASH_SR_SIZE;
    rwop.rd_buf = &rxbuf[0];
    rwop.wr_buf = &txbuf[0];

    if (rc == RTEMS_SUCCESSFUL) {
        ret_cnt = rtems_libi2c_ioctl (minor,
                                      RTEMS_LIBI2C_IOCTL_READ_WRITE,
                                      &rwop);
        if (ret_cnt < 0) {
            rc = -ret_cnt;
        }
        else
        {
            *sr_val = rxbuf[1];
        }
    }

    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_stop(minor);
    }

    return rc;
}

rtems_status_code spi_N25Q_write_sr( rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     uint8_t *sr_val)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code rc = RTEMS_SUCCESSFUL;
    rtems_libi2c_read_write_t rwop;
    int ret_cnt;
    uint8_t rxbuf[FLASH_CMD_SIZE + FLASH_SR_SIZE];
    uint8_t txbuf[FLASH_CMD_SIZE + FLASH_SR_SIZE];


    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_start(minor);
    }

    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_addr(minor,TRUE);
    }

    txbuf[0] = SPI_MEM_CMD_WRSR;
    txbuf[1] = *sr_val;

    rwop.byte_cnt = FLASH_CMD_SIZE + FLASH_SR_SIZE;
    rwop.rd_buf = &rxbuf[0];
    rwop.wr_buf = &txbuf[0];
    if (rc == RTEMS_SUCCESSFUL) {
        ret_cnt = rtems_libi2c_ioctl (minor,
                                      RTEMS_LIBI2C_IOCTL_READ_WRITE,
                                      &rwop);
        if (ret_cnt < 0) {
            rc = -ret_cnt;
        }
    }

    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_stop(minor);
    }

    return rc;
}

rtems_status_code spi_N25Q_poll_write_done(rtems_device_major_number major,
                                           rtems_device_minor_number minor,
                                           u32 sleepTicks)
{
    rtems_status_code rc = RTEMS_SUCCESSFUL;
    uint8_t sr = 0;

    // TODO: timeout me please
    for(;;)
    {
        rc = spi_N25Q_read_sr(major, minor, &sr);
        if (rc == RTEMS_SUCCESSFUL)
        {
            if((sr & 0x3) == 0) // write done
                break;
            else
                rtems_task_wake_after(sleepTicks);
        }
        else
        {
            break;
        }
    }

    return rc;
}

rtems_status_code spi_N25Q_write(rtems_device_major_number major,
                                 rtems_device_minor_number minor,
                                 void                      *arg)
{
    rtems_status_code          rc = RTEMS_SUCCESSFUL;
    rtems_libio_rw_args_t *rwargs = arg;
    off_t                     off = rwargs->offset;
    int                       cnt = rwargs->count;
    unsigned char            *buf = (unsigned char *)rwargs->buffer;
    int                bytes_sent = 0;
    int                  curr_cnt;
    unsigned char       cmdbuf[4];
    int                   ret_cnt = 0;
    int                  cmd_size;
    spi_n25q_param_t  *mem_param_ptr;
    spi_n25q_local_t  *mem_local_ptr;
    rtems_libi2c_tfr_mode_t tfr_mode = {
        .baudrate =      20000000, /* maximum bits per second                   */
        .bits_per_char = 8,        /* how many bits per byte/word/longword?     */
        .lsb_first =     FALSE,    /* FALSE: send MSB first                     */
        .clock_inv =     FALSE,    /* FALSE: non-inverted clock (high active)   */
        .clock_phs =     FALSE     /* FALSE: clock starts in middle of data tfr */
    } ;

    /*
     * get mem parameters
     */
    if (rc == RTEMS_SUCCESSFUL) {
        rc = spi_N25Q_minor2param_ptr(minor,&mem_param_ptr, &mem_local_ptr);
    }
    /*
     * check arguments
     */
    if (rc == RTEMS_SUCCESSFUL) {
        if ((cnt <= 0)                      ||
            (cnt > mem_param_ptr->mem_size) ||
            (off > (mem_param_ptr->mem_size-cnt))) {
            rc = RTEMS_INVALID_SIZE;
        }
        else if (buf == NULL) {
            rc = RTEMS_INVALID_ADDRESS;
        }
    }

    spi_n25q_lock(mem_local_ptr->operationMutex);

    while ((rc == RTEMS_SUCCESSFUL) &&
           (cnt > bytes_sent)) {
        curr_cnt = cnt - bytes_sent;
        if ((mem_param_ptr->page_size > 0) &&
            (off              / mem_param_ptr->page_size) !=
            ((off+curr_cnt+1) / mem_param_ptr->page_size)) {
            curr_cnt = mem_param_ptr->page_size - (off % mem_param_ptr->page_size);
        }
        /*
         * select device, set transfer mode, address device
         */
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_start(minor);
        }
        /*
         * set transfer mode
         */
        if (rc == RTEMS_SUCCESSFUL) {
            tfr_mode.baudrate = mem_param_ptr->baudrate;
            rc = -rtems_libi2c_ioctl(minor,
                                     RTEMS_LIBI2C_IOCTL_SET_TFRMODE,
                                     &tfr_mode);
        }

        /*
         * address device
         */
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_addr(minor,TRUE);
        }

        /*
         * send write_enable command
         */
        if (rc == RTEMS_SUCCESSFUL) {
            cmdbuf[0] = SPI_MEM_CMD_WREN;
            ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,1);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }
        /*
         * terminate transfer
         */
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_stop(minor);
        }

        /*
         * select device, set transfer mode
         */
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_start(minor);
        }

        /*
         * address device
         */
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_addr(minor,TRUE);
        }

        /*
         * send "page program" command and address
         */
        if (rc == RTEMS_SUCCESSFUL) {
            cmdbuf[0] = SPI_MEM_CMD_PP;
            if (mem_param_ptr->mem_size > 0x10000 /* 256*256 */) {
                cmdbuf[1] = (off >> 16) & 0xff;
                cmdbuf[2] = (off >>  8) & 0xff;
                cmdbuf[3] = (off >>  0) & 0xff;
                cmd_size  = 4;
            }
            else if (mem_param_ptr->mem_size > 256) {
                cmdbuf[1] = (off >>  8) & 0xff;
                cmdbuf[2] = (off >>  0) & 0xff;
                cmd_size  = 3;
            }
            else {
                cmdbuf[1] = (off >>  0) & 0xff;
                cmd_size  = 1;
            }

            ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,cmd_size);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }
        /*
         * send write data
         */
        if (rc == RTEMS_SUCCESSFUL) {
            ret_cnt = rtems_libi2c_write_bytes(minor,buf,curr_cnt);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }
        /*
         * terminate transfer
         */
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_stop(minor);
        }
        /*
         * wait proper time for data to store, polling status register
         */
        spi_N25Q_poll_write_done(major, minor, 1);

        /*
         * adjust bytecount to be sent and pointers
         */
        bytes_sent += curr_cnt;
        off        += curr_cnt;
        buf        += curr_cnt;
    }
    rwargs->bytes_moved = bytes_sent;

    spi_n25q_unlock(mem_local_ptr->operationMutex);

    return rc;
}


rtems_status_code spi_N25Q_open( rtems_device_major_number major,
                                 rtems_device_minor_number minor,
                                 void                      *arg )
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code rc = RTEMS_SUCCESSFUL;
    spi_n25q_local_t  *mem_local_ptr;
    int ret_cnt;
    uint8_t txbuf[FLASH_CMD_SIZE];

    if (rc == RTEMS_SUCCESSFUL) {
        rc = spi_N25Q_minor2param_ptr(minor, NULL, &mem_local_ptr);
    }

    // device is sensitive to command sequences, so starting a semaphore
    if (rc == RTEMS_SUCCESSFUL && mem_local_ptr->operationMutex == (rtems_id)0) {
        rc = rtems_semaphore_create(rtems_build_name('N', '2', '5', 'Q'),
                                    1,
                                    RTEMS_PRIORITY |
                                    RTEMS_BINARY_SEMAPHORE |
                                    RTEMS_INHERIT_PRIORITY |
                                    RTEMS_NO_PRIORITY_CEILING |
                                    RTEMS_LOCAL,
                                    0,
                                    &mem_local_ptr->operationMutex);
    }


    spi_n25q_lock(mem_local_ptr->operationMutex);

    // if first opening, release from power down
    if(mem_local_ptr->refCnt == 0)
    {
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_start(minor);
        }

        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_addr(minor,TRUE);
        }

        txbuf[0] = SPI_MEM_CMD_RES;

        if (rc == RTEMS_SUCCESSFUL) {
            ret_cnt = rtems_libi2c_write_bytes (minor, txbuf, FLASH_CMD_SIZE);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }

        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_stop(minor);
        }
    }

    mem_local_ptr->refCnt++;

    spi_n25q_unlock(mem_local_ptr->operationMutex);
    return rc;
}


rtems_status_code spi_N25Q_close( rtems_device_major_number major,
                                  rtems_device_minor_number minor,
                                  void                      *arg )
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code rc = RTEMS_SUCCESSFUL;
    spi_n25q_local_t  *mem_local_ptr;
    int ret_cnt;
    uint8_t txbuf[FLASH_CMD_SIZE];

    if (rc == RTEMS_SUCCESSFUL) {
        rc = spi_N25Q_minor2param_ptr(minor, NULL, &mem_local_ptr);
    }

    spi_n25q_lock(mem_local_ptr->operationMutex);

    mem_local_ptr->refCnt--;

    // if last closing, enter power down
    if(mem_local_ptr->refCnt == 0)
    {
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_start(minor);
        }

        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_addr(minor,TRUE);
        }

        txbuf[0] = SPI_MEM_CMD_DP;

        if (rc == RTEMS_SUCCESSFUL) {
            ret_cnt = rtems_libi2c_write_bytes (minor, txbuf, 1);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }

        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_stop(minor);
        }
    }

    spi_n25q_unlock(mem_local_ptr->operationMutex);
    return rc;
}

rtems_status_code spi_N25Q_read(rtems_device_major_number major,
                                rtems_device_minor_number minor,
                                void                      *arg)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code rc = RTEMS_SUCCESSFUL;
    rtems_libio_rw_args_t *rwargs = arg;
    off_t                     off = rwargs->offset;
    int                       cnt = rwargs->count;
    unsigned char            *buf = (unsigned char *)rwargs->buffer;
    unsigned char         cmdbuf[4];
    int                   ret_cnt = 0;
    int                  cmd_size;
    spi_n25q_param_t  *mem_param_ptr;
    spi_n25q_local_t  *mem_local_ptr;
    rtems_libi2c_tfr_mode_t tfr_mode = {
        .baudrate =      20000000, /* maximum bits per second                   */
        .bits_per_char = 8,        /* how many bits per byte/word/longword?     */
        .lsb_first =     FALSE,    /* FALSE: send MSB first                     */
        .clock_inv =     FALSE,    /* FALSE: non-inverted clock (high active)   */
        .clock_phs =     FALSE     /* FALSE: clock starts in middle of data tfr */
    };

    /*
     * get mem parameters
     */
    if (rc == RTEMS_SUCCESSFUL) {
        rc = spi_N25Q_minor2param_ptr(minor,&mem_param_ptr, &mem_local_ptr);
    }
    /*
     * check arguments
     */
    if (rc == RTEMS_SUCCESSFUL) {
        if ((cnt <= 0)                      ||
            (cnt > mem_param_ptr->mem_size) ||
            (off > (mem_param_ptr->mem_size-cnt))) {
            rc = RTEMS_INVALID_SIZE;
        }
        else if (buf == NULL) {
            rc = RTEMS_INVALID_ADDRESS;
        }
    }

    spi_n25q_lock(mem_local_ptr->operationMutex);

    /*
     * select device, set transfer mode, address device
     */
    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_start(minor);
    }
    /*
     * set transfer mode
     */
    if (rc == RTEMS_SUCCESSFUL) {
        tfr_mode.baudrate = mem_param_ptr->baudrate;
        rc = -rtems_libi2c_ioctl(minor,
                                 RTEMS_LIBI2C_IOCTL_SET_TFRMODE,
                                 &tfr_mode);
    }
    /*
     * address device
     */
    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_addr(minor,TRUE);
    }

    if (off >= mem_param_ptr->mem_size) {
        /*
         * HACK: beyond size of memory array? then read status register instead
         */
        /*
         * send read status register command
         */
        if (rc == RTEMS_SUCCESSFUL) {
            cmdbuf[0] = SPI_MEM_CMD_RDSR;
            ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,1);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }
    }
    else {
        /*
         * send read command and address
         */
        if (rc == RTEMS_SUCCESSFUL) {
            cmdbuf[0] = SPI_MEM_CMD_READ;
            if (mem_param_ptr->mem_size > 0x10000 /* 256*256 */) {
                cmdbuf[1] = (off >> 16) & 0xff;
                cmdbuf[2] = (off >>  8) & 0xff;
                cmdbuf[3] = (off >>  0) & 0xff;
                cmd_size  = 4;
            }
            else if (mem_param_ptr->mem_size > 256) {
                cmdbuf[1] = (off >>  8) & 0xff;
                cmdbuf[2] = (off >>  0) & 0xff;
                cmd_size  = 3;
            }
            else {
                cmdbuf[1] = (off >>  0) & 0xff;
                cmd_size  = 1;
            }
            ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,cmd_size);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }
    }
    /*
     * fetch read data
     */
    if (rc == RTEMS_SUCCESSFUL) {
        ret_cnt = rtems_libi2c_read_bytes (minor,buf,cnt);
        if (ret_cnt < 0) {
            rc = -ret_cnt;
        }
    }

    /*
     * terminate transfer
     */
    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_stop(minor);
    }
    rwargs->bytes_moved = (rc == RTEMS_SUCCESSFUL) ? ret_cnt : 0;

    spi_n25q_unlock(mem_local_ptr->operationMutex);
    return rc;
}


rtems_status_code spi_N25Q_write_enable(rtems_device_major_number major,
                                        rtems_device_minor_number minor)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code rc = RTEMS_SUCCESSFUL;
    unsigned char cmdbuf[1];
    int ret_cnt = 0;

    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_start(minor);
    }
    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_addr(minor,TRUE);
    }
    if (rc == RTEMS_SUCCESSFUL) {
        cmdbuf[0] = SPI_MEM_CMD_WREN;
        ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,1);
        if (ret_cnt < 0) {
            rc = -ret_cnt;
        }
    }
    if (rc == RTEMS_SUCCESSFUL) {
        rc = rtems_libi2c_send_stop(minor);
    }
    return rc;
}

rtems_status_code spi_N25Q_erase(rtems_device_major_number major,
                                 rtems_device_minor_number minor,
                                 u32 offset,  u32 size)
{
    rtems_status_code          rc = RTEMS_SUCCESSFUL;
    off_t                     off = offset;
    unsigned char       cmdbuf[4];
    int                   ret_cnt = 0;
    spi_n25q_param_t  *mem_param_ptr;
    spi_n25q_local_t  *mem_local_ptr;

    if (rc == RTEMS_SUCCESSFUL) {
        rc = spi_N25Q_minor2param_ptr(minor, &mem_param_ptr, &mem_local_ptr);
    }

    spi_n25q_lock(mem_local_ptr->operationMutex);

    for(off = offset ;
        rc == RTEMS_SUCCESSFUL && off < offset + size;
        )
    {
        spi_N25Q_write_enable(major, minor);

        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_start(minor);
        }
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_addr(minor,TRUE);
        }
        if (rc == RTEMS_SUCCESSFUL) {
            if ((offset + size) - off >= mem_param_ptr->sector_size) {
                cmdbuf[0] = SPI_MEM_CMD_SE;
            }
            else {
                cmdbuf[0] = SPI_MEM_CMD_SSE;
            }
            cmdbuf[1] = (off >> 16) & 0xff;
            cmdbuf[2] = (off >>  8) & 0xff;
            cmdbuf[3] = (off >>  0) & 0xff;
            ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,4);
            if (ret_cnt < 0) {
                rc = -ret_cnt;
            }
        }
        if (rc == RTEMS_SUCCESSFUL) {
            rc = rtems_libi2c_send_stop(minor);
        }
        if ((offset + size) - off >= mem_param_ptr->sector_size) {
            off += mem_param_ptr->sector_size;
        }
        else {
            off += mem_param_ptr->subsector_size;
        }
        spi_N25Q_poll_write_done(major, minor, 60);
    }

    spi_n25q_unlock(mem_local_ptr->operationMutex);
    return rc;
}

rtems_status_code spi_N25Q_set_baudrate(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        int* baudrate)
{
    rtems_status_code rc = RTEMS_SUCCESSFUL;
    spi_n25q_param_t  *mem_param_ptr;
    spi_n25q_local_t  *mem_local_ptr;
    UNUSED(major);
    /*
     * get mem parameters
     */
    if (rc == RTEMS_SUCCESSFUL) {
        rc = spi_N25Q_minor2param_ptr(minor,&mem_param_ptr, &mem_local_ptr);
    }

    if (mem_param_ptr == NULL)
        return RTEMS_INVALID_ADDRESS;

    if (rc == RTEMS_SUCCESSFUL) {
        mem_param_ptr->baudrate = MIN((*baudrate), MAX_N25Q_BAUDRATE);
    }

    return rc;
}

rtems_status_code spi_N25Q_control(rtems_device_major_number major,
                                   rtems_device_minor_number minor,
                                   void  *arg)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;

    if (!ctl)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case FLASH_CMD_ERASE:
    {
        struct spiFlashN25QEraseArgs_t *args =
            (struct spiFlashN25QEraseArgs_t *)ctl->buffer;

        int sc = spi_N25Q_erase(major, minor,
                            args->offset,
                            args->size);

        ctl->ioctl_return = sc;
        return sc;
    }break;
    case FLASH_SET_BAUDRATE:
    {
        int *baudrate = (int*)ctl->buffer;

        int sc = spi_N25Q_set_baudrate(major, minor, baudrate);

        ctl->ioctl_return = sc;
        return sc;
    }break;
    default:
        return -RTEMS_INVALID_NUMBER;
    }
}

rtems_status_code spi_N25Q_init(rtems_device_major_number major,
                                rtems_device_minor_number minor,
                                void  *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint8_t rxbuf[FLASH_CMD_SIZE + FLASH_ID_SIZE];
    uint8_t txbuf[FLASH_CMD_SIZE + FLASH_ID_SIZE];
    rtems_libi2c_read_write_t rwop;
    int ret_cnt;

    if (sc == RTEMS_SUCCESSFUL) {
        sc = rtems_libi2c_send_start(minor);
    }

    if (sc == RTEMS_SUCCESSFUL) {
        sc = rtems_libi2c_send_addr(minor,TRUE);
    }

    txbuf[0] = SPI_MEM_CMD_RDID;

    rwop.byte_cnt = FLASH_CMD_SIZE + FLASH_ID_SIZE;
    rwop.rd_buf = &rxbuf[0];
    rwop.wr_buf = &txbuf[0];
    if (sc == RTEMS_SUCCESSFUL) {
        ret_cnt = rtems_libi2c_ioctl (minor,
                                      RTEMS_LIBI2C_IOCTL_READ_WRITE,
                                      &rwop);
        if (ret_cnt < 0) {
            sc = -ret_cnt;
        }
    }

    if (sc == RTEMS_SUCCESSFUL) {
        sc = rtems_libi2c_send_stop(minor);
    }

    return sc;
}

rtems_driver_address_table spi_N25Q_memdrv_rw_ops = {
    .initialization_entry = spi_N25Q_init,
    .open_entry = spi_N25Q_open,
    .close_entry = spi_N25Q_close,
    .read_entry =  spi_N25Q_read,
    .write_entry = spi_N25Q_write,
    .control_entry = spi_N25Q_control,
};

static spi_n25q_t spi_flash_N25Q_rw_drv_t = {
    {/* public fields */
        .ops =         &spi_N25Q_memdrv_rw_ops, /* operations of general memdrv */
        .size =        sizeof (spi_flash_N25Q_rw_drv_t),
    },
    { /* our private fields */
        .baudrate =             MAX_N25Q_BAUDRATE,
        .erase_before_program = true,
        .empty_state =          0xff,
        .page_size =            256,     /* programming page size in bytes */
        .subsector_size =       0x1000, /* 4K - erase sub-sector size in bytes */
        .sector_size =          0x10000, /* 64K - erase sector size in bytes */
        .mem_size =             0x800000, /* 1M - total capacity in bytes */
    },
    {
        .operationMutex = 0,
        .refCnt = 0,
    }
};

rtems_libi2c_drv_t *spi_flash_N25Q_driver_descriptor =
    &spi_flash_N25Q_rw_drv_t.libi2c_drv_entry;
