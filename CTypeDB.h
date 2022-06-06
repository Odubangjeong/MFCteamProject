#pragma once
#include <vector>
#include <iostream>
#include <string>

using namespace std;

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
	//CString path;

};

class CTypeDB
{
	//int m_Sheet;	// 전차 장 수
	//int m_nChar;	// 전체 글자 수
	//CArray<SCharInfo, SCharInfo&> m_Chars;
	
	// std::vector<SCharInfo> m_Chars_v;
//VTK
public:
	/*void displayVTK(CString path);
	vtkSmartPointer<vtkRenderWindow> m_vtkWindow;
	void InitVtkWindow(void* hwnd);
	void ResizeVtkWindow();
	*/
public:
	int m_Sheet;	// 전차 장 수
	int m_nChar;	// 전체 글자 수
	CArray<SCharInfo, SCharInfo&> m_Chars;
	int sheet1 = 0, sheet2 = 0, sheet3 = 0, skcount1 = 0, skcount2 = 0, skcount3 = 0, stcount1 = 0, stcount2 = 0, stcount3 = 0; // 1, 2, 3장 글자수 / 1, 2, 3장 종류 중복 수 / 1, 2, 3장 활자 중복 수
	int toverlapcount = 0, koverlapcount = 0; //전체 활자수, 전체 종류 중복 수
	CString num, type, hwall, num1, type1, hwall1, num2, type2, hwall2, num3, type3, hwall3;
	BOOL ReadCSVFILE(CString filename);
	void PrintInfo(int index);
	SCharInfo getChars(int index);
	int getLength(void);
	int getSheet(int index);
	void numcount();
	void pagechangenum(int img_index);
};

