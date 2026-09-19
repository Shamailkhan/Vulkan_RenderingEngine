#pragma once
#include <CppUnitTest.h>

TEST_CLASS(ObjestPoolTest)
{

public:
	//can add an initialization or clean up function to implement something 
	// before and after the test running 
	TEST_CLASS_INITIALIZE(setUp);
	TEST_CLASS_CLEANUP(tearDown);
	TEST_METHOD(SimpleTest);

};