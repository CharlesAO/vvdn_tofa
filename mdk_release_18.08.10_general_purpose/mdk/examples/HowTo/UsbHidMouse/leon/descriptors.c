///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mouse and keyboard descriptors
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "mousedefs.h"
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// The keyboard report image.
ROM UCHAR BootKeyboardReportDescriptor[] =
    {
        0x05,   0x01,    /* Usage Page (Generic Desktop), */
        0x09,   0x06,    /* Usage (Keyboard), */
        0xA1,   0x01,    /* Collection (Application), */
        0x05,   0x07,    /*  Usage Page (Key Codes); */
        0x19,   0xE0,    /*  Usage Minimum (224), */
        0x29,   0xE7,    /*  Usage Maximum (231), */
        0x15,   0x00,    /*  Logical Minimum (0), */
        0x25,   0x01,    /*  Logical Maximum (1), */
        0x75,   0x01,    /*  Report Size (1), */
        0x95,   0x08,    /*  Report Count (8), */
        0x81,   0x02,    /*  Input (Data, Variable, Absolute),
            ||    Modifier byte
            */
        0x95,   0x01,    /*  Report Count (1), */
        0x75,   0x08,    /*  Report Size (8), */
        0x81,   0x01,    /*  Input (Constant),
            ||     Reserved byte
            */
        0x95,   0x05,    /*  Report Count (5), */
        0x75,   0x01,    /*  Report Size (1), */
        0x05,   0x08,    /*  Usage Page (Page# for LEDs), */
        0x19,   0x01,    /*  Usage Minimum (1), */
        0x29,   0x05,    /*  Usage Maximum (5), */
        0x91,   0x02,    /*  Output (Data, Variable, Absolute),
            ||      LED report
            */
        0x95,   0x01,    /*  Report Count (1), */
        0x75,   0x03,    /*  Report Size (3), */
        0x91,   0x01,    /*  Output (Constant),
            ||       LED report padding
            */
        0x95,   0x06,    /*  Report Count (6), */
        0x75,   0x08,    /*  Report Size (8), */
        0x15,   0x00,    /*  Logical Minimum (0), */
        0x25,   0x65,    /*  Logical Maximum(101), */
        0x05,   0x07,    /*  Usage Page (Key Codes), */
        0x19,   0x00,    /*  Usage Minimum (0), */
        0x29,   0x65,    /*  Usage Maximum (101), */
        0x81,   0x00,    /*  Input (Data, Array),
            ||        Key arrays (6 bytes)
            */
        0xC0        /* End Collection */
    };

ROM USHORT sizeof_BootKeyboardReportDescriptor = sizeof(BootKeyboardReportDescriptor);

// The Mouse report image (simple boot mouse, no pm)

