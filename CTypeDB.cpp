#include "pch.h"
#include "CTypeDB.h"
#include <stdlib.h>
#include <string>

#include <vtkSTLReader.h>
#include <vtkLight.h>

BOOL CTypeDB::ReadCSVFILE(CString filename) {
    CStdioFile file;
    SCharInfo info;
    
    file.Open(filename, CFile::modeRead);

    CString line;

    file.ReadString(line); // 첫번째 줄은 속성 설명 => 제거

    while (file.ReadString(line)) { // 한줄씩 읽어오기
        if (line.IsEmpty()) // 마지막 줄에 엔터가 있을 수 있음
            continue;

        //',' 단위로 잘라서 각 속성 문자열 변수에 저장
        CString sChar, sType, sSheet, sSx, sSy, sLine, sOrder, sWidth, sHeight;
        int n = 0;
        AfxExtractSubString(sChar, line, n++, ',');
        AfxExtractSubString(sType, line, n++, ',');
        AfxExtractSubString(sSheet, line, n++, ',');
        AfxExtractSubString(sSx, line, n++, ',');
        AfxExtractSubString(sSy, line, n++, ',');
        AfxExtractSubString(sLine, line, n++, ',');
        AfxExtractSubString(sOrder, line, n++, ',');
        AfxExtractSubString(sWidth, line, n++, ',');
        AfxExtractSubString(sHeight, line, n++, ',');

        //int형인 속성들의 변환
        int cType, cSheet, cSx, cSy, cLine, cOrder, cWidth, cHeight;
        cType = _ttoi(sType);
        cSheet = _ttoi(sSheet);
        cSx = _ttoi(sSx);
        cSy = _ttoi(sSy);
        cLine = _ttoi(sLine);
        cOrder = _ttoi(sOrder);
        cWidth = _ttoi(sWidth);
        cHeight = _ttoi(sHeight);

        //구조체 생성
        info.m_char = sChar;
        info.m_type = cType;
        info.m_sheet = cSheet;
        info.m_sx = cSx;
        info.m_sy = cSy;
        info.m_line = cLine;
        info.m_order = cOrder;
        info.m_width = cWidth;
        info.m_height = cHeight;

        //배열에 보관
        m_Chars.Add(info);
       // m_Chars_v.push_back(info);
    }


   // m_nChar = m_Chars.GetCount();
    return false;
}

void CTypeDB::PrintInfo(int index) {
    SCharInfo info;
   // info = m_Chars.GetAt(index);

    CString str;
    str.Format(L"%d/%d height %d", index, m_nChar, info.m_height);
    AfxMessageBox(str);

    //TODO : 쓸거면 쓰기
}

// SCharInfo에서 i번째 m_Chars 주기

SCharInfo CTypeDB::getChars(int index) {
    SCharInfo _chars;
    _chars = m_Chars.GetAt(index);

    return _chars;
}
/*
SCharInfo CTypeDB::getChars(int index) {
    SCharInfo _chars;
    std::vector<SCharInfo>::iterator iter = m_Chars_v.begin();
    iter += index;
    _chars = *iter;

    return _chars;
}
*/

int CTypeDB::getLength(void) {
    int index;
    index = m_Chars.GetSize();
    
    return index;
}
/*
int CTypeDB::getLength(void) {
    int length = m_Chars_v.size();
    return length;
}
*/

// index에 해당하는 개체의 sheet 반환

int CTypeDB::getSheet(int index) {
    int sheet;
    SCharInfo _chars;
    _chars = m_Chars.GetAt(index);
    sheet = _chars.m_sheet;

    return sheet;
}
/*
int CTypeDB::getSheet(int index) {
    int sheet;
    SCharInfo _chars;
    std::vector<SCharInfo>::iterator iter = m_Chars_v.begin();
    iter += index;
    _chars = *iter;

    sheet = _chars.m_sheet;
    return sheet;
}
*/

