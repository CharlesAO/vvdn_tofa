#include "stdio.h"
#include <stdlib.h>
#include "system.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <OsWm8325.h>


static int open_wm8325(void)
{
    int fd;

    fd = open( "/dev/i2c2." WM8325_NAME , O_RDWR );
    if (fd < 0) {
        perror("cannot open");
        exit(fd);
    }

    return fd;
}

static int set_led_wm8325(u32 wm8325fd, u32 nled, u32 state)
{
    int sc;
    wm8325LedStruct_t led;

    led.source = WM8325_MANUAL;

    if (nled > LED2) {
        nled = LED2;
    }

    led.nr = nled;

    if (state) {
        led.mode = WM8325_ON;
    } else {
        led.mode = WM8325_OFF;
    }

    sc = ioctl(wm8325fd, WM8325_SET_LED, &led);
    if (sc) {
        perror("ioctl fail");
    }

    return sc;
}

rtems_task blink_task(rtems_task_argument unused)
{
    UNUSED(unused);

    u32 wm8325fd;
    u32 state = 0;
    u32 nled = LED1;

    wm8325fd = open_wm8325();

    while (1) {
        set_led_wm8325(wm8325fd, nled, state);
        state ^= 1;
        rtems_task_wake_after(250);
    }
}

/* end of file */