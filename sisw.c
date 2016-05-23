#include <string.h>
#include "tabctl.h"
#include "const.h"
#include "utils.h"
#include "sisw.h"

static WNDPROC old_si_sw_proc = NULL;

static LRESULT CALLBACK SiSwSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = CallWindowProc(old_si_sw_proc, hWnd, uMsg, wParam, lParam);
	
	switch(uMsg)
	{
	case WM_SETTEXT:
		{
			char text[SI_BUF_SIZE];
			GetSiSwTitle((LPCSTR)lParam,text, NULL);
			SiTabCtl_SetItemText(hWnd,text);
		}
		break;
	default:
		break;
	}

	return lr;
}

//��ȡsi_Sw���ڹ���
void HookSiSw(HWND hwnd)
{
	old_si_sw_proc = (WNDPROC)GetWindowLong(hwnd,GWL_WNDPROC);
	SetWindowLong(hwnd,GWL_WNDPROC,(DWORD)SiSwSubClass);
}

//�ָ�si_Sw���ڹ���
void UnhookSiSw(HWND hwnd)
{
	SetWindowLong(hwnd,GWL_WNDPROC,(DWORD)old_si_sw_proc);
}
