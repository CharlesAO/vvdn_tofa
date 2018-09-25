#include <cstdlib>
#include <string.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include "IOProcessing.h"
#include "moviDebugDll.h"
#include "moviDebug2Dll.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#ifdef MYRIAD2
#define LEON_START_CMD "start los"
#define LEON_TARGET "t los"
#define MAX_SHAVE_NUMBER 12 
#else
#define LEON_START_CMD "start l"
#define LEON_TARGET "t l"
#define MAX_SHAVE_NUMBER 8
#endif
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((!(sizeof(x) % sizeof(0[x])))))
//#define printf(...)

const unsigned int MAX_CMD_LINE = 1024;
const unsigned char PRESERVED_IRFS = 13, PRESERVED_VRFS = 8;
unsigned int IRF_address[PRESERVED_IRFS], VRF_address[PRESERVED_VRFS];
unsigned int IRF_values[PRESERVED_IRFS], VRF_values[PRESERVED_VRFS][4];
unsigned int IRF_values_at_end[PRESERVED_IRFS], VRF_values_at_end[PRESERVED_VRFS][4];
unsigned int IRF_preserved[PRESERVED_IRFS], VRF_preserved[PRESERVED_VRFS];
bool IOProcessing::isFirstTest = false;
bool IOProcessing::defaultPort = true;
bool verbose = 0; //enable/disable tcf log (currently disabled)
char target_core;

#ifdef CVMA2150
#define chipVersion "ma2150"
#endif
#ifdef CVMA2450
#define chipVersion "ma2450"
#endif
#ifdef CVMA2480
#define chipVersion "ma2480"
#endif

#ifndef MODULES_UNIT_TEST
#undef KERNEL_APP_NAME
#undef TARGET_PREFIX
#define KERNEL_APP_NAME "dummy"
#define TARGET_PREFIX "dummy0_"
#endif

#ifdef MV_MVDBG_GEN2
static unsigned int init = 0; //only for movidebug2 (used for loading elf multiple times for movidebug2)
#endif
static void log(MoviDebugUserData data, int level, const char * str)
{
    static const char * logprefix[] = {
        "[          ] ",
        "[  DEBUG   ] ",
        "[  VERBOSE ] ",
        "[  INFO    ] " ,
        "[  WARNING ] ",
        "[  ERROR   ] "
    };
    UNUSED(data);
    if (level < 0 || level >= (int)COUNT_OF(logprefix))
    {
        level = 0;
    }
    else if (level == MOVIDEBUG_LOG_DEBUG)
    {
        return;
    }
    (level < MOVIDEBUG_LOG_WARNING ? std::cout : std::cerr)
          << logprefix[level] << str << std::endl;

}
static void exit(MoviDebugUserData, int exitcode)
{
    std::cerr << "MoviDebug2 exit request with code " << exitcode << std::endl;
}
static void print(MoviDebugUserData data, const char * str)
{
    log(data, 0, str);
}
static void error(MoviDebugUserData data, const char *str)
{
    log(data, MOVIDEBUG_LOG_ERROR, str);
}

static void *allocMem(size_t size)
{
    return new char[size];
}
static void freeMem(void *data)
{
    delete [] static_cast<char *>(data);
}

const MoviDebugDllCallbacks callbacks = {
    exit,
    print,
    error,
    log,
    allocMem,
    freeMem
};

const MoviDebugDllInterface *fptr = NULL;
MoviDebugObjectPtr dbg;

//constructors
IOProcessing::IOProcessing(string prjPath) :
        projectPath(prjPath)
{
    elfLoaded = false;
    debugInterface = DLL_DBGI_JTAG;
    runWithSimulator = false;
}

IOProcessing::IOProcessing(string prjPath, unsigned int dbgInterface) :
        projectPath(prjPath), debugInterface(dbgInterface)
{ 
    mapFilePath = projectPath + "/output/"+ std::string(KERNEL_APP_NAME) + ".map";
    elfLoaded = false;
    runWithSimulator = (dbgInterface & (unsigned int)DLL_DBGI_MSIM);
}

void IOProcessing::SetProjectPath(string prjPath)
{
    projectPath = prjPath;
}

void IOProcessing::SetAsmLabel(string asmLbl)
{
    asmLabel = asmLbl;
}

void IOProcessing::SetSrvIP(char *serverIP)
{
    strcpy(srvIP, serverIP);
}

void IOProcessing::SetSrvPort(char *serverPort, bool defPort)
{
    strcpy(srvPort, serverPort);
    defaultPort = defPort;
}

