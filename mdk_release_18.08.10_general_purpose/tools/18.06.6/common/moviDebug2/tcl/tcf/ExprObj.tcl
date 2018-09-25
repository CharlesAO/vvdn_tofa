# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcf/ExprObj.tcl
# Description: Tcl class encapsulating an expression
# Created on : Sep 15, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval tcf {

    oo::class create ExprObj {

        variable ParentObj
        variable Properties
        variable IsOwner
        variable Children
        variable ValueInfo

        constructor {properties {parentObj {}} } {
            set Properties $properties
            if {$parentObj != {}} {
                set IsOwner yes
            } else {
                set ParentObj $parentObj
            }
        }

        destructor {
            if {[info exists Children]} {
                foreach child $Children {
                    $child destroy
                }
            }

            if {[info exists IsOwner] && $IsOwner} {
                if {[set id [my GetID]] != {}} {
                    tcf::Expressions::dispose $id
                }
            }
        }

        method <cloned> {src} {
            return -code error "cloning disabled"
        }

        method getID {} {
            my getField ID
        }

        method getExpression {} {
            my getField EXPRESSION
        }

        method getParentID {} {
            my getField PARENT_ID
        }

        method getLanguage {} {
            my getField LANGUAGE
        }

        method canAssign {} {
            my getField CAN_ASSIGN 0
        }

        method hasFuncCall {} {
            my getField HAS_FUNC_CALL 0
        }

        method getChildren {} {
            if {![info exists Children]} {
                set childIDs [tcf::Expressions::getChildren [my getID]]
                set Children [list]
                set this [self]
                foreach id $childIDs {
                    set properties [tcf::Expressions::getContext $id]
                    lappend Children [tcf::ExprObj new $properties $this]
                }
            }
            return $Children
        }

        method getType { } {
            my getField TYPE
        }

        method getField {fieldName {default {}}} {
            if {![info exists Properties]} {
                return $default
            }
            tcf::fields::getNamedField Expressions::PROPERTIES [
                my GetProperties
            ] $fieldName $default
        }

        method getProperties {} {
            tcf::fields::getNamedFieldDict Expressions::PROPERTIES [
                my GetProperties
            ]
        }

        method evaluate {} {
            lassign [tcf::Expressions::evaluate [my getID]] value ValueInfo
            return $value
        }

        method getValueInfoField {fieldName {default {}}} {
            if {![info exists ValueInfo]} {
                return default
            }
            tcf::fields::getNamedField Expressions::VALUE $ValueInfo $fieldName $default
        }

        method getValueInfo {} {
            if {![info exists ValueInfo]} {
                return
            }
            tcf::fields::getNamedFieldDict Expressions::VALUE $ValueInfo
        }

        method assign {binaryValue} {
            tcf::Expressions::assign [my getID] $binaryValue
        }

        #
        #  Private
        #
        method GetProperties {} {
            if {![info exists Properties]} {
                return
            }
            return $Properties
        }
    }

    oo::class create ExprContext {
        superclass tcf::ExprObj
        constructor {parent_id language expression} {
             next [tcf::Expressions::create $parent_id $language $expression]
        }

    } ; # oo::class ExprObj

    oo::class create ScopedExpr {
        superclass tcf::ExprObj
        constructor {scope expression} {
             next [tcf::Expressions::createInScope $scope $expression]
        }
    } ; # oo::class ExprObj

} ; # namespace eval tcf