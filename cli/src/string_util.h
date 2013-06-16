// Ŭnicode please 
#pragma once

#include "string_hash.h"

template<typename T=char>
struct StrID {
	typedef T value_type;

	StrID() : hash(0), str(nullptr), length(0) {}
	StrID(unsigned int hash, const T *str, int len)
		: hash(hash), str(str), length(len) {}
	unsigned int hash;
	const T *str;
	int length;

	bool operator==(const StrID &o) const
	{
		return this->hash == o.hash;
	}
	bool operator!=(const StrID &o) const
	{
		return !(*this == o);
	}

	static_assert(std::is_same<char, T>::value == 1 || std::is_same<wchar_t, T>::value == 1, "not valid sid type");
};

template<typename T>
struct StringHolder {};
template<> struct StringHolder<char> { 
	typedef std::string value_type; 
	typedef std::ostringstream stringstream_type;
};
template<> struct StringHolder<wchar_t> { 
	typedef std::wstring value_type; 
	typedef std::wostringstream stringstream_type;
};

template<typename T>
class StringTable {
public:
	typedef typename T elem_type;
	typedef typename StringHolder<T>::value_type string_type;
	typedef CRC32 HashFunc;
	typedef std::unordered_map<unsigned int, StrID<elem_type>* > TableType;

	static_assert(std::is_same<char, T>::value == 1
		|| std::is_same<wchar_t, T>::value == 1,
		"not valid string table type");

public:
	StringTable() {}
	~StringTable() { clear(); }

	StrID<elem_type> get(const elem_type *str, int len)
	{
		unsigned int hash = HashFunc::hash(str, len);
		auto found = table_.find(hash);
		if(found != table_.end()) {
			StrID<elem_type> sid = *found->second;
			return sid;
		}
		// 문자열은 복사해서 추가로 생성한다. 이렇게하면
		// 모든걸 이쪽 클래스로 떠넘길수 있다.
		// 여기에서 할당된 메모리는 소멸자에서 적절히 처리하면 메모리 누수는 없다
		const elem_type *new_str = new elem_type[len];
		StrID<elem_type> *sid = new StrID<elem_type>(hash, new_str, len);
		table_[hash] = sid;
		return *sid;
	}

	StrID<elem_type> get(const string_type &str)
	{
		return get(str.data(), str.length());
	}

	bool clear()
	{
		for(auto it : table_) {
			StrID<elem_type> *sid = it.second;
			delete[](sid->str);
			delete(sid);
		}
		table_.clear();
		return true;
	}

	static StringTable &global()
	{
		static StringTable ctx;
		return ctx;
	}

private:
	TableType table_;
};

class StringUtil {
public:
	template<typename T>
	static const T &whiteSpaceString(Loki::Type2Type<T>);
	static const std::string &whiteSpaceString(Loki::Type2Type<std::string>)
	{
		static std::string str("\t\n\x0b\x0c\r ");
		return str;
	}
	static const std::wstring &whiteSpaceString(Loki::Type2Type<std::wstring>)
	{
		static std::wstring str(L"\t\n\x0b\x0c\r ");
		return str;
	}

	template<typename T>
	static T trim(const T &str) 
	{
		using std::size_t;

		// do no use boost to compile speed + dependenty
		const typename T::value_type *whitespace = whiteSpaceString(Loki::Type2Type<T>()).data();

		size_t leftFound = str.find_first_not_of(whitespace);
		size_t rightFound = str.find_last_not_of(whitespace);

		if (leftFound == T::npos) {
			leftFound = 0;
		}
		if (rightFound == T::npos) {
			rightFound = str.length();
		}
		T result = str.substr(leftFound, rightFound-leftFound+1);
		return result;
	}

	template<typename T>
	static T leftTrim(const T &str) 
	{
		using std::size_t;
		const typename T::value_type *whitespace = whiteSpaceString(Loki::Type2Type<T>()).data();
		size_t n = str.find_first_not_of(whitespace);
		T left(n == T::npos ? str : str.substr(n, str.length()));
		return left;
	}

	template<typename T>
	static T rightTrim(const T &str) 
	{
		using std::size_t;
		const typename T::value_type *whitespace = whiteSpaceString(Loki::Type2Type<T>()).data();
		size_t n = str.find_last_not_of(whitespace);
		T right(n == T::npos ? str : str.substr(0, n + 1));
		return right;
	}

	template<typename T>
	static T join(const T &str, const std::vector<T> &tokenlist) 
	{
		typename typedef T::value_type elem_type;
		typename StringHolder<elem_type>::stringstream_type oss;
		for (size_t i = 0 ; i < tokenlist.size() ; i++) {
			oss << tokenlist[i];
			if (i != tokenlist.size() - 1) {
				oss << str;
			}
		}
		return oss.str();
	}

	template<typename T>
	static int split(const T &str, typename T::value_type ch, std::vector<T> *retval) 
	{
		//SR_ASSERT(retval->empty() == true);
		// if no delimeter
		if (str.length() == 0) {
			retval->push_back(str);
			return retval->size();
		}

		// simple impl
		T tmp_token;
		for (size_t i = 0 ; i < str.length() ; i++) {
			typename T::value_type str_ch = str[i];
			if (str_ch == ch) {
				// split point
				retval->push_back(tmp_token);
				tmp_token.clear();
			} else {
				tmp_token.push_back(str_ch);
			}
		}
		retval->push_back(tmp_token);
		return retval->size();
	}

};
