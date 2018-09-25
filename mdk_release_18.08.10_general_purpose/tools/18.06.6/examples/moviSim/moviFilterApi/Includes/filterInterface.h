//--------------------------------------------
//File Name   : filterInterface.h
//Description : Api to dynamically load of Warp or Stereo filters from moviSim project
//Author      : Adrian Lazar(adrian.lazar@intel.com)
//Date        : 17.11.2017
//--------------------------------------------
#ifndef __FILTER_INTERFACE__
#define __FILTER_INTERFACE__

#include <stdint.h>
#include <cstddef>

class FilterInterface;
/**
* This function is exported by the filter library and creates the object for the FilterInterface
*
* Pseudocode call sequence on client side;
* Windows and Linux compatible;
* For dynamic loading of warp, stereo, etc. libs(Windows like):
*
*   HMODULE library = LoadLibrary("filterName");
*
*   createObjectPtr createFilterObject = (createObjectPtr)GetProcAddress(library, "createFilterObject");
*
*   FilterInterface* warpFilter = createFilterObject();
*
*   //call functions from FilterInterface class; see filterInterface.h; minimal usage for Warp:
*
*   auto statusConfig = warpFilter->configure(warpConfig);
*   auto statusRun = warpFilter->start();
*
*   FreeLibrary(library);
**/
typedef FilterInterface* (*createFilterObjectPtr)(const char*);

enum FilterStatus
{
    NO_ERRORS = 0,                      // set configuration or/and execution no error status
    NO_CONFIGURATION_SET = 1,          // configure was not called
    INCOMPLETE_CONFIGURATION = 2,      // configuration have no minimal configuration set status
    DIRTY_CONFIGUARTION = 3,            // user tries to write configuration before calling reset() after a run
    NOT_IMPLEMENTED = -1
};

enum FilterStartMode
{
    DEFAULT = 0,    // default start mode for all filters
    NO_WRAP = 1     // for Stereo NOWRAP start mode
};

/**
* Interface for filter processing
**/
class FilterInterface
{
public:
    // MODE 1: control via configuration structure using filterConfiguration
    /*
        sets the filter configuration;
        separate structures per each filter(stereo, warp);
        see filtersConfiguration.h;
    */
    virtual FilterStatus configure(void* filterConfiguration) = 0;
    /*
        starts the processing after the configuration is set;
        different modes can be set for start depending on filter;
    */
    virtual FilterStatus start(FilterStartMode startMode = DEFAULT) = 0;

    // MODE 2: control via direct registers read and write using module register interface
    /*
        writes module register specified by 'address' using a 'data' pointer of given 'size'
    */
    virtual FilterStatus apbWriteRegister(size_t address, uintptr_t data, size_t size = 0) = 0;
    /*
        reads module register specified by 'address' value returned in 'data' pointer of a 'size'
    */
    virtual FilterStatus apbReadRegister(size_t address, uintptr_t data, size_t size = 0) = 0;

    /*
        reset the filter configuration to the default values
    */
    virtual void reset() = 0;
};

#endif
