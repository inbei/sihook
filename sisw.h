#ifndef __SISW__H__
#define __SISW__H__

#include <windows.h>
//��ȡ�̱���
char* GetSiSwTitle(const char* lpc,char* title, char* path);
//��ȡsi_Sw���ڹ���
void HookSiSw(HWND hwnd);
//�ָ�si_Sw���ڹ���
void UnhookSiSw(HWND hwnd);

#endif
