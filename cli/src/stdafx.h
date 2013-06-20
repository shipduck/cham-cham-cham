﻿// Ŭnicode please 
#include "base/arch.h"
#include "base/assert_inc.h"

#if SR_USE_PCH
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <climits>

#include <locale>   // wstring_convert
#include <codecvt>  // codecvt_utf8
#include <iostream> // cout
#include <string>   // stoi and u32string

#include <sstream>

#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <memory>

#include <unordered_map>
#include <unordered_set>

#include <type_traits>

#include "irrlicht.h"
#include <OVR.h>

#include "Typelist.h"
#include "HierarchyGenerators.h"

// for unit test
#include <gtest/gtest.h>
#else
#error "NEED PCH"
#endif