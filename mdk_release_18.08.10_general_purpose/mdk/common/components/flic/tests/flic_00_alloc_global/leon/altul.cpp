///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     new/delete without file info
///

void test1()
{
    int *ii = new int;
    *ii = 40;
    delete ii;
}