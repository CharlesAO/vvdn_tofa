#ifndef _LIBCHIP_SPI_FLASH_N25Q_H
#define _LIBCHIP_SPI_FLASH_N25Q_H

#include <mv_types.h>
#include <mvMacros.h>
#include <rtems.h>
#include <rtems/libi2c.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
    rtems_id operationMutex;
    int32_t  refCnt;
    } spi_n25q_local_t;

    typedef struct {
    uint32_t baudrate;              /* tfr rate, bits per second     */
    bool     erase_before_program;
    uint32_t empty_state;           /* value of erased cells         */
    uint32_t page_size;             /* programming page size in byte */
    uint32_t subsector_size;        /* erase sub-sector size in byte */
    uint32_t sector_size;           /* erase sector size in byte     */
    int32_t  mem_size;              /* total capacity in byte        */
    } spi_n25q_param_t;

    typedef struct {
    rtems_libi2c_drv_t  libi2c_drv_entry;   /* general i2c/spi params */
    spi_n25q_param_t    spi_memdrv_param;   /* private parameters     */
    spi_n25q_local_t    localParams;
    } spi_n25q_t;

    typedef enum spiFlashN25QIoctls_t
    {
        FLASH_CMD_ERASE,
        FLASH_SET_BAUDRATE,
    } spiFlashN25QIoctls_t;

    typedef struct spiFlashN25QEraseArgs_t
    {
        u32 offset;
        u32 size;
    } spiFlashN25QEraseArgs_t;

/*
 * pass this descriptor pointers to rtems_libi2c_register_drv
 */
    extern rtems_libi2c_drv_t *spi_flash_N25Q_driver_descriptor;

#ifdef __cplusplus
}
#endif

#endif
