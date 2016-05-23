/************************************************************************
* Copyright(C),2010-2014, Shenzhen TYDIC Information Technology Co.,Ltd
* FileName: Tabctl.c
* Author:
* Version: 1.0
* CreateDate: 2014-11-25 17:52:09
* Description:
* Finished:
* History:
*  1.Date:
*  Author:
*  Modification:
***********************************************************************/


//for mingw
#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include "const.h"
#include "utils.h"
#include "tabctl.h"
#include "mdiclient.h"

//�Ƿ���ʾ�ر�����ѡ�
#define SHOW_CLOSS_ALL 0

#define CM_CLOSE_THIS     WM_USER+195
#define CM_CLOSE_OTHR     WM_USER+196
#define CM_CLOSE_ALLS     WM_USER+197
#define CM_OPEN_EXPER     WM_USER+198
#define CM_ABOUT           WM_USER+199
#define CM_COPY_CLIPBD    WM_USER+200
#define CM_INST_FILECONT  WM_USER+201
#define CM_INST_FUNCTOIN  WM_USER+202
#define CM_FIX_CODESTYLE  WM_USER+203
#define CM_OPEN_COMMD     WM_USER+204
#define CM_PATH_CLIPBD    WM_USER+205
#define CM_FILE_CLIPBD    WM_USER+206
#define CM_INST_BUGVIEW   WM_USER+207

//// 700 - 1000 �����������
#define CM_FILE_LIST  WM_USER+700
//// 700 - 1000 �����������
extern HWND hwnd_si_frame;
extern HWND hwnd_tab_ctl;
extern HWND hwnd_mdi_client;
static WNDPROC old_tab_ctl_proc = NULL;
static int last_row_count = 1;

extern int last_tab;

//��������
static void SiTabCtl_AddCloseItem(void);

static LRESULT CALLBACK TabCtlSubClass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lr = CallWindowProc(old_tab_ctl_proc, hWnd, uMsg, wParam, lParam);

    switch(uMsg)
    {
        //�м�
        case WM_MBUTTONUP:
        {
            SiTabCtl_SetCursorItem();
            //SiTabCtl_OpenFromExplorer();
            SiTabCtl_CloseCurrentItem();
        }
        break;
        //�Ҽ�����
        case WM_RBUTTONDOWN:
        {
            POINT ptPos;
            GetCursorPos(&ptPos);
            SiTabCtl_SetCursorItem();
            SiTabCtl_AddPopupMenu(ptPos);
            return 0;
        }
        break;
        //���˫��
        case WM_LBUTTONDBLCLK:
        {
            //�رյ�ǰ
            SiTabCtl_CloseCurrentItem();
        }
        break;
        case WM_COMMAND:
        {
            switch(wParam)
            {
                //�رյ�ǰ
                case CM_CLOSE_THIS:
                {
                    //SiTabCtl_OnLButtonDblClk();
                    SiTabCtl_CloseCurrentItem();
                    return 0;
                }
                //�ر�����
                case CM_CLOSE_OTHR:
                {
                    SiTabCtl_CloseOtherItem();
                    return 0;
                }
                case CM_OPEN_EXPER:
                {
                    SiTabCtl_OpenFromExplorer();
                    return 0;
                }
                break;
                //�ر�����
                case CM_CLOSE_ALLS:
                {
                    SiTabCtl_CloseAllItem();
                    return 0;
                }
                break;
                //������ǰ�ļ���
                case CM_COPY_CLIPBD:
                {
                    char source[256] = {0};
                    //�õ���ǰ Item ����
                    SiTabCtl_GetItemTitle(CURRITEM, source);
                    char *ptr = strstr(source, "*");
                    if (ptr) *ptr = '\0';
                    SiTabCtl_SetTextToClipboard(source);
                    SiTabCtl_SetTextToClipboard(source);
                    return 0;
                }
                break;
                //�����ļ�Ŀ¼
                case CM_PATH_CLIPBD:
                {
                    char path[MAX_PATH] = { 0 };
                    SiTabCtl_GetItemFilePath(CURRITEM, path);
                    SiTabCtl_SetTextToClipboard(path);
                    SiTabCtl_SetTextToClipboard(path);
                }
                break;
                //�����ļ�·��
                case CM_FILE_CLIPBD:
                {
                    char path[MAX_PATH] = { 0 };
                    char name[MAX_PATH] = { 0 };
                    SiTabCtl_GetItemFilePath(CURRITEM, path);
                    SiTabCtl_GetItemTitle(CURRITEM, name);

                    char *ptr = strstr(name, "*");
                    if (ptr) *ptr = '\0';

                    Trim(path);
                    lstrcat(path, "\\");
                    Trim(name);
                    lstrcat(path, name);
                    SiTabCtl_SetTextToClipboard(path);
                    SiTabCtl_SetTextToClipboard(path);
                }
                break;
                //�����д�Ŀ¼
                case CM_OPEN_COMMD:
                {
                    char path[MAX_PATH] = { 0 };
                    SiTabCtl_GetItemFilePath(CURRITEM, path);
                    Trim(path);
                    ShellExecute( NULL, "open", "cmd.exe", "", path, SW_SHOWNORMAL );
                }
                break;
                case CM_FIX_CODESTYLE:
                {
                    SiTabCtl_FixCodeStyle();
                    return 0;
                }
                break;
                case CM_INST_FILECONT:
                {
                    HWND hwnd = SiTabCtl_GetItemHwnd(CURRITEM);
                    SiTabCtl_AddFileHeader(hwnd);
                    return 0;
                }
                break;
                case CM_INST_FUNCTOIN:
                {
                    HWND hwnd = SiTabCtl_GetItemHwnd(CURRITEM);
                    SiTabCtl_AddFunctionContent(hwnd);
                    return 0;
                }
                break;
				case CM_INST_BUGVIEW:
				{
                    HWND hwnd = SiTabCtl_GetItemHwnd(CURRITEM);
                    SiTabCtl_AddBugContent(hwnd);
                    return 0;
                }
                break;
                case CM_ABOUT:
                {
                    MessageBox(hwnd_tab_ctl,
                               " ���ڱ����˵��: \n"
                               "       �����ΪSource Insight���Tabҳ."
                               " �� Sihook master�������޸� \n"
                               " ��ͨWindows��̵�ͬѧ��������Ҫ�޸ĺ�Ĵ��롣 \n"
                               " \n"
                               " ����QQȺ: 411142119"
                               , "��   �� - QQ: 411142119", MB_OK);
                    return 0;
                }
                break;
                default:
                {
                    //�Ӳ˵���ѡ���Ѵ��ļ�
                    int count = SiTabCtl_GetItemCount();
                    int index = CM_FILE_LIST;
                    index = wParam - index;
                    if ( index >= 0 && index <= count)
                    {
                        TabCtrl_SetCurSel(hwnd_tab_ctl, index);
                        SiTabCtl_OnSelChange();
                    }
                    return 0;
                }
                break;
            }
        }
        break;

        default:
            break;
    }

    return lr;
}


