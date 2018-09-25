#pragma once

#include "dtp/dtp_server_defs.h"

#ifdef __cplusplus
extern "C" {
#endif
	typedef enum { PDS_TYPE_FIXED, PDS_TYPE_RANGE, PDS_TYPE_SET } PDS_TYPE;
	typedef enum { DSI_TYPE_MIN, DSI_TYPE_MAX, DSI_TYPE_FIXED, DSI_TYPE_UNBOUND } DSI_TYPE;
	typedef struct {
		DSI_TYPE type;
		uint32 value;
	} data_set_t;
	
	typedef struct {
		union{
			PDS_TYPE pds_type;
			uint32 data_set_item_count;
			data_set_t data_set[4];
		};
	}parameter_t;

	int cfg_data_locator_init(unsigned char * pDB, int dbID, int aID, int stID, int fixOffsets);
	void cfg_data_locator_deinit();
	int cfg_data_locator_next(dtp_out_data_t* data);
	void cfg_data_locator_reset();
	void export_db(char* filename, unsigned int size);
	int parameter_set_next(parameter_t* parameter);
	void parameter_set_reset();
	
	unsigned char* cfg_data_locator_get_db_address();
#ifdef __cplusplus
	}
#endif