void IOProcessing::storePreservedRegisters()
{
    for(int i = 0; i < PRESERVED_IRFS; i++)
     {
        string irf;
        switch(i)
        {
        case 0 : irf.assign("i19"); break;
        case 1 : irf.assign("i20"); break;
        case 2 : irf.assign("i21"); break;
        case 3 : irf.assign("i22"); break;
        case 4 : irf.assign("i23"); break;
        case 5 : irf.assign("i24"); break;
        case 6 : irf.assign("i25"); break;
        case 7 : irf.assign("i26"); break;
        case 8 : irf.assign("i27"); break;
        case 9 : irf.assign("i28"); break;
        case 10: irf.assign("i29"); break;
        case 11: irf.assign("i30"); break;
        case 12: irf.assign("i31"); break;
        }
         //fptr->symAddr(dbg, 0, const_cast<char*>(irf.c_str()), &IRF_address[i]);
         fptr->regAddr(dbg, const_cast<char*>("S0"), const_cast<char*>(irf.c_str()), &IRF_address[i]);
         fptr->read32(dbg, IRF_address[i], &IRF_values[i]);

                #ifdef MV_MVDBG_GEN1
        IRF_address[i] = DllMoviDebugGetAddress(const_cast<char*>(irf.c_str()));
        DllMoviDebugGet(IRF_address[i], 4, &IRF_values[i]);
                #endif

     }

    for(int v = 0; v < PRESERVED_VRFS; v++)
     {
        string vrf;
        switch(v)
        {
        case 0 : vrf.assign("v24"); break;
        case 1 : vrf.assign("v25"); break;
        case 2 : vrf.assign("v26"); break;
        case 3 : vrf.assign("v27"); break;
        case 4 : vrf.assign("v28"); break;
        case 5 : vrf.assign("v29"); break;
        case 6 : vrf.assign("v30"); break;
        case 7 : vrf.assign("v31"); break;
        }
        fptr->regAddr(dbg, const_cast<char*>("S0"), const_cast<char*>(vrf.c_str()), &VRF_address[v]);
        fptr->read32(dbg, VRF_address[v] +  0, &VRF_values[v][0]);
        fptr->read32(dbg, VRF_address[v] +  4, &VRF_values[v][1]);
        fptr->read32(dbg, VRF_address[v] +  8, &VRF_values[v][2]);
        fptr->read32(dbg, VRF_address[v] + 12, &VRF_values[v][3]);

                #ifdef MV_MVDBG_GEN1
        VRF_address[v] = DllMoviDebugGetAddress(const_cast<char*>(vrf.c_str()));

        DllMoviDebugGet(VRF_address[v], 4, &VRF_values[v][0]);

        DllMoviDebugGet(VRF_address[v] + 4, 4, &VRF_values[v][1]);

        DllMoviDebugGet(VRF_address[v] + 8, 4, &VRF_values[v][2]);

        DllMoviDebugGet(VRF_address[v] + 12, 4, &VRF_values[v][3]);
                #endif
     }
}

void IOProcessing::verifyPreservedRegisters()
{
        //#ifdef MV_MVDBG_GEN1
    for(int i = 0; i < PRESERVED_IRFS; i++)
     {
        //DllMoviDebugGet(IRF_address[i], 4, &IRF_values_at_end[i]);
        fptr->read32(dbg, IRF_address[i], &IRF_values_at_end[i]);
        if(IRF_values[i] == IRF_values_at_end[i])
            IRF_preserved[i] = 1;
        else
            IRF_preserved[i] = 0;
     }
    for(int v = 0; v < PRESERVED_VRFS; v++)
     {
        //DllMoviDebugGet(VRF_address[v], 4, &VRF_values_at_end[v][0]);
        //DllMoviDebugGet(VRF_address[v] + 4, 4, &VRF_values_at_end[v][1]);
        //DllMoviDebugGet(VRF_address[v] + 8, 4, &VRF_values_at_end[v][2]);
        //DllMoviDebugGet(VRF_address[v] + 12, 4, &VRF_values_at_end[v][3]);

        fptr->read32(dbg, VRF_address[v] +  0, &VRF_values_at_end[v][0]);
        fptr->read32(dbg, VRF_address[v] +  4, &VRF_values_at_end[v][1]);
        fptr->read32(dbg, VRF_address[v] +  8, &VRF_values_at_end[v][2]);
        fptr->read32(dbg, VRF_address[v] + 12, &VRF_values_at_end[v][3]);

        VRF_preserved[v] = 1;
        for (int j = 0; j < 4; j++)
            if(VRF_values[v][j] != VRF_values_at_end[v][j])
                {
                    VRF_preserved[v] = 0;
                    break;
                }
     }
         //#endif
}

