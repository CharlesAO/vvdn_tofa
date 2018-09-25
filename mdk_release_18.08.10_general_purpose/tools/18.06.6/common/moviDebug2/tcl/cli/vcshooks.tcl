
namespace eval mdbg {

    cli::Help::Manager add vcshooks::log -short "Configure VCS Hooks Logging" -synopsis {
        vcshooks::log {on|off} ?fileName? ?fileOpenMode?
    } -long {
        Configure the display and logging ouptut of VCS Hooks functionality.

        *** EXAMPLES

        Disable VCS Hooks display:
        % vcshooks::log off

        Enable VCS Hooks display on screen only:
        % vcshooks::log on

        Enable VCS Hooks display on screen and into newly created file:
        % vcshooks::log on "vcshooks.log"

        Enable VCS hooks display on screen and append into file:
        % vcshooks::log on "vcshooks.log" "a"
    }

    shell::autocomplete::addScript {
        vcshooks::log
    } {
        switch [llength $PARAMS] {
            0 {
                shell::autocomplete::filterList {on off} $PREFIX*
            }
            1 {
                list
            }
            2 {
                shell::autocomplete::filterList {w wb a ab} $PREFIX*
            }
            default {
                list $PREFIX
            }
        }
    }

} ; # namespace eval mdbg