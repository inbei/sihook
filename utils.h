/************************************************************************
* Copyright(C),2010-2014, Shenzhen TYDIC Information Technology Co.,Ltd
* FileName: Utils.h  * 
* Author:
* Version: 1.0 
* CreateDate: 2014-11-25 17:52:20 
* Description:
* Finished:
* History:
*  1.Date:
*  Author:
*  Modification:
***********************************************************************/


#ifndef __UTILS__H__
#define __UTILS__H__

#include <windows.h>

//���������Ϣ
void WINAPI OutputDebugStringEx(LPCTSTR lpcFormatText, ...);

//��ȡ�̱���
char* GetSiSwTitle(const char* lpc,char* title, char* path);

//��ȡ�ļ�����
//0 ������Ĭ��
//1 *.c *.cpp *.cxx *.cs
//2 *.h *.hpp *.hxx *.inc
//3 *.txt *.text *.doc
//4 *.php *.php3 *.htm *.js
//5 *.pas *.vb *.pb *.vbs

int GetColorIndex(const char* filename);

char *Trim( char *String );

char* GetRealTime(char* vo_pBuffer);

void DrawGradientV(HDC hdc, COLORREF co1, COLORREF co2, RECT DrawRect );
void DrawGradientH(HDC hdc, COLORREF co1, COLORREF co2, RECT DrawRect );

//int GetExePathFromHWND(HWND hwnd, TCHAR* ExePath);

#endif
