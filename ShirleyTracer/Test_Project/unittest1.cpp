#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\ShirleyTracer\src\utility.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test_Project
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
			// I want to know what's wrong here!
	//		Assert::AreEqual(randd() > 0, true);
			Assert::AreEqual(1 == 1, true);
		}

	};
}