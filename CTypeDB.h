#pragma once


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
	int m_Sheet;	// ���� �� ��
	int m_nChar;	// ��ü ���� ��
	int m_Chars;
	CArray<SCharInfo, SCharInfo&> m_Chars;
public:
	BOOL ReadCSVFILE(CString filename);
};

