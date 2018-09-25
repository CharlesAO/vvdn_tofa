# Profiler

## Introduction

The Movidius Profiling tool (moviProf) is an application that is used to decode profiling trace data files resulting
from profiling functions instrumented with the functionProfiler MDK component. It is integrated with Eclipse and the
MDK build system. It can be used separately from command line as well, but the user should not need to do that, as the
entire profiling process is ~~unpredictable and fragile~~**automated**.

An application can be profiled by setting the variable PROFILE_INFO either as a global environment variable, or as a make variable.

Example of how to execute function profiling from command line:
``` bash
make run PROFILE_INFO=function
```

For more detailed information regarding the profiler, please see the documentation provided with the moviTools package, usually is the
``` bash
xdg-open ${MV_TOOLS_DIR}/${MV_TOOLS_VERSION}/doc/moviProf.pdf
```

## Overview

The _profiler_ is used to profile the code to gather useful information. This is a
**mandatory** tool when you need to analyze the performance. With great power comes great responsibility!

>"The best preparation for tomorrow is doing your best today.
>
>_H. Jackson Brown, Jr._"

---

## Prerequisites

1. Install [the tools package][1]
    - Movidius Eclipse
    - MoviDebug2
    - MoviDebugServer or MoviSim
2. Install MDK (or at least the profiler component)
3. Make sure the profiled elf file have DWARF information regardless the optimization level. By default this is enabled. If not check for the -g compiler argument
4. Check to have the following tools installed: make, coreutils (realpath, readelf, nm, grep, cut, awk), gprof, perf, and cygwin/cygpath in case you still want to profile on
   Windows

## Features

MoviProf can handle trace files for sample, function, and trace profilers.

For the function profiler, moviProf is computing the runtime clock cycles of instrumented
functions and other relevant information, outputting the result in csv files and in
LTTNG format (to be used by TraceCompas, which is integrated within moviEclipse).

The Sample profiler is less intrusive but the result is not always 100% accurate.
For the sample profiler, moviProf takes as input a file containing the gathered samples
and outputs gprof style results, which can be viewed either from moviEclipse or from
other gprof-compliant viewers. Note that for sample profiling not all cores can be
profiled at the same time since one is used to run the sampling interrupt.

Trace profiler can be used in conjunction with any other profile type to send events programatically. There are
also a set of events that are already defined in MDK that are sent to the profile data stream when the trace profiling
is activated.

## Function profiler

The Function Profiler component is responsible for profiling the instrumented functions,
applicable to both RTEMS and/or Bare Metal. It uses compiler options to insert custom hooks
for function entry and exit, and RTEMS extensions for calling a thread switch hook each
time a thread switch occurs.

By storing the exact timestamp (measured in clock cycles) at which a function was entered/exited
and the exact timestamp at which a thread switch occur, we can create the call history of an
application and profiling information such as inclusive, exclusive and total time spent in a function.

Inclusive time refers to the time spent in that function including the calls to other functions
from within the function's body. However, this time does not include the time spent in other threads
(in the case when the Leon is running RTEMS).

Exclusive time refers to the time spent in that function only, excluding the calls to other
functions from within. Also, time spent in other threads is not included, just as in the case
of inclusive time.

The hooks populate the profiling data (such as function address, context and timestamp) into
a circular buffer (placed in non-cached DDR address space). When the buffer gets full, the
data in it will be overwritten with new one. This means, the information in the buffer will
always be the most recent one.

It is possible to specify an existing buffer, instead of using a new one. In this case it is the
user responsibility to flush cache, initialize the buffers, etc.

The time of profiling is dependent on the size of the global buffer and the number of functions
that are instrumented. By default all global functions, static functions, inline functions and
ISRs are instrumented. RTEMS internal functions (in the case of RTEMS running on LEON) and library
functions (for both Leon and Shave) are not instrumented.

The user can choose to save the raw binary file from application and run the profiler parser
(in order to generate the .csv files) from command line, or use the make targets by
typing `make run PROFILE_INFO=function`. Depending on the processor for which profiling
is needed, _FP\_CORE\_PROFILE_ can be used to set core specific options. This will be explained
later in this document.