ROM UCHAR WheelMouseWithWakeupReportDescriptor[] =
    {
    0x05, 0x01,    /* Usage Page (Generic Desktop) */
    0x09, 0x02,    /* Usage (Mouse) */
    0xA1, 0x01,    /*  Collection(Application) */
    0x09, 0x01,    /*   Usage (pointer) */
    0xA1, 0x00,    /*   Collection (Physical) */
    0x05, 0x09,    /*    Usage Page (Buttons) */
    0x19, 0x01,    /*    Usage minimum (01) */
    0x29, 0x03,    /*    Usate maximum (03) */
    0x15, 0x00,    /*    Logical minimum (0) */
    0x25, 0x01,    /*    Logical maximum (1) */
    0x75, 0x01,    /*    Report size (1) */
    0x95, 0x03,    /*    Report count (3) */
    0x81, 0x02,    /*    Input (Data, Variable, Absolute) */
    0x75, 0x95,    /*    Report size (5) */
    0x95, 0x01,    /*    Report count (1) */
    0x81, 0x01,    /*    Input (Constant)  [5 bit padding] */
    0x05, 0x01,    /*    Usage page (generic desktop) */
    0x09, 0x30,    /*    Usage (X), */
    0x09, 0x31,    /*    Usage (Y), */
    0x09, 0x38,    /*    Usage (Wheel), */
    0x15, 0x81,    /*    Logical minimum (-127) */
    0x25, 0x7F,    /*    Logical max (+127) */
    0x75, 0x08,    /*    Report size (8) */
    0x95, 0x03,    /*    Report count (3) */
    0x81, 0x06,    /*    Input (Data, Variable, Relative) [3 pos bytes] */
    0xC0,        /*   End Collection */
    0x09, 0x3C,    /* Usage (Motion Wakeup) */
    0x05, 0xFF,    /* Usage Page (vendor specific [-1]) */
    0x09, 0x01,    /* Usage (who knows) */
    0x15, 0x00,    /* Logical minimum(0) */
    0x25, 0x01,    /* Logical maximum(0) */
    0x75, 0x01,    /* Report size(1) */
    0x95, 0x02,    /* Report count(2) */
    0xB1, 0x22,    /* Feature(Data, Variable, Abs, Nowrap, linear,  */
            /*    no preferred state, no null, */
            /*    non-volatile, bitfield) */
    0x75, 0x06,    /* Report size(6) */
    0x95, 0x01,    /* Report count(1) */
    0xB1, 0x01,    /* Feature(Constant) (6 bit padding) */
    0xC0        /* End Collection */
    };

ROM USHORT sizeof_WheelMouseWithWakeupReportDescriptor = sizeof(WheelMouseWithWakeupReportDescriptor);

// A descriptor for the WheelMouse, w/o motion wakeup
ROM UCHAR WheelMouseReportDescriptor[] =
    {
    0x05, 0x01,    /* Usage Page (Generic Desktop) */
    0x09, 0x02,    /* Usage (Mouse) */
    0xA1, 0x01,    /*  Collection(Application) */
    0x09, 0x01,    /*   Usage (pointer) */
    0xA1, 0x00,    /*   Collection (Physical) */
    0x05, 0x09,    /*    Usage Page (Buttons) */
    0x19, 0x01,    /*    Usage minimum (01) */
    0x29, 0x03,    /*    Usate maximum (03) */
    0x15, 0x00,    /*    Logical minimum (0) */
    0x25, 0x01,    /*    Logical maximum (1) */
    0x75, 0x01,    /*    Report size (1) */
    0x95, 0x03,    /*    Report count (3) */
    0x81, 0x02,    /*    Input (Data, Variable, Absolute) */
    0x75, 0x95,    /*    Report size (5) */
    0x95, 0x01,    /*    Report count (1) */
    0x81, 0x01,    /*    Input (Constant)  [5 bit padding] */
    0x05, 0x01,    /*    Usage page (generic desktop) */
    0x09, 0x30,    /*    Usage (X), */
    0x09, 0x31,    /*    Usage (Y), */
    0x09, 0x38,    /*    Usage (Wheel), */
    0x15, 0x81,    /*    Logical minimum (-127) */
    0x25, 0x7F,    /*    Logical max (+127) */
    0x75, 0x08,    /*    Report size (8) */
    0x95, 0x03,    /*    Report count (3) */
    0x81, 0x06,    /*    Input (Data, Variable, Relative) [3 pos bytes] */
    0xC0,        /*   End Collection */
    0xC0        /* End Collection */
    };

ROM USHORT sizeof_WheelMouseReportDescriptor = sizeof(WheelMouseReportDescriptor);

