/*
****************************************************************************
* Copyright (C) 2015 - 2016 Bosch Sensortec GmbH
*
* bmm050.c
* Date: 2016/03/17
* Revision: 2.0.6 $
*
* Usage: Sensor Driver for  BMM050 and BMM150 sensor
*
****************************************************************************
* License:
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

#include <bmm055.h>

rtems_status_code bmm050_init_trim_registers(rtems_device_minor_number dev_addr,
                                             trimRegs_t* regs, bmmReadFunc readFunc){

    /* variable used to return the bus communication result*/
    rtems_status_code rc = RTEMS_SUCCESSFUL;
    u8 trim_data[2];
    rc = readFunc(dev_addr,
                  BMM050_DIG_X1, (u8 *)&regs->dig_x1,
                  1);
    rc += readFunc(dev_addr,
                   BMM050_DIG_Y1, (u8 *)&regs->dig_y1,
                   1);
    rc += readFunc(dev_addr,
                   BMM050_DIG_X2, (u8 *)&regs->dig_x2,
                   1);
    rc += readFunc(dev_addr,
                   BMM050_DIG_Y2, (u8 *)&regs->dig_y2,
                   1);
    rc += readFunc(dev_addr,
                   BMM050_DIG_XY1, (u8 *)&regs->dig_xy1,
                   1);
    rc += readFunc(dev_addr,
                   BMM050_DIG_XY2, (u8 *)&regs->dig_xy2,
                   1);


    rc += readFunc(dev_addr,
    BMM050_DIG_Z1_LSB, trim_data, 2);
    regs->dig_z1 = (u16)((((u32)((u8)
    trim_data[1])) << 8) | trim_data[0]);

    rc += readFunc(dev_addr,
    BMM050_DIG_Z2_LSB, trim_data, 2);
    regs->dig_z2 = (s16)((((s32)(
    (s8)trim_data[1])) <<
    8)
    | trim_data[0]);

    rc += readFunc(dev_addr,
    BMM050_DIG_Z3_LSB, trim_data, 2);
    regs->dig_z3 = (s16)((((s32)(
    (s8)trim_data[1])) <<
    8)
    | trim_data[0]);

    rc += readFunc(dev_addr,
    BMM050_DIG_Z4_LSB, trim_data, 2);
    regs->dig_z4 = (s16)((((s32)(
    (s8)trim_data[1])) <<
    8)
    | trim_data[0]);

    rc += readFunc(dev_addr,
    BMM050_DIG_XYZ1_LSB, trim_data, 2);
    regs->dig_xyz1 = (u16)((((u32)
    ((u8)trim_data[1])) <<
    8)
    | trim_data[0]);
    return rc;
}

float bmm050_compensate_X_float(s16 mag_data_x, u16 data_r, trimRegs_t* regs)
{
    float inter_retval = 0;

    if (mag_data_x != -4096   /* no overflow */
       ) {
        if ((data_r != 0)
        && (regs->dig_xyz1 != 0)) {
            inter_retval = ((((float)regs->dig_xyz1)
            * 16384.0 / data_r) - 16384.0);
        } else {
            inter_retval = 0.0;
            return inter_retval;
        }
        inter_retval = (((mag_data_x * ((((((float)regs->dig_xy2) *
            (inter_retval*inter_retval /
            268435456.0) +
            inter_retval * ((float)regs->dig_xy1)
            / 16384.0)) + 256.0) *
            (((float)regs->dig_x2) + 160.0)))
            / 8192.0)
            + (((float)regs->dig_x1) *
            8.0)) / 16.0;
    } else {
        inter_retval = 0.0;
    }
    return inter_retval;
}

float bmm050_compensate_Y_float(s16 mag_data_y, u16 data_r, trimRegs_t* regs)
{
    float inter_retval = 0;

    if (mag_data_y != 4096 /* no overflow */
       ) {
        if ((data_r != 0)
        && (regs->dig_xyz1 != 0)) {
            inter_retval = ((((float)regs->dig_xyz1)
            * 16384.0
            /data_r) - 16384.0);
        } else {
            inter_retval = 0;
            return inter_retval;
        }
        inter_retval = (((mag_data_y * ((((((float)regs->dig_xy2) *
            (inter_retval*inter_retval
            / 268435456.0) +
            inter_retval * ((float)regs->dig_xy1)
            / 16384.0)) +
            256.0) *
            (((float)regs->dig_y2) + 160.0)))
            / 8192.0) +
            (((float)regs->dig_y1) * 8.0))
            / 16.0;
    } else {
        /* overflow, set output to 0.0f */
        inter_retval = 0;
    }
    return inter_retval;
}



float bmm050_compensate_Z_float (s16 mag_data_z, u16 data_r, trimRegs_t* regs)
{
    float inter_retval = 0;
     /* no overflow */
    if (mag_data_z != -4096) {
        if ((regs->dig_z2 != 0)
        && (regs->dig_z1 != 0)
        && (regs->dig_xyz1 != 0)
        && (data_r != 0)) {
            inter_retval = ((((((float)mag_data_z)-
            ((float)regs->dig_z4)) * 131072.0)-
            (((float)regs->dig_z3)*(((float)data_r)
            -((float)regs->dig_xyz1))))
            /((((float)regs->dig_z2)+
            ((float)regs->dig_z1)*((float)data_r) /
            32768.0) * 4.0)) / 16.0;
        }
    } else {
        /* overflow, set output to 0.0f */
        inter_retval = 0.0f;
    }
    return inter_retval;
}