//public function implementation
bool IOProcessing::runCheckingRegisters(TargetCore target)
{
    const unsigned int ip_address = 0x20f01100;
    const unsigned int i30_address = 0x20f011f8;
    unsigned int i30_value;
    char i30ss[10];
    unsigned int ip_value, second_last_value, last_value;
        UNUSED(ip_address);  //variables needed for further improvement in the unittest
        UNUSED(i30_address);
        UNUSED(i30_value);
        UNUSED(ip_value);
        UNUSED(ip_value);
        UNUSED(second_last_value);
        UNUSED(last_value);
        UNUSED(i30ss); //currently disabled in use

    if(elfLoaded)
    {
        selectTarget(target);
        #ifndef MYRIAD2
        isFirstTest = false;
        #endif

        if(isFirstTest)
        {
                        #ifdef MV_MVDBG_GEN1
            DllMoviDebugParseString(const_cast<char*>(LEON_START_CMD));
            DllMoviDebugParseString(const_cast<char*>("t s0"));
	    
            string breakpoint1("b "+ TARGET_PREFIX + asmLabel);
            string asmLabel_string(TARGET_PREFIX + asmLabel);
            
            //unsigned int asmLabel_address;
            //asmLabel_address = DllMoviDebugGetAddress(const_cast<char*>(asmLabel_string.c_str()));

            DllMoviDebugParseString(const_cast<char*>(breakpoint1.c_str()));

            DllMoviDebugParseString(const_cast<char*>(LEON_START_CMD));
            DllMoviDebugParseString(const_cast<char*>("run"));
            DllMoviDebugParseString(const_cast<char*>("t s0"));

            DllMoviDebugGet(ip_address, 4, &ip_value);
            second_last_value = 0;
            last_value = ip_value;
            while(true)
                {
                    DllMoviDebugParseString(const_cast<char*>("wait 1000"));
                    DllMoviDebugGet(ip_address, 4, &ip_value);

                    if( ip_value == last_value && last_value == second_last_value)
                        break;
                    second_last_value = last_value;
                    last_value = ip_value;
                }

            DllMoviDebugParseString(const_cast<char*>("cont"));

            // asm kernel beginning

            storePreservedRegisters();

            DllMoviDebugGet(i30_address, 4, &i30_value);

            itoa(i30_value, i30ss, 16);

            string breakpoint2("b 0x" + string(i30ss));

            DllMoviDebugParseString(const_cast<char*>(breakpoint2.c_str()));

            string ub("ub " + TARGET_PREFIX + asmLabel);
            DllMoviDebugParseString(const_cast<char*>(ub.c_str()));

            DllMoviDebugParseString(const_cast<char*>("cont"));

            DllMoviDebugGet(ip_address, 4, &ip_value);
            second_last_value = 0;
            last_value = ip_value;
            while(true)
                {
                    DllMoviDebugParseString(const_cast<char*>("wait 1000"));
                    DllMoviDebugGet(ip_address, 4, &ip_value);

                    if( ip_value == last_value && last_value == second_last_value)
                        break;
                    second_last_value = last_value;
                    last_value = ip_value;
                }

            // return adress
            DllMoviDebugParseString(const_cast<char*>("step"));

            verifyPreservedRegisters();
            printInfoPreservedRegisters();

            ub.assign("ub 0x" + string(i30ss));
            DllMoviDebugParseString(const_cast<char*>(ub.c_str()));

            DllMoviDebugGet(ip_address, 4, &ip_value);
            second_last_value = 0;
            last_value = ip_value;
            while(true)
            {
                DllMoviDebugParseString(const_cast<char*>("wait 1000"));
                DllMoviDebugGet(ip_address, 4, &ip_value);
                if( ip_value == last_value && last_value == second_last_value)
                    break;
                second_last_value = last_value;
                last_value = ip_value;
                DllMoviDebugParseString(const_cast<char*>("step"));
            }
                #endif
            unsigned int sym_name, sym_exist;
            string breakpoint("mdbg::breakpoint add -target S0 " + std::string(TARGET_PREFIX) + asmLabel);
            string kernel_label(TARGET_PREFIX + asmLabel);
            sym_exist = fptr->symAddr(dbg, 0, kernel_label.c_str(), &sym_name);

            if(target_core==1) // do this verification only if we have core run on shave0
            {
            if(!sym_exist)
            {
            fptr->tclEvalScript(dbg, const_cast<char*>(breakpoint.c_str()), 0, 0);
            }
            else
            {
            printf("\nnote: symbol for breakpoint not present on Shave0!\n");
            printf("note: please provide the kernel name used in this unittest for testing\n");
            exit(1);
            }
            }
            fptr->tclEvalScript(dbg, "mdbg::run -wait", 0, 0);
            if(target_core==1) // do this verification only if we have core run on shave0
            {
            if(!sym_exist)
            {
            storePreservedRegisters();
            fptr->tclEvalScript(dbg, "mdbg::step out -target S0", 0, 0);
            fptr->tclEvalScript(dbg, "mdbg::breakpoint remove -all", 0, 0);
            verifyPreservedRegisters();
            printInfoPreservedRegisters();
            fptr->tclEvalScript(dbg, "mdbg::contw -target S0", 0, 0);
            }
            else
            {
            printf("note: symbol for breakpoint not present on Shave0!\n");
            exit(1);
            }
            }

            init++;

            isFirstTest = false;

        }
        else
        {
            #ifdef MV_MVDBG_GEN2
            fptr->tclEvalScript(dbg, "mdbg::run -wait", 0, 0);
            init++;
            #endif
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugParseString(const_cast<char*>(LEON_START_CMD));
            DllMoviDebugParseString(const_cast<char*>("runw"));
            #endif
        }
    }
    fptr->tclEvalScript(dbg, "mdbg::wait 100", 0, 0); //allow delay for data to be written properly
    return isFirstTest;
}

void IOProcessing::printInfoPreservedRegisters()
{
    bool fail_IRF_Restoring = false, fail_VRF_Restoring = false;
    string irf, vrf;

    for(int i = 0; i < PRESERVED_IRFS; i++)
        if(IRF_preserved[i] == 0)
            {
                switch(i)
                {
                    case 0 : irf.assign("i19"); break;
                    case 1 : irf.assign("i20"); break;
                    case 2 : irf.assign("i21"); break;
                    case 3 : irf.assign("i22"); break;
                    case 4 : irf.assign("i23"); break;
                    case 5 : irf.assign("i24"); break;
                    case 6 : irf.assign("i25"); break;
                    case 7 : irf.assign("i26"); break;
                    case 8 : irf.assign("i27"); break;
                    case 9 : irf.assign("i28"); break;
                    case 10: irf.assign("i29"); break;
                    case 11: irf.assign("i30"); break;
                    case 12: irf.assign("i31"); break;
                }
                fail_IRF_Restoring = true;
                printf("Register '%s' is not properly restored. ( 0x%08x vs 0x%08x )\n", irf.c_str(), IRF_values[i], IRF_values_at_end[i]);
            }
    if(fail_IRF_Restoring == false)
    {
        printf("Preserved IRF registers are properly restored.\n");
    }

    fail_VRF_Restoring = false;
    for(int v = 0; v < PRESERVED_VRFS; v++)
        if(VRF_preserved[v] == 0)
            {
                switch(v)
                {
                    case 0 : vrf.assign("v24"); break;
                    case 1 : vrf.assign("v25"); break;
                    case 2 : vrf.assign("v26"); break;
                    case 3 : vrf.assign("v27"); break;
                    case 4 : vrf.assign("v28"); break;
                    case 5 : vrf.assign("v29"); break;
                    case 6 : vrf.assign("v30"); break;
                    case 7 : vrf.assign("v31"); break;
                }
                fail_VRF_Restoring = true;
                printf("Register '%s' is not properly restored. ( { 0x%08x, 0x%08x, 0x%08x, 0x%08x } vs { 0x%08x, 0x%08x, 0x%08x, 0x%08x } )\n", vrf.c_str(), VRF_values[v][3],VRF_values[v][2], VRF_values[v][1], VRF_values[v][0], VRF_values_at_end[v][3], VRF_values_at_end[v][2], VRF_values_at_end[v][1], VRF_values_at_end[v][0]);
            }
    if(fail_VRF_Restoring == false)
    {
        printf ("Preserved VRF registers are properly restored.\n");
    }

    if(fail_IRF_Restoring == true || fail_VRF_Restoring == true)
        exit(1);
}

