///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <svuCommonShave.h>

// 2: Source Specific #defines and types (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// This array is shared with the Leon
char shaveStr[250];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
static const char message[] = "Message from Shave's %s!\n";

class Test{
	private:
		char* 	str;
		char	name[32];

	public:
		Test(const char* name) {
			printf("\t%s object constructed on Shave at 0x%p\n", name, this);
			strcpy(this->name, name);

			str = new char[250];
			if (!str)
				printf("\t\tMemory could not be allocated on the Shave\n");
			else
				sprintf(str, message, name);
		}

		~Test()	{
			printf("\t%s object destructed on Shave\n", this->name);
			delete str;
		}
	
		void hello(void) {
			printf("\t\tHello from Shave's %s space\n", this->name);
			strcpy(shaveStr, str);
		}

};

Test gtest("Global");

__attribute__((dllexport)) int main( void )
{
    Test stest("Stack");
    
    gtest.hello();
    stest.hello();
	
    return 0;
}
