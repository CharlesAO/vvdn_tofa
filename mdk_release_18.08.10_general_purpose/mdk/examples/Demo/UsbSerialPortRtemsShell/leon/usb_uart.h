#ifndef __USB_UART_h
#define __USB_UART_h

#ifdef __cplusplus
extern "C" {
#endif

#define USB_UART_DRIVER_TABLE_ENTRY \
  { usb_uart_initialize, usb_uart_open, usb_uart_close, usb_uart_read, \
    usb_uart_write, usb_uart_control }

rtems_device_driver usb_uart_initialize(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver usb_uart_open(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver usb_uart_close(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver usb_uart_read(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver usb_uart_write(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

rtems_device_driver usb_uart_control(
  rtems_device_major_number,
  rtems_device_minor_number,
  void *
);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