void SiTabCtl_AddFileHeader(HWND hwnd)
{
    char header[512] = { 0 };
    char FileName[256] = { 0 };
    SiTabCtl_GetItemTitle(CURRITEM, FileName);

    char NowTime[50] = { 0 };
    GetRealTime(NowTime);

    wsprintfA(header,
              "/************************************************************************\r\
 * Copyright(C),2010-2014, Shenzhen TYDIC Information Technology Co.,Ltd\r\
 * FileName: %s \r\
 * Author:\r\
 * Version: 1.0 \r\
 * CreateDate: %s \r\
 * Description:\r\
 * Finished:\r\
 * History:\r\
 *  1.Date:\r *\
  Author:\r *\
  Modification:\r\
************************************************************************/\r\r",
              FileName, NowTime);

    header[511] = '\0';
    int len = strlen(header);
    int index = 0;
    for(index = 0; index < len; index++)
    {
        if (header[index] == '*')
            SendMessage(hwnd, WM_IME_CHAR, (WPARAM)'*', (LPARAM)0);
        else
            SendMessage(hwnd, WM_IME_CHAR, (WPARAM)header[index], (LPARAM)0);
        if (header[index] == '\r')
        {
            SendMessage(hwnd, WM_IME_KEYDOWN, (WPARAM)VK_HOME, (LPARAM)0);
            SendMessage(hwnd, WM_IME_KEYUP, (WPARAM)VK_HOME, (LPARAM)0);
        }
    }
}

void SiTabCtl_AddFunctionContent(HWND hwnd)
{
    SendMessage(hwnd, WM_IME_CHAR, (WPARAM)'\r', (LPARAM)0);
    SendMessage(hwnd, WM_IME_CHAR, (WPARAM)'\r', (LPARAM)0);

    char content[512] = "/**\r * @Description  ##_Function_Description##\r * @Input        ##[IN]_Input_Parameter_##\r * @Output       ##[OUT]_Output_Parameter_##\r * @Return       ##Return_Code_Description##\r * @Others       ##__Other___Description__##\r */";
    int len = strlen(content);
    int index = 0;
    for(index = 0; index < len; index++)
    {
        if (content[index] == '*')
            SendMessage(hwnd, WM_IME_CHAR, (WPARAM)'*', (LPARAM)0);
        else
            SendMessage(hwnd, WM_IME_CHAR, (WPARAM)content[index], (LPARAM)0);
        if (content[index] == '\r')
        {
            SendMessage(hwnd, WM_IME_KEYDOWN, (WPARAM)VK_HOME, (LPARAM)0);
            SendMessage(hwnd, WM_IME_KEYUP, (WPARAM)VK_HOME, (LPARAM)0);
        }
    }
}



