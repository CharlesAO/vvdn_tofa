# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcl/LineNumbers.tcl
# Description:
# Created on : Aug 28, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval tcf::LineNumbers {

    namespace eval CodeArea::FIELDS {
        set DIRECTORY Dir
        set FILE File
        set START_LINE SLine
        set END_LINE ELine
        set START_COLUMN SCol
        set END_COLUMN ECol
        set START_ADDRESS SAddr
        set START_ADDRESS.TYPE json::Number
        set START_ADDRESS.FORMAT "0x%08X"
        set END_ADDRESS EAddr
        set END_ADDRESS.TYPE json::Number
        set END_ADDRESS.FORMAT "0x%08X"
        set NEXT_ADDRESS NAddr
        set NEXT_ADDRESS.FORMAT "0x%08X"
        set ISA ISA
        set ISA.TYPE json::Integer
        set IS_STMT IsStmt
        set IS_STMT.TYPE json::Boolean
        set BASIC_BLOCK BasicBlock
        set BASIC_BLOCK.TYPE json::Boolean
        set PROLOGUE_END PrologueEnd
        set PROLOGUE_END.TYPE json::Boolean
        set EPILOGUE_BEGIN EpilogueBegin
        set EPILOGUE_BEGIN.TYPE json::Boolean
    }

    # Maps context address to source lines
    # @param context_id - context id on which to operate
    # @param start_address - start of range
    # @param end_address - end of range
    # @return list of Code area dictionaries
    proc mapToSource {context_id start_address end_address} {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand LineNumbers mapToSource \
                [json::String $context_id] \
                [json::Number $start_address] \
                [json::Number $end_address]
        }
    }

    # Maps file/line/column numbers to context memory addresses
    # @param context_id - context id on which to operate
    # @param file - file name
    # @param line - line number
    # @param column - column number
    # @return list of Code area dictionaries
    proc mapToMemory {context_id file line column} {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand LineNumbers mapToMemory \
                [json::String $context_id] \
                [json::String $file] \
                [json::Integer $line] \
                [json::Integer $column]
        }
    }

} ; # namespace eval tcf::LineNumbers

namespace eval tcf {

    proc getCodeAreaField {codeAreaDict field {default {}}} {
        fields::getNamedField LineNumbers::CodeArea $codeAreaDict $field $default}

    proc getCodeAreaFieldList {codeAreaDict fieldList {defaults {}}} {
        fields::getNamedFieldList LineNumbers::CodeArea $codeAreaDict $fieldList $defaults
    }

    proc getCodeAreaDictList {codeAreaList} {
        fields::getNamedFieldDictList LineNumbers::CodeArea $codeAreaList
    }

    proc getCodeAreaTable {codeAreaList fieldList {defaults {}}} {
        fields::getNamedFieldTable LineNumbers::CodeArea $codeAreaList $fieldList $defaults
    }

}

