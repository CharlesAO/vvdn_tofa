/// =====================================================================================
///
///        @file:      main.cpp
///        @brief:     modular PC visualisation app for flic applications
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>             /* getopt_long() */

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Apps:
#include "demo1.h"
#include "simpleDisplayNv12.h"
#include "bitstrtofile.h"
#include "fe_hevc.h"

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

static void usage(FILE *fp, int argc, char **argv)
{
        fprintf(fp,
                 "Usage: %s [options]\n\n"
                 "Version 0.1\n"
                 "Options:\n"
                 "-h | --help          Print this message\n\n"
                 "-a | --app           app select\n"
                 "-x | --width         image width\n"
                 "-y | --height        image height\n"
                 "-m | --monotofile    write mono output to file\n"
                 "-c | --calib         read calib from file\n"
                 "",
                 argv[0]);
}

static const char short_options[] = "a:x:y:c:hm"; /// "d:h" :-need param
static const struct option
long_options[] = {
        { "app",        required_argument, NULL, 'a' },
        { "width",      required_argument, NULL, 'x' },
        { "height",     required_argument, NULL, 'y' },
        { "help",       no_argument,       NULL, 'h' },
        { "monotofile", no_argument,       NULL, 'm' },
        { "calib",      required_argument, NULL, 'c' },
        { 0, 0, 0, 0 }
};

int main(int argc, char **argv)
{
    unsigned app_to_run = 0;
    unsigned width = 0;
    unsigned height = 0;
    bool monotofile = false;
    char *calibfile = NULL;

    /// Parsing input parameters
    for (;;) 
    {
        int idx;
        int c;

        c = getopt_long(argc, argv,
                        short_options, long_options, &idx);

        if (-1 == c)
                break;

        switch (c) 
        {
            case 'h':
                usage(stdout, argc, argv);
                exit(EXIT_SUCCESS);
                break;
            case 'a':
                app_to_run = strtol(optarg, NULL, 0);
                printf ( "Configuring to run app %d\n", app_to_run );
                break;
            case 'x':
                width = strtol(optarg, NULL, 0);
                printf ( "Configured width = %d\n", width );
                break;
            case 'y':
                height = strtol(optarg, NULL, 0);
                printf ( "Configured height = %d\n", height );
            case 'c':
                calibfile = (char*)malloc(strlen(optarg));
                strcpy(calibfile, optarg);
                printf ( "Using calib file : %s\n", calibfile );
                break;
            case 'm':
                monotofile = true;
                break;
            default:
                usage(stderr, argc, argv);
                exit(EXIT_FAILURE);
                break;
        }
    }

    switch(app_to_run)
    {
        default:
        case 0:
            if((width == 0) || (height == 0))
            {
                printf ( "Resolution not set! Provide --width --height params!\n" );
                exit(EXIT_FAILURE);
            }
            simpleDisplayNv12(width, height, 0);
            break;
        case 1:
            demo1(calibfile);
            break;
        case 2:
            bitstrtofile(calibfile,monotofile);
            break;
    }

    exit(EXIT_SUCCESS);  // no errors
}
