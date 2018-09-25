#
# Expression evaluation service.
#
# Extensions to regular C/C++ syntax:
# 1. Special characters in identifiers: $"X", or just "X" if followed by ::
#    where X is object name that can contain any characters.
# 2. Symbol IDs in expressions: ${X}
#    where X is symbol ID as returned by symbols service.
# 3. CPU registers: $X
#    where X is a register name, e.g. $ax
#
namespace eval tcf::Expressions {

    # Expression context property names.
    namespace eval PROPERTIES::FIELDS {
        set ID "ID"
        set PARENT_ID "ParentID"
        set SYMBOL_ID "SymbolID"
        set LANGUAGE "Language"
        set EXPRESSION "Expression"
        set BITS "Bits"
        set SIZE "Size"
        set CLASS "Class"
        set TYPE "Type"
        set CAN_ASSIGN "CanAssign"
        set HAS_FUNC_CALL "HasFuncCall"

        namespace eval CLASS {
            tcf::fields::importVars tcf::Symbols::SYMBOL::FIELDS::TYPE_CLASS
        }
    }

    # Expression scope attributes.
    # @since 1.2
    namespace eval SCOPE::FIELDS {
        set CONTEXT_ID "ContextID"
        set ADDRESS "Address"
        set LANGUAGE "Language"
    }

    # Expression value property names
    namespace eval VALUE::FIELDS {
        set CLASS "Class"
        set TYPE "Type"
        set BIG_ENDIAN "BigEndian"
        set REGISTER "Register"
        set SYMBOL "Symbol"
        set ADDRESS "Address"
        set ADDRESS.FORMAT 0x%.8X

        namespace eval CLASS {
            tcf::fields::importVars tcf::Symbols::SYMBOL::FIELDS::CLASS
        }
    }

    # @since 1.3
    namespace eval VALUE::FIELDS {
        set BINARY_SCALE "BinaryScale"
        set DECIMAL_SCALE "DecimalScale"
        set IMPLICIT_POINTER "ImplicitPointer"
        set PIECES "Pieces"
    }

    # Retrieve expression context info for given context ID.
    # @see Expression
    #
    # @param id - context ID.
    # @return context properties.
    proc getContext {id} {
        tcf::protocol::jsonCommand \
            Expressions getContext \
            [json::String $id]
    }

    # Retrieve children IDs for given parent ID.
    # Meaning of the operation depends on parent kind:
    # 1. expression with type of a struct, union, or class - fields;
    # 2. expression with type of an enumeration - enumerators;
    # 3. expression with type of an array - array elements;
    # 4. stack frame - function arguments and local variables;
    # 5. thread - top stack frame function arguments and local variables;
    # 6. process - global variables;
    #
    # Children list does *not* include IDs of expressions that were created by clients
    # using "create" command.
    #
    # @param parent_context_id - parent context ID.
    # @return array of available context IDs.
    proc getChildren {parentContextId} {
        tcf::protocol::jsonCommand \
            Expressions getChildren \
            [json::String $parentContextId]
    }

    # Create an expression context.
    # The context should be disposed after use.
    # @param parent_id - a context ID that can be used to resolve symbol names.
    # @param language - language of expression script, null means default language
    # @param expression - expression script
    # @return context properties.

    proc create {parent_id language expression} {
        tcf::protocol::jsonCommand \
            Expressions create \
            [json::String $parent_id] \
            [json::String $language] \
            [json::String $expression]
    }

    # Create an expression context using given scope to resolve symbols.
    # The context should be disposed after use.
    # @param scope - dictionary of see Scope::FIELDS attribute definitions.
    # @param expression - expression script
    # @return context properties.
    # @since 1.2
    proc createInScope {scope expression} {
        tcf::protocol::jsonCommand \
            Expressions createInScope \
            [json::ObjectFromFieldDict $scope SCOPE] \
            [json::String $expression]
    }

    # Dispose an expression context that was created by create()
    # @param id - the expression context ID
    proc dispose {id} {
        tcf::protocol::jsonCommand \
            Expressions dispose [json::String $id]
    }

    # Evaluate value of an expression context.
    # @param id - the expression context ID
    # @param done - call back interface called when operation is completed.
    # @return - expression evaluation result: binary value, type info
    proc evaluate {id} {
        set listResult [tcf::protocol::jsonListCommand \
            2 1 \
            Expressions evaluate \
            [json::String $id] \
        ]

        lset listResult 0 [
            binary decode base64 [lindex $listResult 0]
        ]
    }

    # Assign a value to memory location determined by an expression.
    # @param id - expression ID.
    # @param value - value as an array of bytes.
    proc assign {id value} {
        tcf::protocol::jsonCommand \
            Expressions assign [json::String $id] [json::Base64 $value]
    }

    namespace eval Events {
        # Called when expression value was changed and clients
        # need to update themselves. Clients, at least, should invalidate
        # corresponding cached expression data.
        # Not every change is notified - it is not possible,
        # only those, which are not caused by normal execution of the debuggee.
        # At least, changes caused by "assign" command should be notified.
        # @param id - expression context ID.
        tcf::protocol::JsonEvent Expressions valueChanged
    } ; # namespace eval Events

} ; # namespace eval tcf::Expressions

namespace eval tcf {

    proc getExpressionProperty {exprContextDict field {default {}} } {
        fields::getNamedField Expressions::PROPERTIES $exprContextDict $field $default
    }

    proc getExpressionPropertyList {exprContextDict fieldList {defaults {}} } {
        fields::getNamedFieldList Expressions::PROPERTIES $exprContextDict $fieldList $defaults
    }

    proc getExpressionPropertyDict {exprContextDict} {
        fields::getNamedFieldDict Expressions::PROPERTIES $exprContextDict
    }


} ; # namespace eval tcf