unsigned int IOProcessing::getSymbolAddr(std::string symbolName)
{
    unsigned int symAddr = 0;
    size_t symPos = 0;
    std::string sym = symbolName;
    std::string line;

    std::ifstream f(mapFilePath.c_str(), std::ios::in);

    if (!f.is_open())
    {
        printf("Cannot open map file: %s\n", mapFilePath.c_str());
        exit(-1);
    }

    // Search for symbol line-by-line
    while (!f.eof())
    {
        getline(f, line);

        if ((symPos = line.find(sym)) != string::npos)
        {
            size_t addrPos = 0;

            // Search for pattern starting with 0x
            if ((addrPos = line.find("0x")) != string::npos)
            {
                // Jump first 8 charachters after 0x
                std::string addr = line.substr(addrPos + 8 + 2, 8);

                // Convert string to int
                sscanf(addr.c_str(), "%x", &symAddr);

                //rintf("%x\n", symAddr);

                break;
            }
            else
            {
                printf("Cannot find symbol address on line:\n '%s'\n", line.c_str());
                f.close();
                exit(-1);
            }
        }
    }

    if (symPos == string::npos)
    {
        printf("Cannot find symbol '%s' in '%s'\n", sym.c_str(), mapFilePath.c_str());
        f.close();
        exit(-1);
    }

    f.close();

    return symAddr;
}


bool IOProcessing::Run(TargetCore target)
{
    #ifndef MODULES_UNIT_TEST
    setData("target", SHAVE_ALL);
    setData("runOnce", 1);
    #endif
    if (target != SHAVE0) target_core=0; // added this verification for breakpoint use on shave0
    else target_core=1;
    return runCheckingRegisters(target);
}

bool IOProcessing::Run(TargetCore target, std::string entryPoint)
{
    static int firstRun = 1;
    #ifdef MV_MVDBG_GEN1
    int lowPC, highPC;
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int lowPC;
    #endif

    if (target != SHAVE0) target_core=0; // added this verification for breakpoint use on shave0
    else target_core=1;

    // Treat simple kernels as they used to be treated
    if (entryPoint.empty() && target != FINISH)
    {
        // Tell the Leon main loop to run Shaves
        Run(target);
    }
    else
    {
        printf("%d %s\n", target, entryPoint.c_str());

        if (target == FINISH)
        {
            printf("FINISH\n");
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugParseString(const_cast<char*>("unbreak breakpoint"));
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->tclEvalScript(dbg, "mdbg::breakpoint remove breakpoint", 0, 0);
            #endif
            setData("target", target);
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugParseString(const_cast<char*>(LEON_TARGET));
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->tclEvalScript(dbg, "mdbg::t los", 0, 0);
            #endif
        }
        else if ((target == LEON_OS) || (target == LEON_RT))
        {
            #ifndef MODULES_UNIT_TEST
            setData("target", LEON_OS);
            #endif
            #ifdef MV_MVDBG_GEN1
            DLLMoviDebugGetFunctionIP(target, entryPoint.c_str(), &lowPC, &highPC);
            printf("Running Leon(%d): %x %x %s\n", target, lowPC, highPC, entryPoint.c_str());
            DllMoviDebugParseString(const_cast<char*>("break breakpoint"));
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->symAddr(dbg, 0, entryPoint.c_str(), &lowPC);
            printf("Running Leon(%d): %x %s\n", target, lowPC, entryPoint.c_str());
	    #ifndef MODULES_UNIT_TEST
            fptr->tclEvalScript(dbg, "mdbg::breakpoint add breakpoint", 0, 0);
	    #endif
            #endif
            if(target==LEON_RT){
            setData("lrt_leonEntryPoint", lowPC);
}else{
setData("leonEntryPoint", lowPC);
}
        }
        else
        {
            // Tell the Leon main loop to run Shaves
            #ifndef MODULES_UNIT_TEST
            setData("target", SHAVE_ALL);
            #endif
            #ifdef MV_MVDBG_GEN1
            lowPC = getSymbolAddr((TARGET_PREFIX + entryPoint).c_str());
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->symAddr(dbg, 0, (TARGET_PREFIX + getShaveName(target) + "_"+ entryPoint).c_str(), &lowPC);
            #endif
            #ifdef MODULES_UNIT_TEST
            setData("lrt_shavesEntryPoint", lowPC);
            #else
            setData("shavesEntryPoint", lowPC);
            #endif
            printf("Running Shave(%d): %x %s\n", target, lowPC, entryPoint.c_str());
        }

        if (firstRun)
        {
          runCheckingRegisters(target);

          firstRun = 0;
        }
        else
        {
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugParseString(const_cast<char*>("continueandwait"));
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->tclEvalScript(dbg, "mdbg::contw", 0, 0);
	    #ifndef MODULES_UNIT_TEST
	    fptr->tclEvalScript(dbg, "mdbg::breakpoint remove -all", 0, 0);
	    #endif
            #endif

        }
    }
    return firstRun;
}

