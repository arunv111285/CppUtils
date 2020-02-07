#include "pch.h"
#include "CppUnitTest.h"
#include "..\ContainerRemove.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace av::utils;

TEST_CLASS(utContainerRemove)
{
public:
		
	//========================================================================================================================!
	TEST_METHOD(VectorValueErase)
	{
		std::vector<int> v;
		v.push_back(11);
		v.push_back(22);
		v.push_back(33);

		erase(v, 22);

		Assert::IsTrue(v.size() == 2, L"Unexpected size after removal!", LINE_INFO());

		Assert::IsTrue(v.end() == std::find(v.begin(), v.end(), 22), L"Removed element still present in vector!", LINE_INFO());
	}

	//========================================================================================================================!
	TEST_METHOD(VectorEraseIf)
	{
		std::vector<int> v;
		v.push_back(11);
		v.push_back(22);
		v.push_back(33);
		v.push_back(44);
		v.push_back(55);
		v.push_back(66);

		auto pred = [](int val)
		{
			return (0 == val % 2);
		};

		erase_if(v, pred);

		Assert::IsTrue(v.size() == 3, L"Unexpected size after removal!", LINE_INFO());

		Assert::IsTrue(v.end() == std::find_if(v.begin(), v.end(), pred), L"Removed elements still present in vector!", LINE_INFO());
	}

	//========================================================================================================================!
	TEST_METHOD(UnorderedMapValueErase)
	{
		std::unordered_map<int, std::string> m;
		m.insert({ 1, "Jack" });
		m.insert({ 3, "Back" });
		m.insert({ 2, "Smack" });
		m.insert({ 4, "Zack" });

		erase(m, 2);

		Assert::IsTrue(m.size() == 3, L"Unexpected size after removal!", LINE_INFO());

		Assert::IsTrue( m.end() == m.find(2), L"Removed element still present!", LINE_INFO());
	}

	//========================================================================================================================!
	TEST_METHOD(UnorderedMapEraseIf)
	{
		std::unordered_map<int, std::string> m;
		m.insert({ 1, "Jack" });
		m.insert({ 3, "Back" });
		m.insert({ 2, "Smack" });
		m.insert({ 4, "Zack" });

		auto pred = [](const std::pair<int, std::string> & val)
		{
			return (0 == val.first % 2);
		};

		erase_if(m, pred);

		Assert::IsTrue(m.size() == 2, L"Unexpected size after removal!", LINE_INFO());

		Assert::IsTrue(m.end() == std::find_if(m.begin(), m.end(), pred), L"Removed elements still present in vector!", LINE_INFO());
	}
};