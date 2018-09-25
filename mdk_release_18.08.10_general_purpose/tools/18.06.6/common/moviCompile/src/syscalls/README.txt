        Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
-----------------------------------------------------------------------------

The files in this directory 'common/moviCompile/src/syscalls' are the sources
for the collection of low-level system bindings for the C library.  The
majority of these are very trivial and have simple implementations which
allow a conforming C program to run correctly, although in many cases it will
get an ISO compliant result code informing the program that it was not
possible to satisfy the request.

For example, calling the ISO C function 'fopen' will return NULL indicating
that the function was unable to open the file requested, and setting 'errno'
to a meaningful value.  The reason for this is that the bare-metal SHAVE has
no operating system and no file system, so there is no way of satisfying the
request.

In this case the underlying low-level system binding routine that 'fopen'
ultimately calls, is:

    int _open(const char*, int, int);

and the implementation of this is in the accompanying source file:

    sys_open.c

The full set of low-level system bindings are included in this directory to
allow the system integrator to provide their own system specific
implementation for any of these calls.  The Movidius provided implementation
of these is precompiled into the 'mlibc.a' library.

The system integrator need only provide their own alternative implementation
of any of all of these system calls on the command-line to the linker as
discrete object files (.o files), and this will cause the linker to ignore
the default implementation that is present in these precompiled libraries.

It should be sufficient to simply compile the edited version of any of these
sources files with the default options to the compiler ('moviCompile') in
order to provide a valid alternative implementation.

Only the function implementation should be changed, as the provided header
file:

    shave_syscalls.h

establishes the appropriate context necessary to compile the implementation
of the system call.