### Integration

It is the responsibility of the user to make sure that the timer block that is used for profiling is
enabled and clocked and is not stopped for the entire duration of the profiling execution.
Also, the application must ensure that the bus infrastructure accessing the used Timers block
is always powered and clocked.

If RTEMS is used, then the maximum number of user extensions must be increased by one. Increment the value of **CONFIGURE\_MAXIMUM\_USER\_EXTENSIONS** define, or define one now equal to 1.
`#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    1`

All the profiling information gathering is based on sampling the free running counter each
time a statistic is gathered. The function profiling is using the free running counter from
LeonOS Timers block by default. This can be specified otherwise, by setting the
**FP\_&lt;CORE&gt;\_TIMER** variable to the base address of the LeonRT Timers block.
The **&lt;CORE&gt;** can be one of the LOS, LRT, or SHV.

If the profiling buffers are saved in the DDR (default configuration),
then the application must make sure that the DDR is powered and clocked.
If the application needs to deactivate the DDR or something similar, then profile buffers
should be defined in CMX, using the **FP\_&lt;CORE&gt;\_DATA\_SECTION** variable.
In the future, we will provide the user the means to disable/enable profiling during
Timer block / DDR power/clock outage directly from the application code, using the profiling API,
but this API is not yet available in the current release of the function profiling component.

### Configuration

The profiling can be configured using make variables passed in command line, defined in project's Makefile,
or by using the Eclipse profile plugin provided with moviEclipse.
Except PROFILE_INFO all the other parameters are optionally having predefined defaults.


Name|Default Value|Description
--- | --- | ---
PROFILE_INFO|function|This flag instruct the buildsystem to use the function profiling. This is the only mandatory variable
FP\_CORE\_PROFILE | los lrt shave | The cores where the profiling will be activated
FP\_DURATION\_MS|60000|After 60 seconds, the program will be stopped if did not exited successfully by then
FP\_LOS\_BUFFER_SIZE FP\_LRT\_BUFFER_SIZE|4194304|The buffer for LOS. 4Mb is normally enough for any application, you could adjust this value by you need. More memory reduce the risc for data loss in circular buffer, but more time is needed to transfer the data. Also, for complex applications you might not have enough memory in DDR.
FP\_SHV\_BUFFER_SIZE|1048576|Default is 1Mb for shaves. Caution: for shave you have this value multiplied by number of shaves (12 for Myriad 2)
FP\_LOS\_DATA\_SECTION FP\_LRT\_DATA\_SECTION FP\_SHV\_DATA\_SECTION|.ddr\_direct.bss|If BSS is used, then the buffer will automatically be initialized before main. If you need to instrument functions before main, then change to data section.
FP\_LOS\_BUFFER\_NAME FP\_LRT\_BUFFER\_NAME FP\_SHV\_BUFFER\_NAME|profileBuffer|Buffer name. Change this if you have name conflicts in your code. Otherwise is not useful to change, only to know the buffer name for accessing it from within the application
FP\_LOS\_TIMER FP\_LRT\_TIMER FP\_SHV\_TIMER|TIM0\_BASE\_ADR|Free running counter used by hook function. Make sure is initialized in code. The same counter is recommended to be used for all cores to synchronize the logs. **CORE** is one of LOS, LRT, or SHV
FP\_OUTPUT\_DIRECTORY|output/profile|This is used in order to differentiate the normal built elf and the profiled one since performance penalty can occur and not to be mistaken for a production elf
FP\_INSTRUMENT\_ONLY|*&lt;unset&gt;*|This is a list of object files. If provided, only the provided object files will have instrumented functions. (not fully tested, works for shaves for now)
FP\_COLLECT\_STALLS|*&lt;unset&gt;*|Set this to true to collect stalls and a more accurate execution instruction count. Requires more memory
FP\_32BIT\_TIMESTAMP\_OPT\_MODE||This option is to use 32 bit delta instead of the full 64 bit timestamp
FP\_SHV\_CTORS\_CALLED|*&lt;unset&gt;*|Check this flag if you are sure the global static constructors are beeing called from crtinit before main. Will optimize the speed of the profiler
FP\_SHV\_INSTRUMENT\_FLAG|*&lt;unset&gt;*|Used in function profiling when the shaves are instrumented by different means (pgo, etc)

