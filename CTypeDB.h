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
	//int m_Sheet;	// ���� �� ��
	//int m_nChar;	// ��ü ���� ��
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
	int m_Sheet;	// ���� �� ��
	int m_nChar;	// ��ü ���� ��
	CArray<SCharInfo, SCharInfo&> m_Chars;
	int sheet1 = 0, sheet2 = 0, sheet3 = 0, skcount1 = 0, skcount2 = 0, skcount3 = 0, stcount1 = 0, stcount2 = 0, stcount3 = 0; // 1, 2, 3�� ���ڼ� / 1, 2, 3�� ���� �ߺ� �� / 1, 2, 3�� Ȱ�� �ߺ� ��
	int toverlapcount = 0, koverlapcount = 0; //��ü Ȱ�ڼ�, ��ü ���� �ߺ� ��
	CString num, type, hwall, num1, type1, hwall1, num2, type2, hwall2, num3, type3, hwall3;
	BOOL ReadCSVFILE(CString filename);
	void PrintInfo(int index);
	SCharInfo getChars(int index);
	int getLength(void);
	int getSheet(int index);
	void numcount();
	void pagechangenum(int img_index);
};

