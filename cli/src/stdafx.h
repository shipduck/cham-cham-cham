// Ŭnicode please 
#include "base/arch.h"
#include "base/assert_inc.h"

#if SR_USE_PCH
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <climits>

#include <typeinfo>

#include <locale>   // wstring_convert
#include <codecvt>  // codecvt_utf8
#include <iostream> // cout
#include <string>   // stoi and u32string

#include <sstream>

#include <array>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <memory>

#include <unordered_map>
#include <unordered_set>

#include <type_traits>

// irrlicht
#include "irrlicht.h"

// libovr
#include <OVR.h>

#include "AL/alut.h"
#if defined(__APPLE__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#elif defined(_MSC_VER)
#include <al.h>
#include <alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

// loki
#include "Typelist.h"
#include "HierarchyGenerators.h"

// boost
#include <boost/format.hpp>

// for unit test
#include <gtest/gtest.h>
#else
#error "NEED PCH"
#endif