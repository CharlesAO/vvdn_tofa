#ifndef _HAT_CALIBRATION_H_
#define _HAT_CALIBRATION_H_

typedef struct
{
	uint32 eeprom_checksim;
	uint8  layout_version_major;   //
	uint8  layout_version_minor;   //
	uint8  sensor_module_serial_number[12];   // ACSI string
	uint8  supplier_module_tracking_info[12]; // reserved
} calib_module_description_t;

#endif // _HAT_CALIBRATION_H_

