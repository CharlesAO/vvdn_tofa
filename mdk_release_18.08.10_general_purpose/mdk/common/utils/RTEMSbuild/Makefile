
.PHONY: help all rtems rtems-debug libbsd tests zip distclean rebuild distclean-rtems
help:
	@echo
	@echo "\t|--------------------------------------------------------------|"
	@echo "\t| Help                                                         |"
	@echo "\t| ----                                                         |"
	@echo "\t| make rtems      : Clone and build rtems                      |"
	@echo "\t| make rtems-debug: Clone and build rtems with debug enabled   |"
	@echo "\t| make libbsd     : Clone and build libbsd                     |"
	@echo "\t| make all        : Clone and build rtems and libbsd           |"
	@echo "\t| make tests      : Build rtems tests only                     |"
	@echo "\t|--------------------------------------------------------------|"
	@echo "\t| Utilities                                                    |"
	@echo "\t| ----                                                         |"
	@echo "\t| make zip        : Creates zip archives                       |"
	@echo "\t| make distclean  : Delete all staging area                    |"
	@echo "\t| make rebuild    : Allow RTEMS to be recompiled               |"
	@echo "\t| make distclean-rtems: Delete RTEMS related staging area      |"
	@echo "\t|--------------------------------------------------------------|"
	@echo

all:
	./automate.sh all

rtems:
	./automate.sh rtems

rtems-debug:
	./automate.sh rtems-debug

libbsd:
	./automate.sh libbsd

tests:
	./automate.sh tests

zip:
	./automate.sh zip

distclean:
	rm -rf StagingArea

rebuild:
	rm StagingArea/logs/rtems_complete

distclean-rtems:
	rm -rf StagingArea/output/ StagingArea/logs/ StagingArea/RTEMS/