/**
* @Description  ##���Bug�޶���¼##
* @Input        ##[IN]##
* @Output       ##[OUT]##
* @Return       ## void ##
* @Others       ####
*/
void SiTabCtl_AddBugContent(HWND hwnd)
{
    char content[512] = "// #### VERSION: XXXX ## FOR: XXXXX ####";
    int len = strlen(content);
    int index = 0;
    for(index = 0; index < len; index++)
    {
        if (content[index] == '*')
            SendMessage(hwnd, WM_IME_CHAR, (WPARAM)'*', (LPARAM)0);
        else
            SendMessage(hwnd, WM_IME_CHAR, (WPARAM)content[index], (LPARAM)0);
        if (content[index] == '\r')
        {
            SendMessage(hwnd, WM_IME_KEYDOWN, (WPARAM)VK_HOME, (LPARAM)0);
            SendMessage(hwnd, WM_IME_KEYUP, (WPARAM)VK_HOME, (LPARAM)0);
        }
    }
}

void SiTabCtl_SetTextToClipboard(const char *source)
{
    if( OpenClipboard(NULL) )
    {
        char * buffer;
        EmptyClipboard();
        HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(source) + 1);
        buffer = (char*)GlobalLock(clipbuffer);
        strcpy(buffer, source);
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT, clipbuffer);
        CloseClipboard();
    }

    char title[256] = { 0 };
    if (OpenClipboard(NULL))
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        char* buffer = (char*)GlobalLock(hData);
        strcat(title, buffer);
        GlobalUnlock(hData);
        CloseClipboard();
    }

    OutputDebugStringEx("Clipboard texts : %s.", title);
}

//

/**
* @Description  ##_Function_Description##
* @Input        ##[IN]_Input_Parameter_##
* @Output       ##[OUT]_Output_Parameter_##
* @Return       ##Return_Code_Description##
* @Others       pt ������GetCursorPos�õ�
*/
int SiTabCtl_GetCursorIndex(void)
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hwnd_tab_ctl, &pt);

    TCHITTESTINFO info;
    info.pt.x   = pt.x;
    info.pt.y   = pt.y;
    info.flags = TCHT_NOWHERE;
    //index = TabCtrl_HitTest(hwnd_tab_ctl, &info);

    return TabCtrl_HitTest(hwnd_tab_ctl, &info);
}


/**
* @Description  ##_Function_Description##
* @Input        ##[IN]_Input_Parameter_##
* @Output       ##[OUT]_Output_Parameter_##
* @Return       ##Return_Code_Description##
* @Others       ##__Other___Description__##
*/
void SiTabCtl_SetCursorItem(void)
{
    int index = SiTabCtl_GetCursorIndex();
    int count = SiTabCtl_GetItemCount();
    if (index < 0)
        return;
    if (index == count)
        return;

    TabCtrl_SetCurSel(hwnd_tab_ctl, index);
    SiTabCtl_OnSelChange();
}

