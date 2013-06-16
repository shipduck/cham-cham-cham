// Ŭnicode please
#pragma once

#include <string>

template<typename T>
class CRC32Logic {
public:
    CRC32Logic() { generateTable(); }
    ~CRC32Logic() {}
	///@brief calculate the crc value
	unsigned int hash(const T *str, int len) const
	{
		unsigned int crc = 0xFFFFFFFF;
		for(int i = 0 ; i < len ; i++) {
			int ch = static_cast<int>(str[i]);
			crc = (crc>>8) ^ crc_table[ (crc^ch) & 0xFF ];
		}     
		return( crc^0xFFFFFFFF );
	}

private:
	///@brief build the crc table 
	void generateTable()
	{
		unsigned int crc, poly;
		int i, j;

		poly = 0xEDB88320L;
		for (i = 0; i < 256; i++) {
			crc = i;
			for (j = 8; j > 0; j--) {
				if (crc & 1) {
					crc = (crc >> 1) ^ poly;
				} else {
					crc >>= 1;
				}
				crc_table[i] = crc;
			}
		}
	}
	unsigned int crc_table[256];
};



struct CRC32 {
	template<typename T>
    static unsigned int hash(const T *str, int len)
	{
		static CRC32Logic<T> logic;
		return logic.hash(str, len);
	}
};