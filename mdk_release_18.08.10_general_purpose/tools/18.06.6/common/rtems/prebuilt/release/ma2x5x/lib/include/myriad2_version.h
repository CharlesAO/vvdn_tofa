#ifndef _INCLUDE_MYRIAD2_VERSION_H
#define _INCLUDE_MYRIAD2_VERSION_H

#define MV_VERSION_ID_MAJOR 0x04
#define MV_VERSION_ID_MINOR 0x00
#define MV_VERSION_ID_BUILD 0x01
#define MV_VERSION_ID_REV   0x00

#define MV_VERSION_SH_MAJOR   24  //MS Byte
#define MV_VERSION_SH_MINOR   16
#define MV_VERSION_SH_BUILD    8
#define MV_VERSION_SH_REV      0  //LS Byte
#define MV_VERSION_MASK     0xFF
#define MV_VERSION_MAJOR    ((MV_VERSION_ID_MAJOR&MV_VERSION_MASK)<<MV_VERSION_SH_MAJOR)
#define MV_VERSION_MINOR    ((MV_VERSION_ID_MINOR&MV_VERSION_MASK)<<MV_VERSION_SH_MINOR)
#define MV_VERSION_BUILD    ((MV_VERSION_ID_BUILD&MV_VERSION_MASK)<<MV_VERSION_SH_BUILD)
#define MV_VERSION_REV      ((MV_VERSION_ID_REV  &MV_VERSION_MASK)<<MV_VERSION_SH_REV  )
#define MV_RTEMS_VERSION    (MV_VERSION_MAJOR|MV_VERSION_MINOR|MV_VERSION_BUILD|MV_VERSION_REV)

#endif