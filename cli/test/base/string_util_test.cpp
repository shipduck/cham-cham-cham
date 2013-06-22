// Ŭnicode please 
#include "stdafx.h"
#include "base/string_util.h"

using std::string;
using std::wstring;
using std::vector;

TEST(StrID, test)
{
	StrID<char> a;
	StrID<wchar_t> b;
	StrID<> c;
	//SID<int> c;
}
TEST(StringTable, Get)
{
	{
		StringTable<char> table;

		StrID<char> a = table.get("this is a");
		StrID<char> b = table.get("this is a", 9);
		StrID<char> c = table.get(std::string("this is a"));
		StrID<char> d = table.get("asddf");

		EXPECT_TRUE(a == b);
		EXPECT_TRUE(a == c);
		EXPECT_TRUE(a != d);
	}
	{
		StringTable<wchar_t> table;

		StrID<wchar_t> a = table.get(L"this is a");
		StrID<wchar_t> b = table.get(L"this is a", 9);
		StrID<wchar_t> c = table.get(std::wstring(L"this is a"));
		StrID<wchar_t> d = table.get(L"asddf");

		EXPECT_TRUE(a == b);
		EXPECT_TRUE(a == c);
		EXPECT_TRUE(a != d);
	}
}

TEST(StringUtil, WhiteSpaceString)
{
	string a = StringUtil::whiteSpaceString(Loki::Type2Type<std::string>());
	EXPECT_EQ(true, a.find(" ") != string::npos);
	wstring b = StringUtil::whiteSpaceString(Loki::Type2Type<std::wstring>());
	EXPECT_EQ(true, b.find(L" ") != wstring::npos);
}

TEST(StringUtil, Trim) 
{
	{
		string a("as df");
		EXPECT_TRUE(StringUtil::trim(a) == "as df");
		string b("  as df");
		EXPECT_TRUE(StringUtil::trim(b) == "as df");
		string c("as df  ");
		EXPECT_TRUE(StringUtil::trim(c) == "as df");
		string d("  as df  ");
		EXPECT_TRUE(StringUtil::trim(d) == "as df");
	}
	{
		wstring a(L"as df");
		EXPECT_TRUE(StringUtil::trim(a) == L"as df");
		wstring b(L"  as df");
		EXPECT_TRUE(StringUtil::trim(b) == L"as df");
		wstring c(L"as df  ");
		EXPECT_TRUE(StringUtil::trim(c) == L"as df");
		wstring d(L"  as df  ");
		EXPECT_TRUE(StringUtil::trim(d) == L"as df");
	}
}

TEST(StringUtil, leftTrim) 
{
	string a("as df");
	EXPECT_TRUE(StringUtil::leftTrim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(StringUtil::leftTrim(b) == "as df");

	wstring c(L"as df  ");
	EXPECT_TRUE(StringUtil::leftTrim(c) == L"as df  ");
	wstring d(L"  as df  ");
	EXPECT_TRUE(StringUtil::leftTrim(d) == L"as df  ");
}

TEST(StringUtil, rightTrim) 
{
	string a("as df");
	EXPECT_TRUE(StringUtil::rightTrim(a) == "as df");
	string b("  as df");
	EXPECT_TRUE(StringUtil::rightTrim(b) == "  as df");

	wstring c(L"as df  ");
	EXPECT_TRUE(StringUtil::rightTrim(c) == L"as df");
	wstring d(L"  as df  ");
	EXPECT_TRUE(StringUtil::rightTrim(d) == L"  as df");
}


TEST(StringUtil, split) 
{
	vector<string> result;
	int retval;

	//없는거 쪼개기 시도
	string test0("kldp.org");
	retval = StringUtil::split(test0, 'z', &result);
	EXPECT_EQ(1, retval);

	//1글자로 자르기
	string test1("kldp.org/node/11");
	result.clear();
	retval = StringUtil::split(test1, '/', &result);
	EXPECT_EQ(3, retval);
	ASSERT_EQ(3, (int)result.size());
	EXPECT_TRUE(result[0] == "kldp.org");
	EXPECT_TRUE(result[1] == "node");
	EXPECT_TRUE(result[2] == "11");	
}

TEST(StringUtil, join) 
{
	{
		string delim("::");
		string result;

		vector<string> strlist0;
		strlist0.push_back("a");
		strlist0.push_back("b");
		strlist0.push_back("c");
		result = StringUtil::join(delim, strlist0);
		EXPECT_TRUE(result == "a::b::c");
	}
	{
		wstring delim(L"-");
		wstring result;

		vector<wstring> strlist0;
		strlist0.push_back(L"a");
		strlist0.push_back(L"b");
		strlist0.push_back(L"c");
		result = StringUtil::join(delim, strlist0);
		EXPECT_TRUE(result == L"a-b-c");
	}
}
