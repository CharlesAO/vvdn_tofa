#ifndef _HAT_CSVR_H_
#define _HAT_CSVR_H_

typedef struct
{
	uint8 t_scvr;
} t_csvr_t;


typedef struct
{
    uint32 tbl_size;
    t_csvr_t  *p_tbl;
} csvr_tbl_t;

#endif // _HAT_CSVR_H_