int IOProcessing::connectionInit(int targets, char *moviDebugServerIP, char *moviDebugServerPort)
{
#ifdef MYRIAD2
    targets |= TARGET_MYRIAD2;
#endif

    #ifdef MV_MVDBG_GEN2
    if((runWithSimulator && (defaultPort == true) ))
    {
    int simPort = 30000; //default simulator port
    itoa(simPort, moviDebugServerPort, 10);
    }

    MoviDebugInitOption optionList[] = {
            {MOVIDEBUG_OPT_TCP_HOST, moviDebugServerIP},
            {MOVIDEBUG_OPT_TCP_PORT, moviDebugServerPort},
            {MOVIDEBUG_OPT_CHIP_VERSION, const_cast<char*>(chipVersion)},
            {MOVIDEBUG_OPT_TCF_LISTEN_PORT, "0"},
            {MOVIDEBUG_OPT_VERBOSE, verbose ? "true" : "false"}
        };

    MoviDebugDllInitOptions options = {
            0,
            0,
            COUNT_OF(optionList), // option count
            optionList, // options
            &callbacks  // callbacks
        };
    dbg = moviDebug2_DllInit(&options, &fptr);
    if((fptr == NULL) || (dbg == NULL)){
    printf("\nUnable to connect to debug server. Make sure a server is started on the specified port and is not busy\n");
    exit(1);
    }
    #endif
    #ifdef MV_MVDBG_GEN1
    if(runWithSimulator && (defaultPort == false) )
    {
    int simPort = atoi(moviDebugServerPort);
    simPort++;
    itoa(simPort, moviDebugServerPort, 10);
    }
    if(DllMoviDebugInitLibrary(targets, const_cast<char*>(projectPath.c_str()),
            false, 3000, moviDebugServerIP, moviDebugServerPort, true) == false)
    {
        printf("Initialization error\n");
        exit(1);
    }
    #endif
    return 0;
}


int IOProcessing::connectionInit()
{
#ifdef MYRIAD2
    debugInterface |= TARGET_MYRIAD2;
#endif
#ifdef MV_MVDBG_GEN1
    if(DllMoviDebugInitLibrary(debugInterface, const_cast<char*>(projectPath.c_str()),
            false, 3000, const_cast<char*>("127.0.0.1"), const_cast<char*>("30001"), true) == false)
    {
        printf("Initialization error\n");
        exit(1);
    }
#endif
    return 0;
}

void IOProcessing::LoadElf(std::string elfFilePath)
{
#ifdef MV_MVDBG_GEN2
if(init>0)
{
            ifstream f(elfFilePath.c_str());
        //check if the elf file exists
        if (!f.good()) {
            f.close();
            fprintf(stderr, "Elf file: \"%s\" doesn't exists or can not be found.\n", elfFilePath.c_str());
            exit(1);
        }

        string cmd("mdbg::load " + elfFilePath);

        fptr->tclEvalScript(dbg, "mdbg::breset", 0, 0);
        elfLoaded = fptr->loadElf(dbg, elfFilePath.c_str() , "LOS");
        if (elfLoaded == 0) elfLoaded = 1;
        else elfLoaded = 0;

        //DllMoviDebugParseString(const_cast<char*>("breset"));
        //DllMoviDebugParseString(const_cast<char*>("ddrinit"));
        //elfLoaded = (DllMoviDebugParseString(const_cast<char*>(cmd.c_str())) == 0);

}
else{
    if(connectionInit(debugInterface, srvIP, srvPort) == 0)
    {
        ifstream f(elfFilePath.c_str());
        //check if the elf file exists
        if (!f.good()) {
            f.close();
            fprintf(stderr, "Elf file: \"%s\" doesn't exists or can not be found.\n", elfFilePath.c_str());
            exit(1);
        }


        //DllMoviDebugParseString(const_cast<char*>("breset"));
        fptr->tclEvalScript(dbg, "mdbg::breset", 0, 0);

        //DllMoviDebugParseString(const_cast<char*>("ddrinit"));

        string cmd("mdbg::load " + elfFilePath);

        //elfLoaded = (DllMoviDebugParseString(const_cast<char*>(cmd.c_str())) == 0);

        elfLoaded = fptr->loadElf(dbg, elfFilePath.c_str() , "LOS");
        if (elfLoaded == 0) elfLoaded = 1;
        else elfLoaded = 0;
        fptr->tclEvalScript(dbg,
                  "if {[info commands ::mdbg::targetid] != {} \n"
                  "&&  [info commands ::mdbg::TARGET_ID] != {}} {\n"
                  "::mdbg::alias ::mdbg::TARGET_ID ::mdbg::targetid\n"
                  "}", 0, 0);
    }
    else {
        //debug server not available
        fprintf(stderr, "Unable to connect to debug server. Make sure moviDebugServer is on and not busy.");
        exit(1);
    }
}
#endif
#ifdef MV_MVDBG_GEN1
    if(connectionInit(debugInterface, srvIP, srvPort) == 0)
    {
        ifstream f(elfFilePath.c_str());
        //check if the elf file exists
        if (!f.good()) {
            f.close();
            fprintf(stderr, "Elf file: \"%s\" doesn't exists or can not be found.\n", elfFilePath.c_str());
            exit(1);
        }


        DllMoviDebugParseString(const_cast<char*>("breset"));
        DllMoviDebugParseString(const_cast<char*>("ddrinit"));

        string cmd("load " + elfFilePath);

        elfLoaded = (DllMoviDebugParseString(const_cast<char*>(cmd.c_str())) == 0);
    }
    else {
        //debug server not available
        fprintf(stderr, "Unable to connect to debug server. Make sure moviDebugServer is on and not busy.");
        exit(1);
    }
#endif
}

