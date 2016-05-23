#include "mdiclient.h"
#include "sisw.h"
#include "tabctl.h"
#include "const.h"

extern HWND hwnd_mdi_client;
static WNDPROC old_mdi_client_proc = NULL;
static WINDOWPOS last_windowpos;

//��ȡ���ھ��
HWND GetMdiClientHwnd(HWND parent)
{
	HWND find = NULL;

	find = FindWindowEx(parent,NULL,SI_CLASSNAME_MDI,NULL);

	return find;
}

//�õ�Edit���
HWND GetMdiClientEditHwnd(HWND MdiClient)
{
	HWND find = NULL;
	find = FindWindowEx(MdiClient,NULL,SI_CLASS_MDI_SIW,NULL);
	//if (find != NULL)
	//{
	//	find = FindWindowEx(find,NULL,SI_CLASS_MDI_SIW_SMW,NULL);
	//	//if (find != NULL){
	//	//	find = FindWindowEx(find,NULL,SI_CLASS_MDI_SIW_SMW_SEDIT,NULL);
	//	//}
	//}
	return find;
}

static LRESULT CALLBACK MdiClientSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS* pos = (WINDOWPOS*)lParam;
			SiTabCtl_OnPosChanging(pos);

			if(!(pos->flags & SWP_NOMOVE))
			{
				last_windowpos.x = pos->x;
				last_windowpos.y = pos->y;
			}
			if(!(pos->flags & SWP_NOSIZE))
			{
				last_windowpos.cx = pos->cx;
				last_windowpos.cy = pos->cy;
				pos->y += SiTabCtl_GetHeight();
				pos->cy -= SiTabCtl_GetHeight();
				lParam = (LPARAM)pos;
			}
		}
		break;
	default:
		break;
	}

	LRESULT lr = CallWindowProc(old_mdi_client_proc, hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_MDICREATE:
		{
			MDICREATESTRUCT* mdi = (MDICREATESTRUCT*)lParam;
			HWND hwnd = FindWindowEx(hwnd_mdi_client,NULL,mdi->szClass,mdi->szTitle);
			if(hwnd == NULL)
			{
				OutputDebugString("find si_Sw HWND failed");
				break;
			}
			HookSiSw(hwnd);
			char title[SI_BUF_SIZE];
			GetSiSwTitle(mdi->szTitle,title, NULL);
			SiTabCtl_AddItem(title,hwnd);
			SiTabCtl_SetCurItem(hwnd);
			//resize
			BOOL resize = SiTabCtl_IsRowChanged();
			if(resize == TRUE)
			{
				ResizeMdiClient();
			}
		}
		break;
	case WM_MDIDESTROY:
		{
			HWND hwnd = (HWND)wParam;
			SiTabCtl_DelItem(hwnd);
			UnhookSiSw(hwnd);
			//find new
			HWND curhwnd = (HWND)SendMessage(hwnd_mdi_client,WM_MDIGETACTIVE,0,0);
			if(curhwnd != NULL && IsWindow(curhwnd))
			{
				SiTabCtl_SetCurItem(curhwnd);
			}
			//resize
			BOOL resize = SiTabCtl_IsRowChanged();
			if(resize == TRUE)
			{
				ResizeMdiClient();
			}
		}
		break;
	case WM_MDIACTIVATE:
		{
			HWND hwnd = (HWND)wParam;
			SiTabCtl_SetCurItem(hwnd);
		}
		break;
	default:
		break;
	}

	return lr;
}

//���µ������ڴ�С
void ResizeMdiClient(void)
{
	//����MoveWindow�����ػ洰��
	MoveWindow(hwnd_mdi_client,last_windowpos.x,last_windowpos.y,last_windowpos.cx,last_windowpos.cy,TRUE);
}

//�滻������Ϣ
void HookMdiClient(void)
{
	old_mdi_client_proc = (WNDPROC)GetWindowLong(hwnd_mdi_client,GWL_WNDPROC);
	SetWindowLong(hwnd_mdi_client,GWL_WNDPROC,(DWORD)MdiClientSubClass);
}

