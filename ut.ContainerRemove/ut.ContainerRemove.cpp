#include "pch.h"
#include "CppUnitTest.h"
#include "..\ContainerRemove.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace av::utils;

TEST_CLASS(utContainerRemove)
{
public:
		
	TEST_METHOD(VectorValueRemoval)
	{
		std::vector<int> v;
		v.push_back(11);
		v.push_back(22);
		v.push_back(33);

		erase(v, 22);

		Assert::IsTrue(v.size() == 2, L"Unexpected size after removal!", LINE_INFO());

		Assert::IsTrue(v.end() == std::find(v.begin(), v.end(), 22), L"Removed element still present in vector!", LINE_INFO());
	}
};