void CTypeDB::numcount()
{
    int sheet1 = 0, sheet2 = 0, sheet3 = 0, skcount1 = 0, skcount2 = 0, skcount3 = 0, stcount1 = 0, stcount2 = 0, stcount3 = 0; // 1, 2, 3장 글자수 / 1, 2, 3장 종류 중복 수 / 1, 2, 3장 활자 중복 수
    int toverlapcount = 0, koverlapcount = 0; //전체 활자수, 전체 종류 중복 수

    //전체 글자 종류 + 장 내 글자수
    for (int i = 0; i < m_Chars.GetSize(); i++)
    {
        SCharInfo info;
        info = m_Chars.GetAt(i);

        if (info.m_sheet == 1)
            sheet1++; //1장 글자 수
        else if (info.m_sheet == 2)
            sheet2++; //2장 글자 수
        else if (info.m_sheet == 3)
            sheet3++; //3장 글자 수

        for (int j = i - 1; j >= 0; j--)
        {
            if (i == 0) break;

            SCharInfo info1;
            info1 = m_Chars.GetAt(j);
            if (info.m_char == info1.m_char)
            {
                koverlapcount++;
                break;
            }
        }
    }

    //활자 수 세기
    for (int i = 1; i < m_Chars.GetSize(); i++)
    {
        SCharInfo info;
        info = m_Chars.GetAt(i);

        for (int j = i - 1; j >= 0; j--)
        {
            SCharInfo info1;
            info1 = m_Chars.GetAt(j);
            if (info.m_char == info1.m_char && info.m_type == info1.m_type)
            {
                toverlapcount++;
                if (info1.m_sheet == 1)
                {
                    stcount1++;
                }
                else if (info1.m_sheet == 2)
                {
                    stcount2++;
                }
                else if (info1.m_sheet == 3)
                {
                    stcount3++;
                }
            }
        }
    }

    //장내 글자 종류
    for (int i = 1; i < m_Chars.GetSize(); i++)
    {
        SCharInfo info;
        info = m_Chars.GetAt(i);

        for (int j = i - 1; j >= 0; j--)
        {
            SCharInfo info1;
            info1 = m_Chars.GetAt(j);

            if (info.m_char == info1.m_char && info.m_sheet == info1.m_sheet)
            {
                if (info1.m_sheet == 1)
                    skcount1++;
                else if (info1.m_sheet == 2)
                    skcount2++;
                else if (info1.m_sheet == 3)
                    skcount3++;
                break;

            }
        }
    }

    num.Format(_T("%d"), m_Chars.GetSize());
    type.Format(_T("%d"), m_Chars.GetSize() - koverlapcount);
    hwall.Format(_T("%d"), m_Chars.GetSize() - toverlapcount);

    num1.Format(_T("%d"), sheet1);
    type1.Format(_T("%d"), sheet1 - skcount1);
    hwall1.Format(_T("%d"), sheet1 - stcount1);

    num2.Format(_T("%d"), sheet2);
    type2.Format(_T("%d"), sheet2 - skcount2);
    hwall2.Format(_T("%d"), sheet2 - stcount2);

    num3.Format(_T("%d"), sheet3);
    type3.Format(_T("%d"), sheet3 - skcount3);
    hwall3.Format(_T("%d"), sheet3 - stcount3);
}

void CTypeDB::pagechangenum(int img_index)
{
    if (img_index == 1)
    {
        CString num1, type1, hwall1;
        num1.Format(_T("%d"), sheet1);
        type1.Format(_T("%d"), sheet1 - skcount1);
        hwall1.Format(_T("%d"), sheet1 - stcount1);
    }
    else if (img_index == 2)
    {
        CString num2, type2, hall2, str, str1;
        num2.Format(_T("%d"), sheet2);
        type2.Format(_T("%d"), sheet2 - skcount2);
        hall2.Format(_T("%d"), sheet2 - stcount2);
    }
    else if (img_index == 3)
    {
        CString num3, type3, hall3;
        num3.Format(_T("%d"), sheet3);
        type3.Format(_T("%d"), sheet3 - skcount3);
        hall3.Format(_T("%d"), sheet3 - stcount3);
    }
}

/*void CTypeDB::displayVTK(CString path)
{
        // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
     
        vtkSmartPointer<vtkSTLReader> pSTLReader =
            vtkSmartPointer<vtkSTLReader>::New();
        pSTLReader->SetFileName(path); // 여기서 읽어오는 것임 PATH를 잘 설정해주면 된다.
        //pSTLReader->SetFileName("m_char + "_" + m_type");
        pSTLReader->Update();

        vtkSmartPointer<vtkPolyData> pPolyData =
            pSTLReader->GetOutput();

        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(pPolyData);
        vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(actor);
        renderer->SetBackground(.1, .2, .3);
        renderer->ResetCamera();


        m_vtkWindow->AddRenderer(renderer);
        m_vtkWindow->Render();
    
}*/