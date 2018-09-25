// In order to understand how to use the dpu please see moviSim.pdf chapter "8. Using Computer Vision Models in PC simulation"
// 8.7. Examples of usage for Keembay [DPU:CNN]

#include <cstring>

// Step 0: Include mandatory headers
#include "dpuResources.h"
#include "dpuFilterApiTest.h"
#include "filtersConfiguration.h"

void(DpuFilterApiTest::*DpuFilterApiTest::dpuFilterApiTestMethods[])() = { &DpuFilterApiTest::ApbWriteReadSimulation };

// Step 0: creation of filters using "createFilterObject"
void DpuFilterApiTest::testDpuApi()
{
    if (!dpuLib->isLoaded())
    {
        printf("Library '%s' is not loaded!!!\n", "DPU");
        return;
    }

    createFilterObjectPtr createObject = nullptr;
    if (dpuLib->getProcAddress("createFilterObject", createObject))
    {
        constexpr char* filterName = "DPU";

        printf("START TESTING: %s \n", filterName);

        dpuFilter = createObject(filterName);

        for (auto testMethod : dpuFilterApiTestMethods)
        {
            (this->*testMethod)();
        }

        printf("END TESTING: %s ALL TESTS PASSED\n", filterName);
    }
}

void DpuFilterApiTest::loadLibrary(const char* libName)
{
    dpuLib = new os::SharedLibrary(libName);
}

void DpuFilterApiTest::unloadLibrary()
{
    if (dpuLib->isLoaded())
    {
        dpuLib->unload();
    }
}

// Step 1: define memory
alignas(128) uint8_t cmxNnMemory[CMX_DEPTH] = {};

// Step 2: implement dataRead and dataWrite access from DPUBlock to Step 1 defined cmxNnMemory
inline void dataRead(uint64_t address, uintptr_t *data, size_t size)
{
    *data = cmxNnMemory[address];
}

inline void dataWrite(uint64_t address, uintptr_t data, size_t size)
{
    cmxNnMemory[address] = data;
}

// Step 3 create dataReadWriteAccessPtr
dpuMemoryAccess dataReadWriteAccess;
dpuMemoryAccess* dataReadWriteAccessPtr = &dataReadWriteAccess;

// Step 3 assign callbacks 
void initDataAccess()
{
    dataReadWriteAccess.dataRead = &dataRead;
    dataReadWriteAccess.dataWrite = &dataWrite;

    dataReadWriteAccessPtr = &dataReadWriteAccess;
}

DpuFilterApiTest::DpuFilterApiTest()
{
    initDataAccess();
}

// Step 4: Configuration and run flow
void DpuFilterApiTest::ApbWriteReadSimulation()
{
    // Step 5: register in DPUBlock the callbacks
    dpuFilter->configure((void*)dataReadWriteAccessPtr);

    // Step 6: set data to cmxNnMemory

    // Step 7: configure the registers via ApbWrite
    dpuApbWrites();

    // Step 8: Start the DPU
    dpuApbWriteStartRegister();

    // Step 9: See the results in cmxNnMemory
}

void DpuFilterApiTest::dpuApbWrites()
{
    dpuFilter->apbWriteRegister((NCE_DPU_SE_ADDR_OFFSET), cmxNnMemory[0x00002000]);
    dpuFilter->apbWriteRegister((NCE_DPU_SPARSITY_ADDR_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_Z_CONFIG_OFFSET), 0x00000010);
    dpuFilter->apbWriteRegister((NCE_DPU_KERNAL_PAD_CFG_OFFSET), 0x00000420);
    dpuFilter->apbWriteRegister((NCE_DPU_WEIGHT_SIZE_OFFSET), 0x00000400);
    dpuFilter->apbWriteRegister((NCE_DPU_WEIGHT_NUM_OFFSET), 0x00000080);
    dpuFilter->apbWriteRegister((NCE_DPU_WEIGHT_START_OFFSET), cmxNnMemory[0x00014000]);
    dpuFilter->apbWriteRegister((NCE_DPU_WORKLOAD_SIZE0_OFFSET), 0x00010008);
    dpuFilter->apbWriteRegister((NCE_DPU_WORKLOAD_SIZE1_OFFSET), 0x00000400);
    dpuFilter->apbWriteRegister((NCE_DPU_WORKLOAD_START0_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_WORKLOAD_START1_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_OFFSET_ADDR_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_TENSOR_SIZE0_OFFSET), 0x00010008);
    dpuFilter->apbWriteRegister((NCE_DPU_TENSOR_SIZE1_OFFSET), 0x00000400);
    dpuFilter->apbWriteRegister((NCE_DPU_TENSOR_START_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_TENSOR_MODE_OFFSET), 0x00000052);
    dpuFilter->apbWriteRegister((NCE_DPU_ODU_CFG_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_DATA_BUFFER_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_SE_SIZE_OFFSET), 0x00000080);
    dpuFilter->apbWriteRegister((NCE_DPU_TE_END0_OFFSET), 0x0000E007);
    dpuFilter->apbWriteRegister((NCE_DPU_TE_END1_OFFSET), 0x0000007F);
    dpuFilter->apbWriteRegister((NCE_DPU_TE_BEG0_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_TE_BEG1_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_TE_DIM0_OFFSET), 0x00010008);
    dpuFilter->apbWriteRegister((NCE_DPU_TE_DIM1_OFFSET), 0x00000080);
    dpuFilter->apbWriteRegister((NCE_DPU_SE_BASE_PTR_OFFSET), cmxNnMemory[0x0003A000]);
    dpuFilter->apbWriteRegister((NCE_DPU_SP_BASE_PTR_OFFSET), cmxNnMemory[0x0003C100]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR0_OFFSET), cmxNnMemory[0x0003A100]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR1_OFFSET), cmxNnMemory[0x0003A300]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR2_OFFSET), cmxNnMemory[0x0003A500]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR3_OFFSET), cmxNnMemory[0x0003A700]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR4_OFFSET), cmxNnMemory[0x0003A900]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR5_OFFSET), cmxNnMemory[0x0003AB00]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR6_OFFSET), cmxNnMemory[0x0003AD00]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR7_OFFSET), cmxNnMemory[0x0003AF00]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR8_OFFSET), cmxNnMemory[0x0003B100]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR9_OFFSET), cmxNnMemory[0x0003B300]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR10_OFFSET), cmxNnMemory[0x0003B500]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR11_OFFSET), cmxNnMemory[0x0003B700]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR12_OFFSET), cmxNnMemory[0x0003B900]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR13_OFFSET), cmxNnMemory[0x0003BB00]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR14_OFFSET), cmxNnMemory[0x0003BD00]);
    dpuFilter->apbWriteRegister((NCE_DPU_DB_BASE_PTR15_OFFSET), cmxNnMemory[0x0003BF00]);
    dpuFilter->apbWriteRegister((NCE_DPU_MPE_CFG_OFFSET), 0x20000000);
    dpuFilter->apbWriteRegister((NCE_DPU_MPE_BUS_DATA_SEL_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_MPE_BUS_DATA_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_PPE_CFG_OFFSET), 0x00000000);
    dpuFilter->apbWriteRegister((NCE_DPU_DPU_STS_OFFSET), 0x00000000);
}

void DpuFilterApiTest::dpuApbWriteStartRegister()
{
    dpuFilter->apbWriteRegister((NCE_DPU_DPU_CFG_OFFSET), 0x0000000F);
}



DpuFilterApiTest::~DpuFilterApiTest()
{
}
