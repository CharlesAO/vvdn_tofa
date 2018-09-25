namespace eval tcf::MyriadCore {

    namespace eval HISTORY::FIELDS {
        set PC               "PC"
        set PC.FORMAT        "0x%.8X"
        set OPCODES          "Opcodes"
        set DISASSEMBLY      "Disassembly"

        # LEON
        set TIME_TAG         "TimeTag"
        set RESULT_OR_STORE  "ResultOrStore"
        set INSTRUCTION_TRAP "InstructionTrap"
        set ERROR_MODE       "ErrorMode"
        set MULTI_CYCLE      "MultiCycle"

        # SHAVE
        set BRANCH_TAKEN     "BranchTaken"
        set PREDICATE        "Predicate"
    }

    namespace eval CONFIG::FIELDS {
        set SAFETY              "Safety"
        set SAFETY.TYPE         [namespace parent]::SAFETY::OBJECT
        set SAFETY.VALUES       [namespace parent]::SAFETY::VALUES

        set STATE_UPDATE        "StateUpdate"
        set STATE_UPDATE.TYPE    json::Boolean

        set STATE_UPDATE_INTERVAL       "StateUpdateInterval"
        set STATE_UPDATE_INTERVAL.TYPE  json::NullableNumber

        set CACHE_ACCESS        "CacheAccess"
        set CACHE_ACCESS.TYPE   [namespace parent]::CACHE_ACCESS::OBJECT
        set CACHE_ACCESS.VALUES [namespace parent]::CACHE_ACCESS::VALUES
    }

    namespace eval CONFIG::SAFETY::FIELDS {
        set DEFAULT              "Default"
        set DEFAULT.VALUES       [namespace parent]::VALUES
        set DEFAULT.TYPE         json::NullableString

        set MEMORY_READ          "MemoryRead"
        set MEMORY_WRITE         "MemoryWrite"
        set DISASSEMBLY          "Disassembly"
        set SOFTWARE_BREAKPOINT  "SoftwareBreakpoint"
        set L1_DATA_CACHE_READ   "Level1DataCacheRead"
        set L1_DATA_CACHE_WRITE  "Level1DataCacheWrite"
        set L1_INSTR_CACHE_READ  "Level1InstCacheRead"
        set L1_INSTR_CACHE_WRITE "Level1InstrCacheWrite"
        set L2_CACHE_READ        "Level2CacheRead"
        set L2_CACHE_WRITE       "Level2CacheWrite"

        set MEMORY_READ.VALUES          ${DEFAULT.VALUES}
        set MEMORY_WRITE.VALUES         ${DEFAULT.VALUES}
        set DISASSEMBLY.VALUES          ${DEFAULT.VALUES}
        set SOFTWARE_BREAKPOINT.VALUES  ${DEFAULT.VALUES}
        set L1_DATA_CACHE_READ.VALUES   ${DEFAULT.VALUES}
        set L1_DATA_CACHE_WRITE.VALUES  ${DEFAULT.VALUES}
        set L1_INSTR_CACHE_READ.VALUES  ${DEFAULT.VALUES}
        set L1_INSTR_CACHE_WRITE.VALUES ${DEFAULT.VALUES}
        set L2_CACHE_READ.VALUES        ${DEFAULT.VALUES}
        set L2_CACHE_WRITE.VALUES       ${DEFAULT.VALUES}

        set MEMORY_READ.TYPE            ${DEFAULT.TYPE}
        set MEMORY_WRITE.TYPE           ${DEFAULT.TYPE}
        set DISASSEMBLY.TYPE            ${DEFAULT.TYPE}
        set SOFTWARE_BREAKPOINT.TYPE    ${DEFAULT.TYPE}
        set L1_DATA_CACHE_READ.TYPE     ${DEFAULT.TYPE}
        set L1_DATA_CACHE_WRITE.TYPE    ${DEFAULT.TYPE}
        set L1_INSTR_CACHE_READ.TYPE    ${DEFAULT.TYPE}
        set L1_INSTR_CACHE_WRITE.TYPE   ${DEFAULT.TYPE}
        set L2_CACHE_READ.TYPE          ${DEFAULT.TYPE}
        set L2_CACHE_WRITE.TYPE         ${DEFAULT.TYPE}
    }

    namespace eval CONFIG::SAFETY::VALUES {
        set DEFAULT             ""
        set SAFE                "Safe"
        set STRICT              "Strict"
        set BYPASS_CACHE        "BypassCache"
        set UNSAFE              "Unsafe"
    }

    proc CONFIG::SAFETY::OBJECT {value} {
        if {[llength $value] == 1} {
            return [json::String $value]
        }
        json::ObjectFromFieldDict $value ; # CONFIG::SAFETY
    }

    namespace eval CONFIG::CACHE_ACCESS::VALUES {
        set DEFAULT             ""
        set PRECISE             "Precise"
        set SIMPLE              "Simple"
        set BYPASS              "Bypass"
    }

    namespace eval CONFIG::CACHE_ACCESS::FIELDS {
        set DEFAULT              "Default"
        set DEFAULT.VALUES       [namespace parent]::VALUES
        set DEFAULT.TYPE         json::NullableString

        set LEVEL_1                   "Level1"
        set LEVEL_1_DATA              "Level1Data"
        set LEVEL_1_DATA_READ         "Level1DataRead"
        set LEVEL_1_DATA_WRITE        "Level1DataWrite"
        set LEVEL_1_INSTR             "Level1Instr"
        set LEVEL_1_INSTR_READ        "Level1InstrRead"
        set LEVEL_1_INSTR_WRITE       "Level1InstrWrite"
        set LEVEL_2                   "Level2"
        set LEVEL_2_READ              "Level2Read"
        set LEVEL_2_WRITE             "Level2Write"

        set LEVEL_1.VALUES                   ${DEFAULT.VALUES}
        set LEVEL_1_DATA.VALUES              ${DEFAULT.VALUES}
        set LEVEL_1_DATA_READ.VALUES         ${DEFAULT.VALUES}
        set LEVEL_1_DATA_WRITE.VALUES        ${DEFAULT.VALUES}
        set LEVEL_1_INSTR.VALUES             ${DEFAULT.VALUES}
        set LEVEL_1_INSTR_READ.VALUES        ${DEFAULT.VALUES}
        set LEVEL_1_INSTR_WRITE.VALUES       ${DEFAULT.VALUES}
        set LEVEL_2.VALUES                   ${DEFAULT.VALUES}
        set LEVEL_2_READ.VALUES              ${DEFAULT.VALUES}
        set LEVEL_2_WRITE.VALUES             ${DEFAULT.VALUES}

        set LEVEL_1.TYPE                     ${DEFAULT.TYPE}
        set LEVEL_1_DATA.TYPE                ${DEFAULT.TYPE}
        set LEVEL_1_DATA_READ.TYPE           ${DEFAULT.TYPE}
        set LEVEL_1_DATA_WRITE.TYPE          ${DEFAULT.TYPE}
        set LEVEL_1_INSTR.TYPE               ${DEFAULT.TYPE}
        set LEVEL_1_INSTR_READ.TYPE          ${DEFAULT.TYPE}
        set LEVEL_1_INSTR_WRITE.TYPE         ${DEFAULT.TYPE}
        set LEVEL_2.TYPE                     ${DEFAULT.TYPE}
        set LEVEL_2_READ.TYPE                ${DEFAULT.TYPE}
        set LEVEL_2_WRITE.TYPE               ${DEFAULT.TYPE}
    }

    proc CONFIG::CACHE_ACCESS::OBJECT {value} {
        if {[llength $value] == 1} {
            return [json::String $value]
        }
        json::ObjectFromFieldDict $value ; # CONFIG::CACHE_ACCESS
    }

    proc getExecutionHistory { contextId count disassemble } {
        tcf::protocol::jsonCommand \
            MyriadCore getExecutionHistory \
            [json::String $contextId] \
            [json::Integer $count] \
            [json::Boolean $disassemble]
    }

    proc setEntryPoint { contextId address } {
        tcf::protocol::jsonCommand \
        MyriadCore setEntryPoint \
        [json::String $contextId] \
        [json::Integer $address]
    }

    proc getPhysicalAddress { contextId address } {
        tcf::protocol::jsonCommand \
        MyriadCore getPhysicalAddress \
        [json::String $contextId] \
        [json::Integer $address]
    }

    proc getExitCode { contextId } {
        tcf::protocol::jsonCommand \
            MyriadCore getExitCode \
            [json::String $contextId]
    }

    proc getPC { contextId } {
        tcf::protocol::jsonCommand \
            MyriadCore getPC \
            [json::String $contextId]
    }

    proc setConfig { contextId config } {
        tcf::protocol::jsonCommand \
            MyriadCore setConfig [json::String $contextId] $config
    }

    proc getConfig { contextId } {
        tcf::protocol::jsonCommand \
            MyriadCore getConfig [json::String $contextId]
    }

    proc start { contextId } {
        tcf::protocol::jsonCommand \
        MyriadCore start \
        [json::String $contextId]
    }

    proc reset { contextId } {
        tcf::protocol::jsonCommand \
        MyriadCore reset \
        [json::String $contextId]
    }

    proc cont { contextId } {
        tcf::protocol::jsonCommand \
        MyriadCore cont \
        [json::String $contextId]
    }

    proc stop { contextId } {
        tcf::protocol::jsonCommand \
        MyriadCore stop \
        [json::String $contextId]
    }

    proc step { contextId } {
        tcf::protocol::jsonCommand \
        MyriadCore step \
        [json::String $contextId]
    }

    proc setPower { contextId powered } {
        tcf::protocol::jsonCommand \
        MyriadCore setPower \
        [json::String $contextId] \
        [json::Boolean $powered]
    }

    proc setClock { contextId clocked } {
        tcf::protocol::jsonCommand \
        MyriadCore setClock \
        [json::String $contextId] \
        [json::Boolean $clocked]
    }

    proc updateState { contextId sync } {
        tcf::protocol::jsonCommand \
        MyriadCore updateState \
        [json::String $contextId] \
        [json::Boolean $sync]
    }

    proc getCoreList { contextId } {
        tcf::protocol::jsonCommand \
        MyriadCore getCoreList \
        [json::String $contextId] \
    }

} ; # namespace eval tcf::MyriadCore

