# Clean targets should systematically call this insted of directly invoking
# shell's rm, with the risk to bring havoc on the local computer if some
# variable gets mistakenly pointed to outside the mdk source tree

ifeq (,$(MV_COMMON_BASE))
$(error MV_COMMON_BASE is not defined!)
endif

-include $(MV_COMMON_BASE)/make/lib.mk

define SAFE_RM
	@export SHOW_RM="$(ECHO)"\
	THE_RM_ALLOWED_IN=`readlink -f $(MV_COMMON_BASE)`; \
	for d in $(1) ; do \
	[ "$${d##$$THE_RM_ALLOWED_IN}" != "$$THE_RM_PATH" ] && rm -rf $$d ; \
	[ "$$SHOW_RM" != "@" ] && echo "rm -rf $$d" || true ; \
	done ;

endef
