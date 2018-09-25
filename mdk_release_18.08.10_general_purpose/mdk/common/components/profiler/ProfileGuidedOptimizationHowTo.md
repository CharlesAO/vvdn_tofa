# Movidius Profile Guided Optimization

## What is

Profile guided optimization is a technique used for providing information to the compiler
in order to generate better code. The data is gathered at runtime using an instrumented
version of the program, saved in a file on disk. This file is feeded back to the compiler
during next compile generating a binary optimized to have the best runtime in the tested
use case

## Prerequisites

- A project needs speed improvement
- MDK
- Tools

## Setup

- [] Make sure crtinit is called for shaves
- [] Add in Makefile in MVLIBOPT \_EP\_crtinit \_EP\_setheap \_EP\_crtfini 
- [] Set FS\_TOTAL\_SIZE= depending on the project size. At the end of the profiling session
you can see the amount of memory used for fine tuning
- [] Set ENTRYPOINTS in Makefile the MVLIBOPT stuff???

## Interpreting the results