/**
* @Description  ##_Function_Description##
* @Input        ##[IN]_Input_Parameter_##
* @Output       ##[OUT]_Output_Parameter_##
* @Return       ##Return_Code_Description##
* @Others       ##__Other___Description__##
*/
void SiTabCtl_AddPopupMenu(POINT pt)
{
    int indexs = SiTabCtl_GetCursorIndex();
    int counts = SiTabCtl_GetItemCount();
    BOOL IsOnItem = (indexs >= 0) && (indexs != counts); //��Item��
    HMENU AMenu = CreatePopupMenu();
    if (IsOnItem)
    {
        AppendMenu(AMenu, MF_POPUP, CM_CLOSE_THIS, "�رյ�ǰ�ļ�");
        AppendMenu(AMenu, MF_POPUP, CM_CLOSE_OTHR, "�ر������ļ�");
        AppendMenu(AMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);
    }

    AppendMenu(AMenu, MF_POPUP, CM_CLOSE_ALLS, "�ر������ļ�");
    AppendMenu(AMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);

    if (IsOnItem)
    {
        AppendMenu(AMenu, MF_POPUP, CM_OPEN_EXPER, "������Ŀ¼(��Դ������)");
        AppendMenu(AMenu, MF_POPUP, CM_OPEN_COMMD, "�������ļ���(������)");
    }
    /////////////////////////////////////////////////////////////////////////////////

    HMENU SMenu = CreatePopupMenu();
    int count = SiTabCtl_GetItemCount();
    int index = 0;
    while(index < count)
    {
        char text[256] = { 0 };
        SiTabCtl_GetItemTitle(index, text);
        AppendMenu(SMenu, MF_POPUP, CM_FILE_LIST + index, text);
        index ++;
    }
    AppendMenu(AMenu, MF_POPUP, (UINT_PTR)SMenu, "���Ѵ��ļ���ѡ��");
    /////////////////////////////////////////////////////////////////////////////////
    if (IsOnItem)
    {
        AppendMenu(AMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);
        AppendMenu(AMenu, MF_POPUP, CM_COPY_CLIPBD, "�����ļ�����������");
        AppendMenu(AMenu, MF_POPUP, CM_PATH_CLIPBD, "�����ļ�Ŀ¼��������");
        AppendMenu(AMenu, MF_POPUP, CM_FILE_CLIPBD, "�����ļ�·����������");
        AppendMenu(AMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);

        AppendMenu(AMenu, MF_POPUP, CM_INST_FILECONT, "��ǰλ������ļ�ע��");
        AppendMenu(AMenu, MF_POPUP, CM_INST_FUNCTOIN, "��ǰλ����Ӻ���ע��");
		AppendMenu(AMenu, MF_POPUP, CM_INST_BUGVIEW,  "��ǰλ������޶�ע��");
        AppendMenu(AMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);
        AppendMenu(AMenu, MF_POPUP, CM_FIX_CODESTYLE, "�ļ������ʽ����");
        AppendMenu(AMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);
    }
    AppendMenu(AMenu, MF_POPUP, CM_ABOUT, "����(&About)");

    SetForegroundWindow(hwnd_tab_ctl);
    TrackPopupMenu(AMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd_tab_ctl, NULL); //�ڵ�ǰλ�õ���
}



/**
* @Description  ##_Function_Description##
* @Input        ##[IN]_Input_Parameter_##
* @Output       ##[OUT]_Output_Parameter_##
* @Return       ##Return_Code_Description##
* @Others       ##__Other___Description__##
*/
void SiTabCtl_Create(HWND parent)
{

#ifndef NULL_COLOR
    //ɫ�ʰ�
    hwnd_tab_ctl = CreateWindowEx(0, WC_TABCONTROL, "",
                                  WS_CHILD | WS_VISIBLE | TCS_SINGLELINE | \
                                  TCS_FLATBUTTONS | TCS_FORCELABELLEFT | TCS_OWNERDRAWFIXED,
                                  0, 0, 0, SI_TAB_HEIGHT, parent, NULL, NULL, NULL);
#else
    //�ذ�
    hwnd_tab_ctl = CreateWindowEx(0, WC_TABCONTROL, "",
                                  WS_CHILD | WS_VISIBLE | TCS_SINGLELINE | \
                                  TCS_FLATBUTTONS | TCS_FORCELABELLEFT,
                                  0, 0, 0, SI_TAB_HEIGHT, parent, NULL, NULL, NULL);
#endif

    HFONT font = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Consolas");

    SendMessage(hwnd_tab_ctl, WM_SETFONT, (WPARAM)font, MAKELPARAM(1, 0));


    DWORD dwStyle = GetWindowLong(hwnd_tab_ctl, GWL_STYLE);
    dwStyle &= ~WS_BORDER;
    dwStyle &= ~WS_CAPTION;
    dwStyle &= ~TCS_TABS;
    SetWindowLong(hwnd_tab_ctl, GWL_STYLE, dwStyle);


    //SiTabCtl_AddCloseItem();
    last_row_count = 1;
    old_tab_ctl_proc = (WNDPROC)GetWindowLong(hwnd_tab_ctl, GWL_WNDPROC);
    if(old_tab_ctl_proc == NULL)
    {
        OutputDebugString("old_tab_ctl_proc null!");
    }
    SetWindowLong(hwnd_tab_ctl, GWL_WNDPROC, (DWORD)TabCtlSubClass);
}

//��ȡҳ����
int SiTabCtl_GetItemCount(void)
{
    //�����رհ�ť
    if (SHOW_CLOSS_ALL)
    	return TabCtrl_GetItemCount(hwnd_tab_ctl) - 1;
	else
		return TabCtrl_GetItemCount(hwnd_tab_ctl);
}

