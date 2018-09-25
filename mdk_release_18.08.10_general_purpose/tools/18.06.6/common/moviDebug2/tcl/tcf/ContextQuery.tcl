namespace eval tcf::ContextQuery {

    # ContextQuery allows to search for context that match a pattern.
    # Query Syntax and Semantics
    #   query = [ "/" ], { part, "/" }, part ;
    #   part = string | "*" | "**" | properties ;
    #   properties = property, { ",", property } ;
    #   property = string, "=", value ;
    #   value = string | number | boolean ;
    #   string = quoted string | symbol ;
    #   quoted string = '"', {any-character - ('"' | '\') | ('\', ('"' | '\'))}, '"' ;
    #   symbol = letter, { letter | digit } ;
    #   number = digit, { digit } ;
    #   boolean = "true" | "false" ;
    #   letter = ? A-Z, a-z or _ ? ;
    #   digit = ? 0-9 ? ;
    #   any-character = ? any character ? ;
    proc query {{query {}}} {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand ContextQuery query [json::String $query]
        }
    }

    proc getAttrNames {} {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand ContextQuery getAttrNames
        }
    }

}