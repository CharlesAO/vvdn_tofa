namespace eval tcf::Disassembly {

    # predefined properties for params and capabilities
    # <string> The name of the instruction set architecture
    set ISA ISA
    # <boolean> If true, simplified mnemonics are specified.
    set SIMPLE Simplified
    # <boolean> If true, pseudo-instructions are requested.
    set PSEUDO PseudoInstructions
    #  <boolean> If true, the instruction code byte values are returned.
    set OPCODE OpcodeValue

    namespace eval RESULT::FIELDS {
        # <string> Instruction Set Architecture of the disassembly line
        set ISA ISA
        # <int> - Address of the instruction
        set ADDRESS Address
        set ADDRESS.FORMAT "0x%08X"
        # <int> Size of the instruction in bytes
        set SIZE Size
        set SIZE.TYPE json::Integer
        # <array> - Array of the intruction fields
        set INSTR Instruction
        # <string> - Encoded byte array of the instruction binary representation
        set OPCODE OpcodeValue

        # <object: Instruction field>
        # Instruction field is an object containing properties.
        # Predefined properties are:
        namespace eval INSTR::FIELDS {
            # <string> - The predefined type of the instruction field
            set TYPE Type
            # Predefined types are:
            namespace eval type {
                set STRING String
                set REGISTER Register
                set ADDRESS Address
                set DISPLACEMENT Displacement
                set IMMEDIATE Immediate
            }

            # <string> - Value of the field for "String" and "Register" types.
            set TEXT Text
            # <number> - Value of the field for "Address," "Displacement," or "Immediate" types.
            set VALUE Value
            # <context-id> - Context-ID of the address space used with the "Address" type.
            set ADDRESS_SPACE AddressSpace
        }
    }

    # C • <token> • Disassembly • disassemble •
    # <string: context-id> • <int: start location>
    # • <int: size> • <object: disassembly parameters> •
    proc disassemble {contextId startLocation size {params {}}} {
        tcf::protocol::jsonCommand \
            Disassembly disassemble \
            [json::String $contextId] \
            [json::Number $startLocation] \
            [json::Number $size] \
            [json::Object $params]
    }

    proc getCapabilities {contextId} {
        tcf::protocol::jsonCommand \
            Disassembly getCapabilities \
            [json::String $contextId]
    }
} ; # namespace eval tcf::Disassembly

namespace eval tcf {
    proc getDasmResultFieldList {dasmItemDict fieldDefs {defaults {}}} {
        fields::getNamedFieldList Disassembly::RESULT $dasmItemDict $fieldDefs $defaults
    }
} ; # namespace eval tcf



