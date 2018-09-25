#include "warpFilterApiTest.h"
#include "stereoFilterApiTest.h"
#include "dpuFilterApiTest.h"
#include <string.h>

#ifndef WIN32
    const char* warpLib = "warpDll.so";
    const char* stereoLib = "stereoDll.so";
    const char* dpuLib = "dpuDll.so";
    const char* pathDelimiter = "/";
#else
    const char* warpLib = "warpDll";
    const char* stereoLib = "stereoDll";
    const char* dpuLib = "dpuDll";
    const char* pathDelimiter = "\\";
#endif

using namespace std;

void warpTest();
void stereoTest();
void dpuTest();

string librariesPath = "";
string ResourcesPath = "";

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("Libraries folder set by 'arg1' and resources folder set 'arg2' are defaulted to current folder\n\n");

        printf("Usage: moviFilterApiTests <arg1> <arg2>\n\n");
        printf("    add arg1 for libraries path\n");
        printf("    add arg2 for image resources path\n\n");
    }
    else
    {
        string arg1 = argv[1];
        librariesPath = arg1;
        printf(" Libraries Folder: %s\n    \n", librariesPath.c_str());

        string arg2 = argv[2];
        ResourcesPath = arg2;
        printf(" Resources Folder: %s\n\n", ResourcesPath.c_str());
    }

    printf("TESTING COMPUTER VISION API FOR PC SIMULATION\n\n");

    warpTest();

    printf("\n");

    stereoTest();

    printf("\n");

    dpuTest();

    printf("\n");

    return 0;
}

void warpTest()
{
    auto *warpApi = new WarpFilterApiTest();

    string fullPathToLib = librariesPath + pathDelimiter + warpLib;

    warpApi->loadLibrary(fullPathToLib.c_str());

    warpApi->testWarpApi();

    warpApi->unloadLibrary();

    delete warpApi;
}

void stereoTest()
{
    auto *stereoApi = new StereoFilterApiTest(ResourcesPath.c_str());

    string fullPathToLib = librariesPath + pathDelimiter + stereoLib;

    stereoApi->loadLibrary(fullPathToLib.c_str());

    stereoApi->testStereoApi();

    stereoApi->unloadLibrary();

    delete stereoApi;
}


void dpuTest()
{
    auto *dpuApi = new DpuFilterApiTest();

    string fullPathToLib = librariesPath + pathDelimiter + dpuLib;

    dpuApi->loadLibrary(fullPathToLib.c_str());

    dpuApi->testDpuApi();

    dpuApi->unloadLibrary();

    delete dpuApi;
}