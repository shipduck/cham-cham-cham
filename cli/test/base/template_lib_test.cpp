// Ŭnicode please 
#include "stdafx.h"
#include "base/template_lib.h"

using namespace std;
using namespace Loki;

class FooClass { };
class BarClass { };
typedef TYPELIST_2(FooClass, BarClass) FooBarTypeList;
class FooBarClass : public GenSimpleHierarchy<FooBarTypeList> { };

TEST(GenSimpleHierarchy, baseTest)
{
	static_assert(is_base_of<FooClass, FooBarClass>::value == 1, "");
	static_assert(is_base_of<BarClass, FooBarClass>::value == 1, "");
	static_assert(is_base_of<NullType, FooBarClass>::value == 0, "");
	static_assert(is_base_of<std::string, FooBarClass>::value == 0, "");
}