string IOProcessing::getTargetName(TargetCore target)
{
    char targetNumber[3];
    sprintf(targetNumber, "%d", target);

    //if shave selected
    if(target < MAX_SHAVE_NUMBER)
    {
        return string("S") + targetNumber;
    }
    else if ((target == LEON_OS) || (target == LEON))
    {
#ifdef MYRIAD2
        return "LOS";
#else
        return "L";
#endif
    }
    else if (target == LEON_RT)
    {
        return "LRT";
    }
    //return empty string if target unknown
    return "";
}

void IOProcessing::setData(std::string symbol, unsigned int value)
{
    if(elfLoaded)
    {
        #ifdef MV_MVDBG_GEN1
        unsigned int symAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
        DllMoviDebugSet(symAddr, value, sizeof(unsigned int));
        #endif
        #ifdef MV_MVDBG_GEN2
        unsigned int symAddr;
        fptr->symAddr(dbg, 0, symbol.c_str(), &symAddr);
        fptr->write32(dbg, symAddr, value);
        #endif
    }

}

void IOProcessing::setData(unsigned int addr, unsigned char* data, unsigned int size)
{
    unsigned int currentAddr = addr;
#ifdef MV_MVDBG_GEN1
    unsigned int i;
    int index = 0;
    unsigned int value = 0;
    // TODO: try using only burst regardless of transfer size
    if (size > 1 * 1024 && (size % 4 == 0))
    {
        DllMoviDebugSetBurst(currentAddr, (unsigned*)data, (unsigned)size/4);
    }
    else
    {
        for(i = 0; i < size; i++)
        {
            value |= ((unsigned int)*data) << (index * 8);
            data++;

            index++;
            if((index > 3) || (i == size - 1))
            {
                //save value
                DllMoviDebugSet(currentAddr, value, sizeof(unsigned int));
                DllMoviDebugSet(currentAddr, value, sizeof(unsigned int));

                index = 0;
                value = 0;
                currentAddr += 4;
            }
        }
    }
#endif
#ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, currentAddr, (unsigned*)data, (unsigned)size);

#endif

}


void IOProcessing::setData(unsigned int addr, float* data, unsigned int size)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) data, size);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned*)data, size*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, float4 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, int4 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, uint4 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, short8 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, ushort8 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, char16 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, uchar16 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, half8 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data, 4*4);
    #endif
}
/*
void IOProcessing::setData(std::string symbol, float4x4 data)
{
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    unsigned int offset = 0;

    setData(symbAddr + offset, (float*)&(data.rows[0]), 16);
}
*/

void IOProcessing::setData(unsigned int addr, float4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data.rows, 16);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data.rows, 16*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, int4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data.rows, 16);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data.rows, 16*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, uint4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data.rows, 16);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data.rows, 16*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, long4x4 data)
{
    unsigned int value;
    unsigned int currentAddr = addr;

    for ( int i = 0; i < 4; i++)
        for ( int j = 0; j < 4; j++)
        {
            value = *((unsigned int*)&data.rows[i][j]);
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugSet(currentAddr, value, sizeof(unsigned int));
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->writeBlock(dbg, currentAddr, (unsigned int*) &value, sizeof(unsigned int));
            #endif
            currentAddr += 4;
        }
}

void IOProcessing::setData(unsigned int addr, ulong4x4 data)
{
    unsigned int value;
    unsigned int currentAddr = addr;

    for ( int i = 0; i < 4; i++)
        for ( int j = 0; j < 4; j++)
        {
            value = *((unsigned int*)&data.rows[i][j]);
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugSet(currentAddr, value, sizeof(unsigned int));
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->writeBlock(dbg, currentAddr, (unsigned int*) &value, sizeof(unsigned int));
            #endif
            currentAddr += 4;
        }
}

void IOProcessing::setData(unsigned int addr, short8x8 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data.rows, 8);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data.rows, 8*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, ushort8x8 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data.rows, 8);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data.rows, 8*4);
    #endif
}

void IOProcessing::setData(unsigned int addr, half8x8 data)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetBurst(addr, (unsigned int*) &data.rows, 8);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->writeBlock(dbg, addr, (unsigned int*) &data.rows, 8*4);
    #endif
}

void IOProcessing::setData(string symbol, unsigned char* data, unsigned int size)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif

    setData(symbAddr, data, size);
}

void IOProcessing::setDataAndLoad(string symbol, unsigned char* data, unsigned int size)
{
        UNUSED(symbol); //to be implemented for movidebug2
        UNUSED(data);
        UNUSED(size);
        #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));

    if (size < 1 * 1024)
    {
        setData(symbAddr, data, size);
    }
    else
    {
        DllMoviDebugParseString(const_cast<char*>(("l " + symbol + " " + symbol + ".pc").c_str()));
    }
        #endif
}


void IOProcessing::setData(string symbol, float* data,
        unsigned int size)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif

    setData(symbAddr, data, size);
}

