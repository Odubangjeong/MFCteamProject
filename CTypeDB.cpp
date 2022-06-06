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

    file.ReadString(line); // ù��° ���� �Ӽ� ���� => ����

    while (file.ReadString(line)) { // ���پ� �о����
        if (line.IsEmpty()) // ������ �ٿ� ���Ͱ� ���� �� ����
            continue;

        //',' ������ �߶� �� �Ӽ� ���ڿ� ������ ����
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

        //int���� �Ӽ����� ��ȯ
        int cType, cSheet, cSx, cSy, cLine, cOrder, cWidth, cHeight;
        cType = _ttoi(sType);
        cSheet = _ttoi(sSheet);
        cSx = _ttoi(sSx);
        cSy = _ttoi(sSy);
        cLine = _ttoi(sLine);
        cOrder = _ttoi(sOrder);
        cWidth = _ttoi(sWidth);
        cHeight = _ttoi(sHeight);

        //����ü ����
        info.m_char = sChar;
        info.m_type = cType;
        info.m_sheet = cSheet;
        info.m_sx = cSx;
        info.m_sy = cSy;
        info.m_line = cLine;
        info.m_order = cOrder;
        info.m_width = cWidth;
        info.m_height = cHeight;

        //�迭�� ����
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

    //TODO : ���Ÿ� ����
}

// SCharInfo���� i��° m_Chars �ֱ�

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

// index�� �ش��ϴ� ��ü�� sheet ��ȯ

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
    int sheet1 = 0, sheet2 = 0, sheet3 = 0, skcount1 = 0, skcount2 = 0, skcount3 = 0, stcount1 = 0, stcount2 = 0, stcount3 = 0; // 1, 2, 3�� ���ڼ� / 1, 2, 3�� ���� �ߺ� �� / 1, 2, 3�� Ȱ�� �ߺ� ��
    int toverlapcount = 0, koverlapcount = 0; //��ü Ȱ�ڼ�, ��ü ���� �ߺ� ��

    //��ü ���� ���� + �� �� ���ڼ�
    for (int i = 0; i < m_Chars.GetSize(); i++)
    {
        SCharInfo info;
        info = m_Chars.GetAt(i);

        if (info.m_sheet == 1)
            sheet1++; //1�� ���� ��
        else if (info.m_sheet == 2)
            sheet2++; //2�� ���� ��
        else if (info.m_sheet == 3)
            sheet3++; //3�� ���� ��

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

    //Ȱ�� �� ����
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

    //�峻 ���� ����
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
        // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
     
        vtkSmartPointer<vtkSTLReader> pSTLReader =
            vtkSmartPointer<vtkSTLReader>::New();
        pSTLReader->SetFileName(path); // ���⼭ �о���� ���� PATH�� �� �������ָ� �ȴ�.
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