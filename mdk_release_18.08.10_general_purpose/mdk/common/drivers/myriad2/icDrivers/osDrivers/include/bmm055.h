/** \mainpage
*
****************************************************************************
* Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
*
* File : bmm050.h
*
* Date : 2016/03/17
*
* Revision : 2.0.5 $
*
* Usage: Sensor Driver for BMM050 and BMM150 sensor
*
****************************************************************************
*
* \section License
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
*   Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
*   Neither the name of the copyright holder nor the names of the
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
* OR CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
* The information provided is believed to be accurate and reliable.
* The copyright holder assumes no responsibility
* for the consequences of use
* of such information nor for any infringement of patents or
* other rights of third parties which may result from its use.
* No license is granted by implication or otherwise under any patent or
* patent rights of the copyright holder.
**************************************************************************/
/****************************************************************************/

#ifndef IMU_BMM_055_H
#define IMU_BMM_055_H
#include <rtems.h>
#include <mv_types.h>


typedef struct trimRegs_t{
    s8 dig_x1;/**< trim x1 data */
    s8 dig_y1;/**< trim y1 data */

    s8 dig_x2;/**< trim x2 data */
    s8 dig_y2;/**< trim y2 data */

    u16 dig_z1;/**< trim z1 data */
    s16 dig_z2;/**< trim z2 data */
    s16 dig_z3;/**< trim z3 data */
    s16 dig_z4;/**< trim z4 data */

    u8 dig_xy1;/**< trim xy1 data */
    s8 dig_xy2;/**< trim xy2 data */

    u16 dig_xyz1;/**< trim xyz1 data */
}trimRegs_t;

/* Trim Extended Registers */
#define BMM050_DIG_X1                      (0x5D)
#define BMM050_DIG_Y1                      (0x5E)
#define BMM050_DIG_Z4_LSB                  (0x62)
#define BMM050_DIG_Z4_MSB                  (0x63)
#define BMM050_DIG_X2                      (0x64)
#define BMM050_DIG_Y2                      (0x65)
#define BMM050_DIG_Z2_LSB                  (0x68)
#define BMM050_DIG_Z2_MSB                  (0x69)
#define BMM050_DIG_Z1_LSB                  (0x6A)
#define BMM050_DIG_Z1_MSB                  (0x6B)
#define BMM050_DIG_XYZ1_LSB                (0x6C)
#define BMM050_DIG_XYZ1_MSB                (0x6D)
#define BMM050_DIG_Z3_LSB                  (0x6E)
#define BMM050_DIG_Z3_MSB                  (0x6F)
#define BMM050_DIG_XY2                     (0x70)
#define BMM050_DIG_XY1                     (0x71)

typedef rtems_status_code (*bmmReadFunc)(rtems_device_minor_number minor,
                                         u8 regAddr, void *data, u32 size);

rtems_status_code bmm050_init_trim_registers(rtems_device_minor_number dev_addr,
                                             trimRegs_t* regs, bmmReadFunc readFunc);
float bmm050_compensate_X_float(s16 mag_data_x, u16 data_r, trimRegs_t* regs);
float bmm050_compensate_Y_float(s16 mag_data_y, u16 data_r, trimRegs_t* regs);
float bmm050_compensate_Z_float (s16 mag_data_z, u16 data_r, trimRegs_t* regs);

#endif