//��ȡ����
int SiTabCtl_GetRowCount(void)
{
    int row = TabCtrl_GetRowCount(hwnd_tab_ctl);
    if(row == 0)
        row = 1;
    return row;
}

//��ȡ�߶�
int SiTabCtl_GetHeight(void)
{
    return (SiTabCtl_GetRowCount() * SI_TAB_HEIGHT);
}

//��ӹر�Item
static void SiTabCtl_AddCloseItem(void)
{
    RECT rect;

    TCITEM tci;
    memset(&tci, 0, sizeof(TCITEM));
    tci.mask = TCIF_TEXT;
    tci.pszText = "ȫ���ر�";
    TabCtrl_InsertItem(hwnd_tab_ctl, SI_TAB_CLOSE_IDX, &tci);

    //���ù̶��߶�
    SendMessage(hwnd_tab_ctl, TCM_GETITEMRECT, SI_TAB_CLOSE_IDX, (LPARAM)&rect);
    SendMessage(hwnd_tab_ctl, TCM_SETITEMSIZE, SI_TAB_CLOSE_IDX, MAKELPARAM(rect.right - rect.left + 1, SI_TAB_HEIGHT));
}

//���Item
void SiTabCtl_AddItem(char* title, HWND hwnd)
{
    char text[SI_BUF_SIZE] = " ";
    strcat(text, title);
    //����
    int idx = SiTabCtl_GetItemCount();
    RECT rect;
    TCITEM tci;
    memset(&tci, 0, sizeof(TCITEM));
    tci.mask = TCIF_TEXT | TCIF_PARAM;
    tci.pszText = (LPSTR)text;
    tci.lParam = (LPARAM)hwnd;
    TabCtrl_InsertItem(hwnd_tab_ctl, idx, &tci);
    //���ù̶��߶�
    SendMessage(hwnd_tab_ctl, TCM_GETITEMRECT, idx, (LPARAM)&rect);
    SendMessage(hwnd_tab_ctl, TCM_SETITEMSIZE, idx, MAKELPARAM(rect.right - rect.left + 10, SI_TAB_HEIGHT));
    //SendMessage(hwnd_tab_ctl,TCM_SETPADDING,idx,MAKELPARAM(10,10));
}

//����item
int SiTabCtl_FindItem(HWND hwnd)
{
    int rtv = -1;
    int i;
    int count = SiTabCtl_GetItemCount();
    TCITEM tci;
    for(i = 0; i < count; i++)
    {
        memset(&tci, 0, sizeof(TCITEM));
        tci.mask = TCIF_TEXT | TCIF_PARAM;
        TabCtrl_GetItem(hwnd_tab_ctl, i, &tci);
        if((HWND)tci.lParam == hwnd)
        {
            rtv = i;
            break;
        }
    }

    return rtv;
}

//ɾ��Item
void SiTabCtl_DelItem(HWND hwnd)
{
    int idx = SiTabCtl_FindItem(hwnd);
    if(idx == -1)
        return;
    TabCtrl_DeleteItem(hwnd_tab_ctl, idx);
}

//����Item����
void SiTabCtl_SetItemText(HWND hwnd, char* text)
{
    int idx = SiTabCtl_FindItem(hwnd);
    if(idx == -1)
        return;
    TCITEM tci;
    memset(&tci, 0, sizeof(TCITEM));
    tci.mask = TCIF_TEXT | TCIF_PARAM;
    TabCtrl_GetItem(hwnd_tab_ctl, idx, &tci);

    tci.pszText = (LPSTR)text;
    TabCtrl_SetItem(hwnd_tab_ctl, idx, &tci);
}

//ѡ��Item
void SiTabCtl_SetCurItem(HWND hwnd)
{
    int idx = SiTabCtl_FindItem(hwnd);
    if(idx == -1)
        return;
    TabCtrl_SetCurSel(hwnd_tab_ctl, idx);
}

//��ȡ��ǰItem
int SiTabCtl_GetCurItem(void)
{
    return TabCtrl_GetCurSel(hwnd_tab_ctl);
}

//�ж��Ƿ������ı�
BOOL SiTabCtl_IsRowChanged(void)
{
    int rtv = FALSE;
    int row = SiTabCtl_GetRowCount();
    if(last_row_count != row)
    {
        rtv = TRUE;
        last_row_count = row;
    }

    return rtv;
}

