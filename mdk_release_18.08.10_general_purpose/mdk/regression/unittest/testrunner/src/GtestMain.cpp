#include "gtest/gtest.h"
#include "FunctionInfo.h"
#include "TestRunner.h"
#include "TestEventListener.h"

void processCommandLine(int argc, char **argv)
{
	FunctionInfo info = FunctionInfo::Instance();
	for (int i = 0; i < argc; i++)
	{
		// Cormac request: add command line use user Seed parameters
		if (strcmp(argv[i],"-srvIP:") == 0)
		{
			TestRunner::setSrvIP(argv[i+1]);
		}
		if (strcmp(argv[i],"-srvPort:") == 0)
		{
			TestRunner::setSrvPort(argv[i+1]);
		}
		if (strcmp(argv[i],"-useSeed:") == 0)
		{
			unsigned int userSeed = atoi(argv[i+1]);
			info.setUserSeed(userSeed);
			printf("seed set by user = %d \n", userSeed);
		}
		if (strcmp(argv[i],"-registersChecking") == 0)
		{
			TestRunner::setRegistersCheckingOn();
		}
		else if (strcmp(argv[i],"-debug") == 0)
		{
			TestRunner::setDebugOn();
		}
	}
}


int main(int argc, char **argv) {
	processCommandLine(argc, argv);
	InitGoogleTest(&argc, argv);
	UnitTest& unit_test = *UnitTest::GetInstance();
	TestEventListeners& listeners = unit_test.listeners();
	listeners.Append(new TestEventListener);
	int ret_val = RUN_ALL_TESTS();

	return ret_val;
}
