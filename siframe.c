#include <string.h>
#include <stdio.h>
#include "siframe.h"
#include "tabctl.h"
#include "const.h"


extern HWND hwnd_si_frame;
extern HWND hwnd_tab_ctl;
static WNDPROC old_si_frame_proc = NULL;
int last_tab = -1;



static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam)
{
	DWORD processid = *((DWORD*)lparam);
	DWORD threadprocessid = 0;

	GetWindowThreadProcessId(hwnd,&threadprocessid);
	if(threadprocessid == processid && GetParent(hwnd) == NULL)
	{
		*((HWND*)lparam) = hwnd;
		return FALSE;
	}

	return TRUE;
}

//��ȡ���ھ��
HWND GetSiFrameHwnd(void)
{
	DWORD processid = GetCurrentProcessId();
	if(!EnumWindows(EnumWindowsProc,(LPARAM)&processid))
	{
		return (HWND)processid;
	}

	return NULL;
}

static LRESULT CALLBACK SiFrameSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    
	switch(uMsg)
	{
	case WM_SETTEXT:
		{
			char buf[SI_BUF_SIZE];
			strcpy(buf,(char*)lParam);
			strcat(buf,PLUGIN_TITLE);
			lParam = (LPARAM)buf;
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR hdr = (LPNMHDR)lParam;
			if(hdr->hwndFrom != hwnd_tab_ctl)
			{
				break;
			}
			if(hdr->code == TCN_SELCHANGE)
			{
				SiTabCtl_OnSelChange();
			}
			else if(hdr->code == NM_CLICK)
			{
				SiTabCtl_OnLButtonClk();
			}
			if(hdr->code == TCN_SELCHANGING)
			{
				last_tab = SiTabCtl_GetCurItem();
			}

		}
		break;
	case WM_DRAWITEM:
		{
			DRAWITEMSTRUCT* item = (DRAWITEMSTRUCT*)lParam;
            if (item->hwndItem != hwnd_tab_ctl)
                break;
            SiTabCtl_OnDrawItem(item);
			return 0;
		}
		break;
    
	case WM_RBUTTONDOWN:
		{
			SendMessage(hwnd_tab_ctl,WM_RBUTTONDOWN,(WPARAM)NULL,(LPARAM)NULL);
			return 0;
		}
		break;
	default:
		break;
	}


	LRESULT lr = CallWindowProc(old_si_frame_proc, hWnd, uMsg, wParam, lParam);
	return lr;
}

//�滻������Ϣ
void HookSiFrame(void)
{
	char text[256];
	memset(text,0,sizeof(text));
	GetWindowText(hwnd_si_frame,text,sizeof(text));
	strcat(text,PLUGIN_TITLE);
	SetWindowText(hwnd_si_frame,text);
	
	old_si_frame_proc = (WNDPROC)GetWindowLong(hwnd_si_frame,GWL_WNDPROC);
	SetWindowLong(hwnd_si_frame,GWL_WNDPROC,(DWORD)SiFrameSubClass);
	
	//create tabctl
	SiTabCtl_Create(hwnd_si_frame);	
}
