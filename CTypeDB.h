#pragma once
#include <vector>
#include <iostream>

struct SCharInfo
{
	CString m_char;
	int m_type;
	int m_sheet;
	int m_sx;
	int m_sy;
	int m_line;
	int m_order;
	int m_width;
	int m_height;
};

class CTypeDB
{
protected:
	int m_Sheet;	// 전차 장 수
	int m_nChar;	// 전체 글자 수
	CArray<SCharInfo, SCharInfo&> m_Chars;
	// std::vector<SCharInfo> m_Chars_v;
public:
	BOOL ReadCSVFILE(CString filename);
	void PrintInfo(int index);
	SCharInfo getChars(int index);
	int getLength(void);
	int getSheet(int index);
};

