#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\ShirleyTracer\src\pdf.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test_Project
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: �ڴ�������Դ���
			Assert::AreEqual(randd() > 0, true);
			Assert::AreEqual(randd() < 1, true);
		}

	};
}