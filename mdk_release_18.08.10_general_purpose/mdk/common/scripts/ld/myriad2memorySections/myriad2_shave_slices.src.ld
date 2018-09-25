  /*Linker script file which sets all shave slices code*/

  /* Beware of the nl tags at the end of each line.
   * They are used with sed to produce readable output ldscript */

#ifndef SHV_DEFAULT_STACK_SIZE
#define SHV_DEFAULT_STACK_SIZE   (8*1024)
#endif

#define ERROR_MSG(x, sect, MEM) ERROR: S.shv##x.cmx.##sect could not be fit inside defined memory region SHV##x##_##MEM !
#define QUOTE "

#define SHAVE_SLICE(x)                                                              nl\
  S.shv##x.cmx.text : {                                                             nl\
    __shave##x##_cmx_text_start = .;                                                nl\
    lrt___shave##x##_cmx_text_start = .;                                            nl\
    *(.shv##x.S.text*)                                                              nl\
    *(.shv##x.S.__TEXT__sect)                                                       nl\
    *(.shv##x.S.__MAIN__sect)                                                       nl\
    *(.shv##x.S.init*)                                                              nl\
    *(.lrt.shv##x.S.text*)                                                          nl\
    *(.lrt.shv##x.S.__TEXT__sect)                                                   nl\
    *(.lrt.shv##x.S.__MAIN__sect)                                                   nl\
    *(.lrt.shv##x.S.init*)                                                          nl\
    __shave##x##_cmx_text_end = .;                                                  nl\
    lrt___shave##x##_cmx_text_end = .;                                              nl\
  } > SHV##x##_CODE                                                                 nl\
                                                                                    nl\
  S.shv##x.cmx.text.free (NOLOAD): {                                                nl\
    __TEXT_FREE_START_SHV##x = ORIGIN(SHV##x##_CODE) + SIZEOF(S.shv##x.cmx.text);   nl\
    __TEXT_FREE_SIZE_SHV##x = ORIGIN(SHV##x##_CODE) + LENGTH(SHV##x##_CODE)           \
                              - __TEXT_FREE_START_SHV##x;                           nl\
    FILL(0);                                                                        nl\
    . = ( __TEXT_FREE_SIZE_SHV##x > 0) ? ORIGIN(SHV##x##_CODE) + LENGTH(SHV##x##_CODE) : . ;  nl\
    __TEXT_FREE_END_SHV##x = .;                                                     nl\
  } > SHV##x##_CODE                                                                 nl\
                                                                                    nl\
  __CMX_CODE_SIZE_SHV##x = SIZEOF(S.shv##x.cmx.text) +                                \
                           SIZEOF(S.shv##x.cmx.text.free);                          nl\
  ASSERT(__CMX_CODE_SIZE_SHV##x <= LENGTH(SHV##x##_CODE),                             \
  QUOTE ERROR_MSG(x,text,CODE) QUOTE)                                               nl\
                                                                                    nl\
  S.shv##x.cmx.data : {                                                             nl\
    __shave##x##_cmx_data_start = .;                                                nl\
    lrt___shave##x##_cmx_data_start = .;                                            nl\
    *(.shv##x.S.data.__shaveData*)                                                  nl\
    *(.shv##x.S.data*)                                                              nl\
    *(.shv##x.S.rodata*)                                                            nl\
    *(.shv##x.S.__DATA__sect*)                                                      nl\
    *(.shv##x.S.__STACK__sect*)                                                     nl\
    *(.shv##x.S.__static_data*)                                                     nl\
    *(.shv##x.S.__HEAP__sect*)                                                      nl\
    *(.shv##x.S.__T__*)                                                             nl\
    *(.lrt.shv##x.S.data*)                                                          nl\
    *(.lrt.shv##x.S.rodata*)                                                        nl\
    *(.lrt.shv##x.S.__DATA__sect*)                                                  nl\
    *(.lrt.shv##x.S.__STACK__sect*)                                                 nl\
    *(.lrt.shv##x.S.__static_data*)                                                 nl\
    *(.lrt.shv##x.S.__HEAP__sect*)                                                  nl\
    *(.lrt.shv##x.S.__T__*)                                                         nl\
    __shave##x##_cmx_data_end = .;                                                  nl\
    lrt___shave##x##_cmx_data_end = .;                                              nl\
  } > SHV##x##_DATA                                                                 nl\
                                                                                    nl\
  __CMX_DATA_SIZE_SHV##x = SIZEOF(S.shv##x.cmx.data);                               nl\
                                                                                    nl\
  ASSERT( __CMX_DATA_SIZE_SHV##x  <=                                                  \
  LENGTH(SHV##x##_DATA), QUOTE ERROR_MSG(x,data,DATA) QUOTE)                        nl\
                                                                                    nl\
  S.shv##x.cmx.data.free (NOLOAD): {                                                nl\
    __DATA_FREE_START_SHV##x = ORIGIN(SHV##x##_DATA) + SIZEOF(S.shv##x.cmx.data);   nl\
    __DATA_FREE_SIZE_SHV##x = ( __DATA_FREE_START_SHV##x  > (ORIGIN(SHV##x##_DATA) + LENGTH(SHV##x##_DATA)) ) ? 0 :  \
                              (ORIGIN(SHV##x##_DATA) + LENGTH(SHV##x##_DATA)) - __DATA_FREE_START_SHV##x;          nl\
    FILL(0);                                                                        nl\
    . = (__DATA_FREE_SIZE_SHV##x >= SHV_DEFAULT_STACK_SIZE) ? \
        ORIGIN(SHV##x##_DATA) + LENGTH(SHV##x##_DATA) : . ;  nl\
    __DATA_FREE_END_SHV##x = .;                                                     nl\
  } > SHV##x##_DATA                                                                 nl\
                                                                                    nl\
  S.shv##x.stack (NOLOAD):                                                          nl\
  {                                                                                 nl\
    __STACKTOP_SHV##x  = ORIGIN(SHV##x##_DATA) + LENGTH(SHV##x##_DATA);             nl\
    __STACKSIZE_SHV##x = ( SIZEOF(S.shv##x.cmx.text) > 0) ? __STACKTOP_SHV##x - . : 0; nl\
    FILL(0);                                                                        nl\
    . = (__STACKSIZE_SHV##x >= SHV_DEFAULT_STACK_SIZE) ? __STACKTOP_SHV##x : .;     nl\
  } > SHV##x##_DATA                                                                 nl\
                                                                                    nl\
  ASSERT( ( SIZEOF(S.shv2.cmx.text) > 0)||(__STACKSIZE_SHV##x  > SHV_DEFAULT_STACK_SIZE) \
                         , QUOTE ERROR_MSG(x,stack,DATA) QUOTE) nl\
                                                                                    nl\


SECTIONS {
  SHAVE_SLICE(0)
  SHAVE_SLICE(1)
  SHAVE_SLICE(2)
  SHAVE_SLICE(3)
  SHAVE_SLICE(4)
  SHAVE_SLICE(5)
  SHAVE_SLICE(6)
  SHAVE_SLICE(7)
  SHAVE_SLICE(8)
  SHAVE_SLICE(9)
  SHAVE_SLICE(10)
  SHAVE_SLICE(11)
#ifdef MA2480
  SHAVE_SLICE(12)
  SHAVE_SLICE(13)
  SHAVE_SLICE(14)
  SHAVE_SLICE(15)
#endif
}

/*The following are empty sections but we just need to make sure their symbols get pulled in*/

SECTIONS {

  S.shvdynamic.text 0x1D000000 : {
        *(.dyn.text*)
  }

  S.shvdynamic.data 0x1C000000 : {
        *(.dyn.data*)
  }

}