//λ�øı�
void SiTabCtl_OnPosChanging(WINDOWPOS* pos)
{
    if(!(pos->flags & SWP_NOSIZE))
    {
        int row;
        SetWindowPos(hwnd_tab_ctl, NULL, pos->x, pos->y, pos->cx, SI_TAB_HEIGHT, pos->flags);
        row = SiTabCtl_GetRowCount();
        SetWindowPos(hwnd_tab_ctl, NULL, 0, 0, pos->cx, SI_TAB_HEIGHT * row, SWP_NOMOVE);
    }
}

//��ǩѡ�иı�
void SiTabCtl_OnSelChange(void)
{
    int idx = SiTabCtl_GetCurItem();
    int count = SiTabCtl_GetItemCount();
    HWND hwnd = SiTabCtl_GetItemHwnd(idx);
    if (idx == count)
        SiTabCtl_CloseAllItem();
    last_tab = idx;
    SendMessage(GetParent(hwnd), WM_MDIACTIVATE, (WPARAM)hwnd, 0);
}

//˫��ѡ�
//�رյ�ǰ
//void SiTabCtl_OnLButtonDblClk(void)
void SiTabCtl_CloseCurrentItem(void)
{

    int idx = SiTabCtl_GetCurItem();
    if(idx == -1)
        return;
    HWND hwnd = SiTabCtl_GetItemHwnd(idx);
    SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
}

//����[ȫ���ر�]ѡ�
void SiTabCtl_OnLButtonClk(void)
{
    int idx = SiTabCtl_GetCurItem();
    int count = SiTabCtl_GetItemCount();

    if(idx != count)
        return;
    if(count == 0)
        return;

    SiTabCtl_CloseAllItem();
}


