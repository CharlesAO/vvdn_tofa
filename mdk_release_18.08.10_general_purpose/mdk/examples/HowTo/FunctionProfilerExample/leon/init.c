/*
 * init.c
 *
 *  Created on: Feb 24, 2016
 *      Author: ovidiuandoniu
 */


#include <DrvLeon.h>
#include <OsDrvTimer.h>
#include <DrvIcb.h>
#include <DrvTimer.h>
#include <stdio.h>


#include "rtems_config.h"

#define THREAD_NUM 3

#define USED_TIMER_0   2
#define USED_TIMER_1   3

#define LEVEL_TIMER_0   10
#define LEVEL_TIMER_1   11

#define CFG_TIMER_TICKS       (500)

// Private Register Bitfield descriptions
#define D_TIMER_CFG_ENABLE          (1 << 0)
#define D_TIMER_CFG_RESTART         (1 << 1)
#define D_TIMER_CFG_EN_INT          (1 << 2)
#define D_TIMER_CFG_CHAIN           (1 << 3)
#define D_TIMER_CFG_IRQ_PENDING     (1 << 4)
#define D_TIMER_CFG_FORCE_RELOAD    (1 << 5)

int stuff = 0;
void incStuff() {
    stuff++;
}

void timerIrqHandler(u32 icb_src)
{
    u32 timer = icb_src - IRQ_TIMER;
    u32 regAddr = (TIM0_BASE_ADR + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer);
    u32 mask = (D_TIMER_CFG_IRQ_PENDING | D_TIMER_CFG_EN_INT);

    DrvClearTimConfigRegBit(regAddr,mask);

     incStuff();
    // Clear the irq in ICB (only needed if the interrupt is configured on edge)
    DrvIcbIrqClear(IRQ_TIMER + timer);
}

// TODO: are the following kind of generic mdk stuff? or for profiler?
uint32_t timerConfig;

static void configTimer(u32 timer, int priority)
{
    // Ensure there is no IRQ Pending before enabling Irqs
    CLR_REG_BITS_MASK((TIM0_BASE_ADR + TIM0_CONFIG_OFFSET - 0x10) + (0x10 * timer), D_TIMER_CFG_IRQ_PENDING);

    DrvIcbSetupIrq(IRQ_TIMER + timer, priority, POS_LEVEL_INT, timerIrqHandler );

    // Load the current reload value into the countdown timer, enable countdown and irq
    // don't yet write to the timer register
    timerConfig = D_TIMER_CFG_FORCE_RELOAD    |
                  D_TIMER_CFG_ENABLE          |
                  D_TIMER_CFG_EN_INT;
}

//void setHandler() {
//	rtems_extension_create();
//}

void __attribute__((no_instrument_function)) testing() {}


void initProfilerTimers() {
	initClocksAndMemory();

	OsDrvTimerInit();
	configTimer(USED_TIMER_0, LEVEL_TIMER_0);
	configTimer(USED_TIMER_1, LEVEL_TIMER_1);
	DrvStartTimer(USED_TIMER_0, CFG_TIMER_TICKS, timerConfig);
	DrvStartTimer(USED_TIMER_1, CFG_TIMER_TICKS, timerConfig);
}





