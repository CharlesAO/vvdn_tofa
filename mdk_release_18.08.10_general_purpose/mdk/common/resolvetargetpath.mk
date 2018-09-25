# when running under Cygwin, care must be taken to avoid giving moviTools
# Linux-style paths or even worse, mixed paths. Use this RESOLVE_TARGET_PATH
# macro where paths are involved.
ifndef UNAME
UNAME:=$(shell uname -o)
endif
ifeq ($(UNAME),Cygwin)
  RESOLVE_TARGET_PATH = `cygpath -aw $(1)`
else
  RESOLVE_TARGET_PATH = $(1)
endif