//�ر�����
void SiTabCtl_CloseAllItem(void)
{
    int count = SiTabCtl_GetItemCount();

    int rtv = MessageBox(hwnd_tab_ctl, "�Ƿ�ر����б�ǩ?", "�ر����б�ǩ", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
    if(rtv == IDCANCEL)
    {
        TabCtrl_SetCurSel(hwnd_tab_ctl, last_tab);
        return;
    }

    while(count > 0)
    {
        SendMessage(SiTabCtl_GetItemHwnd(count - 1), WM_SYSCOMMAND, SC_CLOSE, 0);
        count--;
    }
}

void SiTabCtl_FixCodeStyle(void)
{
    char FilePath[MAX_PATH] = { 0 };
    char FileName[MAX_PATH] = { 0 };
    char ExecPath[MAX_PATH + SI_BUF_SIZE]  = { 0 };
    char ExecFlag[SI_BUF_SIZE] = { 0 };
    SiTabCtl_GetItemFilePath(CURRITEM, FilePath);
    SiTabCtl_GetItemTitle(CURRITEM, FileName);
    Trim(FileName);
    char* ptr = strstr(FileName, "*");
    if (ptr) *ptr = '\0';
    Trim(FilePath);
    lstrcat(FilePath, "\\");
    lstrcat(FilePath, FileName);

    /*
    GetModuleFileName(NULL, ExecPath, MAX_PATH);
    OutputDebugStringEx(" Get Current Directory is : %s. ", ExecPath);
    ptr = strrchr(ExecPath, '\\');
    if (ptr) *(ptr+1) = '\0';
    OutputDebugStringEx(" Get Current Directory is : %s. ", ExecPath);
    */

    lstrcat(ExecPath, TEXT("AStyle.exe"));
    lstrcpy(ExecFlag, "--style=linux -s4 -A1 -S -N -L -m0 -M40 --pad-oper --suffix=none --convert-tabs ");
    lstrcat(ExecFlag, FilePath);


    ShellExecute( NULL, "open", ExecPath, ExecFlag, NULL, SW_SHOWNORMAL );
}

char* SiTabCtl_GetItemFilePath(int index, char* path)
{
    char title[SI_BUF_SIZE] = { 0 };
    char fileName[SI_BUF_SIZE] = { 0 };
    char filePath[SI_BUF_SIZE] = { 0 };

    if (index < 0)
        index = SiTabCtl_GetCurItem();
    HWND hwnd = SiTabCtl_GetItemHwnd(index);
    GetWindowText( hwnd, title, MAX_PATH )  ; // ��û���ڱ���

    if (strstr(title, "..."))
    {
        MessageBox(hwnd_tab_ctl, "������������ã�������ɺ�����:\n"
                   "�ڲ˵������ҵ�: options -> preferences -> Display\n"
                   "ȥ����ѡ:Trim long path names with ellipses.",
                   "���������", MB_OKCANCEL | MB_ICONERROR);
    }

    GetSiSwTitle(title, fileName, filePath);
    strcpy(path, filePath);

    path[strlen(filePath)] = '\0';
    return path;
}

void SiTabCtl_OpenFromExplorer(void)
{
    char FilePath[SI_BUF_SIZE] = { 0 };
    char FileName[SI_BUF_SIZE] = { 0 };
    char ExecFlag[SI_BUF_SIZE] = { 0 };

    SiTabCtl_GetItemFilePath(CURRITEM, FilePath);
    SiTabCtl_GetItemTitle(CURRITEM, FileName);

    char *ptr = strstr(FileName, "*");
    if (ptr)
        *ptr = '\0';

    strcat(FilePath, "\\");
    strcat(FilePath, FileName);
    Trim(FilePath);

    strcpy(ExecFlag, "/select, ");
    strcat(ExecFlag, FilePath);
    ShellExecute( NULL, "open", "explorer.exe", ExecFlag, NULL, SW_SHOWNORMAL );
}

//�ر�����
void SiTabCtl_CloseOtherItem(void)
{
    int index = 0;
    int idx = SiTabCtl_GetCurItem();
    int count = SiTabCtl_GetItemCount();
    HWND lists[count];

    while(index < count)
    {
        if (idx != index)
            lists[index] = SiTabCtl_GetItemHwnd(index);
        else
            lists[index] = 0;
        index ++;
    }
    index = 0;
    while(index < count)
    {
        if (lists[index] != 0)
            SendMessage(lists[index], WM_SYSCOMMAND, SC_CLOSE, 0);
        index ++;
    }
}

HWND SiTabCtl_GetItemHwnd(int index)
{
    HWND hwnd;
    TCITEM tci;
    if(index < 0)
        index = SiTabCtl_GetCurItem();

    memset(&tci, 0, sizeof(TCITEM));
    tci.mask = TCIF_TEXT | TCIF_PARAM;
    TabCtrl_GetItem(hwnd_tab_ctl, index, &tci);
    hwnd = (HWND)tci.lParam;

    return hwnd;
}

void SiTabCtl_GetItemTitle(int index, char *text)
{
    char title[MAX_PATH] = {0};
    if ( index < 0 )
        index = SiTabCtl_GetCurItem();

    HWND hwnd = SiTabCtl_GetItemHwnd(index);
    GetWindowText( hwnd, title, MAX_PATH )  ; // ��û���ڱ���
    GetSiSwTitle(title, text, (char *)NULL);
}

//��ɫ�б�
static COLORREF color_table[10] =
{
    RGB(255, 255, 255), //default
    RGB(197, 210, 222), //c
    RGB(210, 227, 178), //h
    RGB(219, 236, 249), //txt
    RGB(249, 179, 180), //js
    RGB(255, 241, 134), //vb
};

//��ɫ����
static COLORREF color_solution[3][2] =
{
    {
        RGB(246, 246, 246), //�����ɫ
        RGB(252, 252, 252), //��������
    },
    {
        RGB(201, 214, 224), //�Ǽ����ɫ
        RGB(201, 214, 204), //�Ǽ�������
    },
    {
        RGB(249, 145, 60),  //�༭��ɫ
        RGB(255, 241, 134), //�Ǳ༭��ɫ
    }
};

enum enum_ACTION
{
    ACTION = 0,
    UNACTION,
    EDIT
} ACTIONS;



/**
* @Description  ##�ػ�Item##
* @Input        ##[IN]##
* @Output       ##[OUT]##
* @Return       ####
* @Others       ####
*/
void SiTabCtl_OnDrawItem(DRAWITEMSTRUCT* item)
{
    HBRUSH Brush[3];   //��̬��ˢ
    HPEN   Pen[3];     //��̬��
    HBRUSH hBrushOld;
    HPEN   hPenOld;

    RECT   rect = item->rcItem;
    char   title[SI_BUF_SIZE]; //Item����
    int    dest = 12;
    int    line = 2;
    BOOL   isCurrentItem = ((SiTabCtl_GetCurItem() == item->itemID) && (item->itemID != SiTabCtl_GetItemCount()));

    POINT  editpt[4];
    int    type = SiTabCtl_GetDrawTitleType(item->itemID, title);
    SiTabCtl_GetEditPoints(editpt, rect, dest);
    POINT  itempt[5];
    SiTabCtl_GetItemPoints(itempt, rect, line);

#ifdef MULTI_COLOR
    //��� ��ˢ �� ����
    Brush[ACTION] =   CreateSolidBrush(RGB(246, 246, 246));
    Brush[UNACTION] = CreateSolidBrush(color_table[type]);
    Brush[EDIT] =     CreateSolidBrush(RGB(249, 145, 60));

    Pen[ACTION] =    CreatePen(PS_SOLID, 1, RGB(201, 214, 204));
    Pen[UNACTION] =  CreatePen(PS_SOLID, 1, RGB(182, 199, 186));
    Pen[EDIT] =      CreatePen(PS_SOLID, 1, RGB(201, 214, 204));
#else
    //��ˢ �� ����
    Brush[ACTION] =   CreateSolidBrush(color_solution[ACTION][0]);
    Brush[UNACTION] = CreateSolidBrush(color_solution[UNACTION][0]);
    Brush[EDIT] =     CreateSolidBrush(color_solution[EDIT][0]);

    Pen[ACTION] =    CreatePen(PS_SOLID, 1, color_solution[ACTION][1]);
    Pen[UNACTION] =  CreatePen(PS_SOLID, 1, color_solution[UNACTION][1]);
    Pen[EDIT] =      CreatePen(PS_SOLID, 1, color_solution[EDIT][1]);

#endif

    //���Ƶ�ɫ
    if (isCurrentItem)
    {
        hPenOld = SelectObject(item->hDC, Pen[ACTION]) ;
        hBrushOld = SelectObject(item->hDC, Brush[ACTION]) ;
        Polygon (item->hDC, itempt, 5) ;
        //FillRect(item->hDC,&item->rcItem,Brush[ACTION] );
    }
    else
    {
        hPenOld = SelectObject(item->hDC, Pen[UNACTION]) ;
        hBrushOld = SelectObject(item->hDC, Brush[UNACTION]) ;
        Polygon (item->hDC, itempt, 5) ;
        //FillRect(item->hDC,&item->rcItem,Brush[UNACTION] );
    }

    SelectObject (item->hDC , hPenOld);
    SelectObject (item->hDC , hBrushOld);

    //����������
    if (isCurrentItem)
    {
        SetBkMode(item->hDC, TRANSPARENT);
        hPenOld = SelectObject(item->hDC, Pen[EDIT]) ;
        hBrushOld = SelectObject(item->hDC, Brush[EDIT]) ;
        Polygon (item->hDC, editpt, 4) ;
    }
    SelectObject (item->hDC , hPenOld);
    SelectObject (item->hDC , hBrushOld);

    DeleteObject(Brush[ACTION]);
    DeleteObject(Brush[UNACTION]);
    DeleteObject(Brush[EDIT]);

    DeleteObject ( Pen[ACTION] );
    DeleteObject ( Pen[UNACTION] );
    DeleteObject ( Pen[EDIT] );

    //��������
    SetBkMode(item->hDC, TRANSPARENT);
    DrawText(item->hDC, title, strlen(title), &item->rcItem, DT_CENTER | DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    //���Ƶײ�����
    RECT rtBottom;
    GetClientRect(hwnd_tab_ctl, &rtBottom);
    rtBottom.top += SI_TAB_HEIGHT - 2;
    HDC hdcBline = GetDC(hwnd_tab_ctl);
    HBRUSH hbBLine = CreateSolidBrush(RGB(227, 227, 227));
    FillRect(hdcBline, &rtBottom, hbBLine);
    ReleaseDC(hwnd_tab_ctl, hdcBline);
}

void SiTabCtl_GetEditPoints(POINT pt[], RECT rect, int dest)
{
    pt[0].x = rect.left;
    pt[0].y = rect.top;
    pt[1].x = rect.left + dest;
    pt[1].y = rect.top;
    pt[2].x = rect.left;
    pt[2].y = rect.top  +  dest;
    pt[3].x = rect.left;
    pt[3].y = rect.top;
}

void SiTabCtl_GetItemPoints(POINT pt[], RECT rect, int line)
{
    pt[0].x = rect.left  + line;
    pt[0].y = rect.top + line;
    pt[1].x = rect.right - line - 1;
    pt[1].y = rect.top + line;
    pt[2].x = rect.right - line - 1;
    pt[2].y = rect.bottom;
    pt[3].x = rect.left  + line;
    pt[3].y = rect.bottom;
    pt[4].x = rect.left  + line;
    pt[4].y = rect.top + line;
}


int SiTabCtl_GetDrawTitleType(int index, char *title)
{
    //�õ��ļ���
    TCITEM tci;
    memset(&tci, 0, sizeof(TCITEM));
    tci.mask = TCIF_TEXT | TCIF_STATE;
    tci.pszText = (LPSTR)title;
    tci.cchTextMax = SI_BUF_SIZE;
    TabCtrl_GetItem(hwnd_tab_ctl, index, &tci);
    return GetColorIndex(title);
}