// A descriptor for the standard mouse with wakeup.
ROM UCHAR StandardMouseWithWakeupReportDescriptor[] =
    {
    0x05, 0x01,    /* Usage Page (Generic Desktop) */
    0x09, 0x02,    /* Usage (Mouse) */
    0xA1, 0x01,    /*  Collection(Application) */
    0x09, 0x01,    /*   Usage (pointer) */
    0xA1, 0x00,    /*   Collection (Physical) */
    0x05, 0x09,    /*    Usage Page (Buttons) */
    0x19, 0x01,    /*    Usage minimum (01) */
    0x29, 0x03,    /*    Usage maximum (03) */
    0x15, 0x00,    /*    Logical minimum (0) */
    0x25, 0x01,    /*    Logical maximum (1) */
    0x75, 0x01,    /*    Report size (1) */
    0x95, 0x03,    /*    Report count (3) */
    0x81, 0x02,    /*    Input (Data, Variable, Absolute) */
    0x75, 0x95,    /*    Report size (5) */
    0x95, 0x01,    /*    Report count (1) */
    0x81, 0x01,    /*    Input (Constant)  [5 bit padding] */
    0x05, 0x01,    /*    Usage page (generic desktop) */
    0x09, 0x30,    /*    Usage (X), */
    0x09, 0x31,    /*    Usage (Y), */
    0x15, 0x81,    /*    Logical minimum (-127) */
    0x25, 0x7F,    /*    Logical max (+127) */
    0x75, 0x08,    /*    Report size (8) */
    0x95, 0x02,    /*    Report count (2) */
    0x81, 0x06,    /*    Input (Data, Variable, Relative) [2 pos bytes] */
    0xC0,        /*   End Collection */
    0x09, 0x3C,    /* Usage (Motion Wakeup) */
    0x05, 0xFF,    /* Usage Page (vendor specific [-1]) */
    0x09, 0x01,    /* Usage (who knows) */
    0x15, 0x00,    /* Logical minimum(0) */
    0x25, 0x01,    /* Logical maximum(0) */
    0x75, 0x01,    /* Report size(1) */
    0x95, 0x02,    /* Report count(2) */
    0xB1, 0x22,    /* Feature(Data, Variable, Abs, Nowrap, linear,  */
            /*    no preferred state, no null, */
            /*    non-volatile, bitfield) */
    0x75, 0x06,    /* Report size(6) */
    0x95, 0x01,    /* Report count(1) */
    0xB1, 0x01,    /* Feature(Constant) (6 bit padding) */
    0xC0        /* End Collection */
    };

ROM USHORT sizeof_StandardMouseWithWakeupReportDescriptor = sizeof(StandardMouseWithWakeupReportDescriptor);

// This is the boot mouse report descriptor.
ROM UCHAR BootMouseReportDescriptor[] =
    {
    0x05, 0x01,    /* Usage Page (Generic Desktop) */
    0x09, 0x02,    /* Usage (Mouse) */
    0xA1, 0x01,    /*  Collection(Application) */
    0x09, 0x01,    /*   Usage (pointer) */
    0xA1, 0x00,    /*   Collection (Physical) */
    0x05, 0x09,    /*    Usage Page (Buttons) */
    0x19, 0x01,    /*    Usage minimum (01) */
    0x29, 0x03,    /*    Usage maximum (03) */
    0x15, 0x00,    /*    Logical minimum (0) */
    0x25, 0x01,    /*    Logical maximum (1) */
    0x95, 0x03,    /*    Report count (3) */
    0x75, 0x01,    /*    Report size (1) */
    0x81, 0x02,    /*    Input (Data, Variable, Absolute) */
    0x95, 0x01,    /*    Report count (1) */
    0x75, 0x05,    /*    Report size (5) */
    0x81, 0x01,    /*    Input (Constant)  [5 bit padding] */
    0x05, 0x01,    /*    Usage page (generic desktop) */
    0x09, 0x30,    /*    Usage (X), */
    0x09, 0x31,    /*    Usage (Y), */
    0x15, 0x81,    /*    Logical minimum (-127) */
    0x25, 0x7F,    /*    Logical max (+127) */
    0x75, 0x08,    /*    Report size (8) */
    0x95, 0x02,    /*    Report count (2) */
    0x81, 0x06,    /*    Input (Data, Variable, Relative) [2 pos bytes] */
    0xC0,        /*   End Collection */
    0xC0        /* End Collection */
    };

ROM USHORT sizeof_BootMouseReportDescriptor = sizeof(BootMouseReportDescriptor);