void IOProcessing::setData(string symbol, float4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, int4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, uint4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, short8 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, ushort8 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, char16 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, uchar16 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, half8 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, float4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, int4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, uint4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, long4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, ulong4x4 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, short8x8 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, ushort8x8 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(string symbol, half8x8 data)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    setData(symbAddr, data);
}

void IOProcessing::setData(std::string symbol,
        unsigned char** data, unsigned int lineDataSize, unsigned int lineWidth,
        unsigned int noOfLines)
{

    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbol.c_str(), &symbAddr);
    #endif
    unsigned int offset = 0;
    unsigned int dataOffset = 0;

    for(unsigned int i = 0; i < noOfLines; i++)
    {
        setData(symbAddr + offset, data[i], lineDataSize);
        offset += lineWidth;
        dataOffset += lineWidth;
    }
}


void IOProcessing::setData(std::string symbol, float* data, unsigned int lineDataSize,
        unsigned int lineWidth, unsigned int noOfLines)
{
       UNUSED(symbol); //to be used for further improvements on unittest
       UNUSED(data);
       UNUSED(lineDataSize);
       UNUSED(lineWidth);
       UNUSED(noOfLines);

        #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbol.c_str()));
    unsigned int offset = 0;
    unsigned int dataOffset = 0;

    for(unsigned int i = 0; i < noOfLines; i++)
    {
        setData(symbAddr + offset, data + dataOffset, lineDataSize);
        offset += lineWidth;
        dataOffset += lineWidth;
    }
        #endif
}

void IOProcessing::connectionClose()
{
        #ifdef MV_MVDBG_GEN1
    DllMoviDebugCloseLibrary();
        #endif
}

unsigned int IOProcessing::getValueFromAddr(unsigned int address, TargetCore target)
{
    unsigned int value = 0;
        UNUSED(target); //to be used for further improvements on the unittest
        UNUSED(address);
        #ifdef MV_MVDBG_GEN1
    DllMoviDebugSetCurrentTarget(target, 1);

    DllMoviDebugGet(address, 4, &value);
        #endif
    return value;
}

unsigned int IOProcessing::GetIntOutput(string varName)
{
    unsigned int varAddr;
    #ifdef MV_MVDBG_GEN2
    fptr->symAddr(dbg, 0, const_cast<char*>(varName.c_str()), &varAddr);
    #endif
    #ifdef MV_MVDBG_GEN1
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif

    unsigned int retValue;
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGet(varAddr, sizeof(unsigned int), &retValue);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->read32(dbg, varAddr, &retValue);
    #endif
    return retValue;
}

void IOProcessing::SaveToFile(unsigned int addr, unsigned int size,
        std::string fileName)
{
    char cmd[MAX_CMD_LINE];

    sprintf(cmd, "save %d %d %s", addr, size, fileName.c_str());
        #ifdef MV_MVDBG_GEN1
    DllMoviDebugParseString(cmd);
        #endif
}


void IOProcessing::SaveToFile(std::string symbol, unsigned int size,
        std::string fileName)
{
    char cmd[MAX_CMD_LINE];

    sprintf(cmd, "save %s %d %s", symbol.c_str(), size, fileName.c_str());
        #ifdef MV_MVDBG_GEN1
    DllMoviDebugParseString(cmd);
        #endif
}

std::string IOProcessing::getRegisterName(unsigned int paramPosition,
        RegisterType type)
{
    char name[4];

    switch(type)
    {
    case IRF:
        sprintf(name, "%c%d", 'I', 18 - paramPosition);
        break;
    case SRF:
        sprintf(name, "%c%d", 'S', 23 - paramPosition);
        break;
    case VRF:
        sprintf(name, "%c%d", 'V', 23 - paramPosition);
        break;
    }

    return string(name);
}

void IOProcessing::LoadFromFile(unsigned int addr, std::string fileName)
{
    char cmd[MAX_CMD_LINE];

    sprintf(cmd, "load %d %s", addr, fileName.c_str());
        #ifdef MV_MVDBG_GEN1
    DllMoviDebugParseString(cmd);
        #endif
}


void IOProcessing::LoadFromFile(std::string symbol, std::string fileName)
{
    char cmd[MAX_CMD_LINE];

    sprintf(cmd, "load %s %s", symbol.c_str(), fileName.c_str());
        #ifdef MV_MVDBG_GEN1
    DllMoviDebugParseString(cmd);
        #endif
}

std::string IOProcessing::getShaveName(TargetCore target)
{
    char shaveName[4];
    sprintf(shaveName, "%d", target);
    return string(shaveName);
}

unsigned int IOProcessing::getSymAddr(std::string varName)
{
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    return varAddr;
}

void IOProcessing::GetOutput(std::string varName, unsigned int size, unsigned char* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif

    GetOutput(varAddr, size, output);
}


void IOProcessing::GetOutput(unsigned int addr, unsigned int size, unsigned char* output)
{
    unsigned int value;
    unsigned int idx = 0;
    unsigned int currentAddr = addr ;
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGet(addr, 4, &value);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->read32(dbg, currentAddr, &value);
    #endif
    for(unsigned int i = 0; i < size; i++)
    {
        if(idx > 3)
        {
            currentAddr += 4;
            #ifdef MV_MVDBG_GEN1
            DllMoviDebugGet(currentAddr, 4, &value);
            #endif
            #ifdef MV_MVDBG_GEN2
            fptr->read32(dbg, currentAddr, &value);
            #endif
            idx = 0;
        }
        output[i] = (value >> (idx * 8)) & 0xFF;
        idx++;
    }
}

