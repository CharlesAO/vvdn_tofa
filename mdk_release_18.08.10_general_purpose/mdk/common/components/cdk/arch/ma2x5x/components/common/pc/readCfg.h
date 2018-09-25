#ifndef __READ_CFG_H__
#define __READ_CFG_H__

#include <common/leon/ipipeSipp.h>

int   parseIniFile  (const char *fname, ispCfg *cfg);
void  parseAppArgs  (int argc, char **argv, int *dbgLevel, char **cfgFileName);
void  stillGetConfig(const char *cfgFile, ispCfg *cfg);
void  dumpIspConfig (ispCfg *iCfg, char *cfgName);


#endif