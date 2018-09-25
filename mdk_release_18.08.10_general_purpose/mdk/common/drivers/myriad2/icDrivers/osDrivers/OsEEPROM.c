///   
/// @file                             
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved. 
///            For License Warranty see: common/license.txt   
///
/// @brief     MV0182 R4 onwards EEPROM driver
/// 
/// 
///
#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include <stdio.h>
#include <OsEEPROM.h>

//#define DEBUG_ENABLED_1

#ifdef DEBUG_ENABLED_1
#define DPRINTF(...) printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

// TBD allow more than one EEPROM device or rebase RTEMS and use generic EEPROM 

// Max address bytes
#define EEPROM_MAXADRESSBYTES                           sizeof(u32)
// Size of address
#define DEFAULT_ADDRESS_SIZE                            sizeof(u16)   
// Maximum offset allowed
#define DEFAULT_MAX_OFFSET                              (4096)  

// Information on the memory
static osEEPROMDev_t osEEPROMDeviceInfo = {DEFAULT_ADDRESS_SIZE, DEFAULT_MAX_OFFSET};

// Function to set the offset when reading 
static rtems_status_code eepromSeek(rtems_device_minor_number minor, u32 offset, u32 read, s32 address_length)
{
    rtems_status_code sc = RTEMS_IO_ERROR;
    // We are little endian !
    u8 address[EEPROM_MAXADRESSBYTES] = {        
        (u8) (offset >> 24),  
        (u8) (offset >> 16),
        (u8) (offset >> 8),
        (u8) offset
    };
    s32 bytes = 0;

    sc = rtems_libi2c_send_start(minor);
    DPRINTF("rtems_libi2c_send_start minor %d res %d\n", minor, sc);
    // Start
    if (sc != RTEMS_SUCCESSFUL)        
        return sc;
    // Send I2C Addr and select write
    sc = rtems_libi2c_send_addr(minor, 0);
    DPRINTF("rtems_libi2c_send_addr minor %d read %d res %d\n", minor, 0, sc);
    if (sc != RTEMS_SUCCESSFUL)        
        return sc;    
    // Address will start on index depending on length of address
    bytes = rtems_libi2c_write_bytes(minor, (u8 *) &address[EEPROM_MAXADRESSBYTES - address_length] ,
                                  address_length);
    DPRINTF("rtems_libi2c_write_bytes minor %d address %x Addr Size %d res %d\n", minor, offset, 
                                                                                address_length, bytes);
    if (bytes != address_length)
        return RTEMS_IO_ERROR;    
    if (read) {
        // Do a new start before reading
        sc = rtems_libi2c_send_start(minor);
        DPRINTF("rtems_libi2c_send_start minor %d res %d\n", minor, sc);
        if (sc)
            return(sc);
        // Send I2C Addr again this time for reading
        sc = rtems_libi2c_send_addr(minor, 1);
        DPRINTF("rtems_libi2c_send_addr minor %d read %d res %d\n", minor, read, sc);
    }
    return sc;
}

// Init function
static rtems_status_code eepromInit(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major);// hush the compiler warning.

    u8 buffer[sizeof(u32)] = {0};
    rtems_status_code sc = RTEMS_IO_ERROR;
    s32 bytes = 0;

    // Capture parameters
    osEEPROMDev_t *devInfo = (osEEPROMDev_t *) arg;
    if (devInfo)
        osEEPROMDeviceInfo = *devInfo;
    else {
        // If not provided use default ones
        osEEPROMDeviceInfo.address_length = DEFAULT_ADDRESS_SIZE;
        osEEPROMDeviceInfo.eeprom_size = DEFAULT_MAX_OFFSET;
    }

    // Set the pointer at the beginning of the EEPROM memory
    sc = eepromSeek(minor, 0, 1, osEEPROMDeviceInfo.address_length);
    if (sc == RTEMS_SUCCESSFUL) {
        // Read first 4 bytes
        bytes = rtems_libi2c_read_bytes(minor, (void *) buffer, sizeof(buffer));
        // Check return value
        if (bytes < 0)
            return bytes;              
        sc = rtems_libi2c_send_stop(minor);
        // Check return value & bytes moved
        if ((sc == RTEMS_SUCCESSFUL) && (bytes != sizeof(buffer))) {            
            sc = RTEMS_IO_ERROR;
        }
    }      
    return -sc;
}

// Standard read function
static rtems_status_code eepromWrite(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)                                         
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    return RTEMS_NOT_IMPLEMENTED;
}

// Standard read function
static rtems_status_code eepromRead(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major);// hush the compiler warning.

    rtems_libio_rw_args_t *rwargs = (rtems_libio_rw_args_t *) arg;
    rtems_status_code sc = RTEMS_IO_ERROR;
    s32 bytes = 0;

    // Limit the operation if it goes beyond the device size
    if ((rwargs->offset + rwargs->count) > osEEPROMDeviceInfo.eeprom_size)
        rwargs->count = osEEPROMDeviceInfo.eeprom_size - rwargs->offset;
    
    // Set offset in EEPROM Memory for reading
    sc = eepromSeek(minor, rwargs->offset, 1, osEEPROMDeviceInfo.address_length);
    if (sc == RTEMS_SUCCESSFUL) {
        // Read data
        bytes = rtems_libi2c_read_bytes(minor, (void *) rwargs->buffer, rwargs->count);
        DPRINTF("rtems_libi2c_read_bytes minor %d data %p Length %d res %d\n", minor, rwargs->buffer, 
                                                                               rwargs->count , bytes);   
        if (bytes < 0)
            return bytes;
        // Update bytes moved based on result            
        rwargs->bytes_moved = bytes;        
        // Send Stop bit 
        sc = rtems_libi2c_send_stop(minor);
        DPRINTF("rtems_libi2c_send_stop minor %d res %d\n", minor, sc);        
    }
    return sc;  
}


// Driver operations
static rtems_driver_address_table eepromReadOnlyDrvOps = {
    .initialization_entry = eepromInit,
    .read_entry = eepromRead,
    .write_entry = eepromWrite,     
};

// Driver Table
const rtems_libi2c_drv_t eepromDrvTbl = {
    .ops = &eepromReadOnlyDrvOps,
    .size = sizeof(eepromReadOnlyDrvOps),
};


