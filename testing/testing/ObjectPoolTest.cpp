#include "ObjectPoolTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void ObjestPoolTest::setUp(){}
void ObjestPoolTest::tearDown(){}


void ObjestPoolTest::SimpleTest()
{
	//Assert::IsTrue() this validate that the given expresion return true 
	//if it return the test fails 
	//assert also provide function like is equal , is null , exceptException etc

	Assert::IsTrue(0 < 1);
}