void IOProcessing::GetOutput(std::string varName, unsigned int lineDataSize,
        unsigned int lineWidth, unsigned int lineNo, unsigned char** output)
{
    unsigned int offset = 0;
    unsigned int varAddr;

    #ifdef MV_MVDBG_GEN2
    fptr->symAddr(dbg, 0, const_cast<char*>(varName.c_str()), &varAddr);
    #endif
    #ifdef MV_MVDBG_GEN1
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif

    for(unsigned int i = 0; i < lineNo; i++)
    {
        GetOutput(varAddr + offset, lineDataSize, *output);

        offset += lineWidth;
        output++;
    }
}


void IOProcessing::GetOutput(std::string varName, float4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);

}


void IOProcessing::GetOutput(unsigned int addr,  float4* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, int4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr, int4* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, uint4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr, uint4* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, short8* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}

void IOProcessing::GetOutput(unsigned int addr, short8* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, ushort8* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}

void IOProcessing::GetOutput(unsigned int addr, ushort8* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, char16* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr, char16* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, uchar16* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr, uchar16* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, half8* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr, half8* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*) output, 4);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*) output, 4*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, float4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  float4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*)&output->rows, 16);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*)&output->rows, 16*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, int4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  int4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*)&output->rows, 16);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*)&output->rows, 16*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, uint4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  uint4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*)&output->rows, 16);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*)&output->rows, 16*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, long4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  long4x4* output)
{
    unsigned int value;

    unsigned int currentAddr = addr;
    for(unsigned int i = 0; i < 4; i++)
        for(unsigned int j = 0; j < 4; j++)
        {
                        #ifdef MV_MVDBG_GEN1
            DllMoviDebugGet(currentAddr, 4, &value);
                        #endif
            currentAddr += 4;
            (*output).rows[i][j] = *((unsigned int*)&value);

        }
}

void IOProcessing::GetOutput(std::string varName, ulong4x4* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  ulong4x4* output)
{
    unsigned int value;

    unsigned int currentAddr = addr;
    for(unsigned int i = 0; i < 4; i++)
        for(unsigned int j = 0; j < 4; j++)
        {
                        #ifdef MV_MVDBG_GEN1
            DllMoviDebugGet(currentAddr, 4, &value);
                        #endif
            currentAddr += 4;
            (*output).rows[i][j] = *((unsigned int*)&value);

        }
}

void IOProcessing::GetOutput(std::string varName, short8x8* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  short8x8* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*)&output->rows, 8);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*)&output->rows, 8*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, ushort8x8* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  ushort8x8* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*)&output->rows, 8);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*)&output->rows, 8*4);
    #endif
}

void IOProcessing::GetOutput(std::string varName, half8x8* output)
{
    #ifdef MV_MVDBG_GEN1
    unsigned int varAddr;
    varAddr = DllMoviDebugGetAddress(const_cast<char*>(varName.c_str()));
    #endif
    #ifdef MV_MVDBG_GEN2
    unsigned int varAddr;
    fptr->symAddr(dbg, 0, varName.c_str(), &varAddr);
    #endif
    GetOutput(varAddr, output);
}


void IOProcessing::GetOutput(unsigned int addr,  half8x8* output)
{
    #ifdef MV_MVDBG_GEN1
    DllMoviDebugGetBurst(addr, (unsigned int*)&output->rows, 8);
    #endif
    #ifdef MV_MVDBG_GEN2
    fptr->readBlock(dbg, addr, (unsigned int*)&output->rows, 8*4);
    #endif
}


unsigned int IOProcessing::GetValue(std::string symbName)
{
    #ifdef MV_MVDBG_GEN2
    unsigned int symbAddr;
    fptr->symAddr(dbg, 0, symbName.c_str(), &symbAddr);
    unsigned int value;
    fptr->read32(dbg, symbAddr, &value);
    #endif
    #ifdef MV_MVDBG_GEN1
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(symbName.c_str()));
    unsigned int value;
    DllMoviDebugGet(symbAddr, 4, &value);
    #endif

    return value;
}

void IOProcessing::selectTarget(TargetCore target)
{
    char cmd[MAX_CMD_LINE];
    #ifdef MV_MVDBG_GEN1
    sprintf(cmd, "target %s", getTargetName(target).c_str());
    DllMoviDebugParseString(cmd);
    #endif
    #ifdef MV_MVDBG_GEN2
    sprintf(cmd, "mdbg::target %s", getTargetName(target).c_str());
    fptr->tclEvalScript(dbg, cmd, 0, 0);
    #endif
}

bool IOProcessing::InteractiveDebug(void)
{

    bool debugMode = true;
        #ifdef MV_MVDBG_GEN1
    bool finished = false; //to be moved outside the ifdef statement
    string cmd;
    char targetName[4];
    DllMoviDebugGetCurrentTargetName(targetName);
    string prompt("> ");

    const string exit_cmd("exit");

    do {
        cout << targetName << prompt;
        getline(cin, cmd);

        if(cmd.compare(exit_cmd) == 0)
        {
            Run(LEON);
            finished = true;
        }
        else if((cmd.compare("quit") == 0) ||
                 (cmd.compare("q") == 0))
        {
            Run(LEON);
            finished = true;
            debugMode = false;
        }
        else {
            DllMoviDebugParseString(const_cast<char*>(cmd.c_str()));
            DllMoviDebugGetCurrentTargetName(targetName);
        }
    } while(!finished);
    // returns false if a request to exit debug mode was made
    // (by quit or q command) and true otherwise
        #endif
    return debugMode;
}

