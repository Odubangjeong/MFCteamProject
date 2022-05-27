#include "pch.h"
#include "CTypeDB.h"
#include <stdlib.h>

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
    }


    m_nChar = m_Chars.GetCount();
    return false;
}

void CTypeDB::PrintInfo(int index) {
    SCharInfo info;
    info = m_Chars.GetAt(index);

    CString str;
    str.Format(L"%d/%d height %d", index, m_nChar, info.m_height);
    AfxMessageBox(str);

    //TODO : 쓸거면 쓰기
}