><small>Please note that there is a single buffer name specified for all Shaves, but in reality the build system will create one variable with the specified name for each Shave (with a specific prefix). All the Shave buffers will have the size of FP\_SHV\_BUFFER\_SIZE, so keep in mind that the total DDR used for Shave buffers is FP\_SHV\_BUFFER\_SIZE multiplied by the number of Shaves used by the application. It is not possible to configure the individual buffer sizes for the Shaves at this time.
>
>There may be other variables inside the profiler.mk that are not described in this table, but they may be subject to change without notice, so one should not rely on any other variable outside the ones described in the above table for fine tuning the mechanics of the function profiler component. The behavior of those other variables is not guaranteed to stay the same in the future.</small>


### Running the application

For running the application, you can use the `make run PROFILE_INFO=function` or `make debug PROFILE_INFO=function` target.
If you want to run with 32 bit timestamp ,you can use the `make run PROFILE_INFO=function FP_32BIT_TIMESTAMP_OPT_MODE=true` or `make debug PROFILE_INFO=function FP_32BIT_TIMESTAMP_OPT_MODE=true`

With this, make launches the moviDebug2 with the application's default
startup script as input. When the application exists or the timeout expires,
the profile buffers are saved to output/profile/*.bin files.
After that, moviProf is invoked to decode the data from all buffers, creating the csv and lttng files.
In moviEclipse, the TraceCompass view is automatically updated with call-stack view and different statistics,
once the user double-clicks the trace that becomes available within the project view.


### Manually parsing the raw data (optional)

The application moviProf can be used also to parse a previously saved raw profiling data. To do this, open a terminal and launch moviProf with specific command line parameters.

``` shell
Usage: profiler [OPTION]... [BIN_FILE]
Parse raw data to csv output file(s)
  --verbose         more verbose
  --los             LOS only profiling and tracing
  --lrt             LRT only profiling and tracing
  --shv<n>          LRT only profiling and tracing
  -s, --sym=<symbol file>	symbol file
  --lttng=<log path>	dumps LTTNG logs to specified path
  --sample              treat input as sample profile data
  --version             print version


Examples:
  profiler --los input.bin	Process input.bin file and creates LOS csv files
  profiler --lrt input.bin	Process input.bin file and creates LRT csv files
  profiler --los input.bin -s symfile.sym	Process input.bin file as LOS file using the symfile.sym to decode function addresses.
  profiler input.bin --verbose	Process input.bin file and print some more useless info
  date -d @$(moviProf --version | cut -f2 -d@)
```

In order to parse raw data and output results to csv output file(s), one should not use the --sample option. Please note that if none of the mutually exclusive flags –los, --lrt, --shv<n> is specified, the tool will output incomplete profiling information.
>In general this method is not recommended. In all cases, using the `make PROFILE_INFO=function run` command is better.

### Interpreting results

Depending on the options used, several files can be created:
- Profiling\_LRT.csv and Tracing\_LRT.csv if --lrt argument is passed
- Profiling\_LOS.csv, Tracing\_LOS.csv and Profiling\_LOS\_compact.csv for -–los
- Prof\_shave.csv and Tracing.csv for --shv&lt;n&gt;

These files will be generated in output/profile if the make profile targets were used to run the application.

The profiling csv file contains the total time of profiling, the total time used by the instrumented functions,
and information about every function that was instrumented. Some functions may be found more than one time in the
Profiler.csv, this should be the case when the same function is called from multiple threads.
If the total time of profiling is lower than 100ms, it is recommended to remove some of the functions from
instrumentation (in order to profile the application for more time – at least 1 second is recommended) or increase
the profiling data buffer size (if the memory is available).

The generated profile csv file have up to 13 columns, minimum 11, depending on the profile input buffer type

|#0|#1|#2|#3|#4|#5|#6|#7|#8|#9|#10|#11|#12|#13|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|API|THD(s)|CALLS|AV-EXCL|AV-INCL|MIN-EXCL|MIN-INCL|MAX-EXCL||MAX-INCL|TOTAL|USAGE(%)|TOTAL/F\_SIZE\_B|F\_SIZE\_B|FUNCTION_NAME|

In the compact version of LOS, the API is not present, and the functions are no longer grouped by thread. In this view can be seen the total running time of a particular function regardless in which thread it was called.

The meaning of the columns is as follows:

- THD(s) represents the thread ID that call a certain function. In compact mode this is a list of all threads calling this particular function.
- API refers to the used RTEMS API (such as classic, Posix, etc.). In compact mode or when RTEMS threads are available, those two fields are missing (API and THD)

The next fields are available on all csv files and have the following meanings:

- CALLS represents the number of calls of that function on the corresponding thread (in case of RTEMS).
- AV-EXCL represents the average Exclusive time (in cc) of that function. It is calculated as a sum of all exclusive time of that function, divided by the number of calls.
- AV-INCL represents the average Inclusive time (in cc) of that function. It is calculated as a sum of all inclusive time of that function, divided by the number of calls.
- MIN-EXCL represents the minimum function duration (in cc). It does not contain thread switch time or other function calls in that function.
- MIN-INCL represents the minimum function duration (in cc, also meaning the shortest function call) including calls to other functions, but excluding time spent in other threads.
- MAX-EXCL represents the maximum function duration (in cc, also meaning the longest function call). It does not contain thread switch time or other function calls in that function.
- MAX-INCL represents the maximum function duration (in cc, also meaning the longest function call) including calls to other functions, but excluding time spent in other threads.
- TOTAL lists the sum of each exclusive function calls, also meaning the time spent by the processor in this function, excluding calls to other functions.

- USAGE is calculated as TOTAL time divided by profiling time. It reflects the usage of that function in percentage.
- TOTAL/F\_SIZE\_B is used to reflect how many cc were spent to execute (on average) an instruction in that function. It is calculated as TOTAL function time divided by the size of the function.
- F\_SIZE\_B is the size of the function (in Bytes). If the symbols file is not provided as an input to the parser, these column will display 0.
- FUNCTION\_NAME is the name of the function. When moviProf is failing to associate the function name to the function address, the address will be displayed instead.
 The user can manually search for the function name in the .lst or .sym file, with nm or objdump in .elf, etc.

The Tracing\_LOS/LRT csv file contains entries sorted by timestamp. For the case of RTEMS it contains the function calls and
 thread switches as they happened for the last FP\_CORE\_BUFFER\_SIZE/16 entries, while on Bare Metal, only function calls are
  displayed. The contents are as follows:

- #0 ENTRY_TYPE is one of: Entry (Function Entry), Exit (Function Exit), TEntry (entering a new thread)  or TExit (exiting the old thread). The last two are only used in RTEMS.
- #1 FUNCTION_NAME/THREAD_ID is the functions name or the thread's ID (in the case of TEntry and TExit)
- #2 TIMESTAMP is the 64 bit timestamp of the entry.
- #3 INCLUSIVE_TIME this is populated only for Exit entries. It contains the inclusive cc of that particular call of function
- #4 EXCLUSIVE_TIME this is populated only for Exit entries. It contains the exclusive cc of that particular call of function

>The “No records found in input file” message is valid for some shaves. If it appears for Leon, then please check that the free running counter is powered/clocked correctly, and that the profiled code is actually executed on the Leon.

## Sample Profiler

The sample profiler is reading and storing the IP addresses of running Shaves at configured time intervals.
At the end of the execution, several .gprof filer are generated on disk (output/sampleProfiler/).

The sampling interrupt can be set to trigger on any/both of the Leons (LRT and/or LOS) and is non-intrusive for SHAVE processors, as it does not require extra instrumentation. It can be run on both RTEMS or baremetal environments. The Leons sample the SHAVE's instruction pointers and put them into memory buffers (each Leon has its own).

After the buffers are saved, they are given to the moviProf as input and the“.elf” file is parsed in order to determine the address space for each function from the Shaves. With this information, moviProf will create the gprof files including the time spent and the percentages out of total runtime for each function from each Shave.
For a quick run-through, please try the sample project SampleProfilerExample from the MDK example/HowTo projects, to see the actual files that are produced.

>It is not possible to do sampling profiling of the Leon cores with the current release of the sampling profiler.

### Usage

Compile and run using the `make run PROFILE_INFO=sample` target. Also `profile`, `debug`, etc targets can be used.

### Sample Profiler Configuration

Name|Default Value|Description
--- | --- | ---
PROFILE_INFO|sample|This flag instruct the buildsystem to use the sample profiling. This is the only mandatory variable
SAMPLE\_PERIOD\_MICRO SAMPLE\_PERIOD\_MICRO\_LRT|10|Instruction pointer read interval in microseconds. The default value should work for most of the applications. If the application runs shaves for a long time, bigger values can be used for decreasing the used DDR size
SAMPLE\_DURATION\_MS SAMPLE\_DURATION\_MS\_LRT|14000|Maximum duration of the profiling, in milliseconds. This value should be configured to cover the total runtime of Shaves.
SP\_TIMER\_INTERRUPT\_LEVEL SP\_TIMER\_INTERRUPT\_LEVEL\_LRT|13|Timer interrupt
SAMPLE\_PROFILER\_DATA\_SECTION SAMPLE\_PROFILER\_DATA\_SECTION\_LRT|\_\_attribute\_\_ ((section (\".ddr_direct.bss\")))|Sample profiler data section name

Update SAMPLE\_PERIOD\_MICRO and SAMPLE\_DURATION\_MS if absolutely necessary. These parameters will impact the DDR buffer size used by the profiler. For most of the applications, the default values should work. The used DDR space in bytes will be defined by the formula:

``` c++
const int ddrBufferSize =
    (SAMPLE_DURATION_MS / SAMPLE_PERIOD_MICRO * 1000) * ADDR_SIZE;
```

where ADDR\_SIZE is 4 on current myriad platforms.

### How it works

The sample profiler overwrite all the swcStartShave functions to start a timer and set up an interrupt.

For RTEMS, a task is created in order to start the timer. In the timer interrupt, the instruction pointer of the Shave is read,
at every x ms. The sampling profiler will overwrite the swcWaitShave function too, which will stop the measurements for bare metal.
For RTEMS, the functions OsDrvSvuStartShave and OsDrvSvuStartShaveCC call the function swcShaveStartAsync.
The function that is given as a parameter to swcShaveStartAsync will be called when the shaves finish execution, and will
then send an event to the measurement task to stop the timer. In swcShaveLoader, if using RTEMS, the overwritten functions will
use RTEMS drivers, otherwise they will use the bare metal drivers.

A hook into moviDebug2 is added in order to save the instruction pointers into a binary file and decode it to
.gprof afterwards. The moviDebug2 will be started using a user-provided debug script or a default script
if there are no custom scripts provided. The default script will try to save both buffers, one for LOS and one for LRT, even if
shaves are not started from one of them. In the case the buffer doesn't exist, an error message can appear from the debugger,
mentioning that the symbol could not be found. It is normal and will not affect the overall functioning of the Sample
Profiler – only the existing buffer will be saved. If both LOS and LRT buffers exist, the functions from Shaves that are started
from LRT will be prefixed with “lrt\_”. In the case that a Shave is used by both Leons, the functions called will appear
twice, for each Leon. The real time spent in a function will be the sum of the time associated with each of the Leons (with
and without “lrt_”).

The saved binary data is converted into gprof compatible files that can be viewed either in command line or moviEclipse.

## Trace Profiler

The standard events are defined in dbgLogEvents.h and have id's starting from 1 to 9999. User defined events must have
id's from 9999 (LOG_EVENT_LAST_EVENT).

### Configuration

Name|Default Value|Description
--- | --- | ---
`TRACE_BUFFER_SIZES`|1024|Trace buffer size in bytes


### Trace Profiler API

It is defined in the dbgTracerApi.h

``` C++
////////////////////////////////////////////////////////////////////////////
/// @brief initTraceProfiler - Init function
///
/// @param buffer - trace profiler buffer. If NULL it will disable logging
/// @param size - trace buffer size
/// @param level - logging level. if set to 0, then profiling will be disabled
////////////////////////////////////////////////////////////////////////////
void initTraceProfiler(void* buffer, size_t size, uint32_t level);
```
This is the init function. Normally you don't need to call it explicitly unless you have "special needs" :)
It is called from a static constructor with a preallocated buffer of predefined size. The size is configurable, please
see the [configuration section](#Configuration) for more details

``` C++
/// @brief Logs Event from the system by a specific logger method
///
/// Components that are going to be logged are: Timestamp, eventID and
/// data value (if any) to a (ring) buffer
/// @param[in] eventId    - ID of the event that will be logged
/// @param[in] data       - Data field containing custom info
/// @param[in] eventLevel - Debug Level for the current eventId to determine if
///            the event would be traced or not.
/// @return void
void dbgLogEvent(Event_t eventId, u32 data, u32 eventLevel);
```

## FAQ

#### 1. Code compiled succesfully with `PROFILE_INFO=<type>` but I no result. Where should I look?

> Run `make clean`
>
> Sometimes helps when make doesn't rebuild
> objects from previous project compilations

#### 2. Code and/or data too big after function profiling the code and doesn't fit to memory

> First, reduce the buffer size `FP_<CORE>_BUFFER_SIZE=size`
> and/or enable profiling
> only on the core you need to analyze `FP_CORE_PROFILE=...`.
>
> Other potential solutions would be to try to remove first
> unused code, or to use sample profiling instead
> of function profiling.
> Also make sure you build in release mode.

#### 3. Can I function profile a project that enable/disable DDR?

> Yes. But make sure you take out the profiler buffers from DDR in other sections
>
> e.g.: `FP_LOS_DATA_SECTION=.cmx.data`

#### 4. What happens with the profiled application if I stop the timer block?

> Nothing. It will simply not work. It is required for the timer block
> to have clock all the time.

#### 5. While collecting buffers from shave function profiling, buffer symbols are not visible

> Check if symbols contain spaces. If it is so, then check in your project's Makefile how SHAVE\_APP\_LIBS is built
> The profiler needs to know the symbol prefix

#### 6. I have problems when profiling under Windows. What can I try?

> Linux

#### 6. I have the message _"unable to install thread switch extension hook"_ and no thread switch event is detected

> The profiler needs to add the thread switch extension in RTEMS. Please check the define
> CONFIGURE\_MAXIMUM\_USER\_EXTENSIONS. It should be defined and at least equal to 1 or greater if you have your own
> extensions

#### 7. I have link errors after `make clean all PROFILE_INFO=...` but works without clean `make all && make all PROFILE_INFO=...`

> The profiler set the output directory to a different place for safety. Unfortunatelly some projects
> are using the ./output/ hardcoded in Makefile instead of $DirAppOutput. If this is the case, either fix the project's
> Makefile, or set `FP_OUTPUT_DIRECTORY=output`

## Internals

If sample profiler is activated, then SAMPLE\_PROFILING is defined
If function profiler is activated, then LRT\_PROFILE, LOS\_PROFILE, and/or SHAVE\_PROFILE is defined
Trace profiler code is not only in common/components/profiler, but also in common/swCommon/src/traceProfiler.c, common/swCommon/include/dbgTracerApi.h,
and common/swCommon/include/dbgLogEvents.h

<!-- Links section -->

[1]: http://www.movidius.org/tools "Movi Tools"

<!---
 TODO: convert doc to md:
soffice --headless --convert-to html moviProf_00.xx.xx.doc
pandoc -t markdown -o moviProfHowTo.md moviProf_00.xx.xx.html
-->
