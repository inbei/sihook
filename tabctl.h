#ifndef __TABCTL__H__
#define __TABCTL__H__

#include <windows.h>
#include <windef.h>
#include <commctrl.h>
#include <wingdi.h>


//����TabCtl
void SiTabCtl_Create(HWND parent);
//��ȡҳ����
int SiTabCtl_GetItemCount(void);
//��ȡ����
int SiTabCtl_GetRowCount(void);
//��ȡ�߶�
int SiTabCtl_GetHeight(void);
//���Item
void SiTabCtl_AddItem(char* title,HWND hwnd);
//����item
int SiTabCtl_FindItem(HWND hwnd);
//ɾ��Item
void SiTabCtl_DelItem(HWND hwnd);
//����Item����
void SiTabCtl_SetItemText(HWND hwnd,char* text);
//ѡ��Item
void SiTabCtl_SetCurItem(HWND hwnd);

//��ȡ��ǰItem
int SiTabCtl_GetCurItem(void);
//�ж��Ƿ������ı�
BOOL SiTabCtl_IsRowChanged(void);
//λ�øı�
void SiTabCtl_OnPosChanging(WINDOWPOS* pos);
//ѡ��ı�
void SiTabCtl_OnSelChange(void);
//˫��ѡ�
//void SiTabCtl_OnLButtonDblClk(void);

//����ѡ�
void SiTabCtl_OnLButtonClk(void);
//�ػ�ѡ�
void SiTabCtl_OnDrawItem(DRAWITEMSTRUCT* item);
//�رյ�ǰ
void SiTabCtl_CloseCurrentItem(void);
//�ر�����
void SiTabCtl_CloseAllItem(void);

//�ر�����
void SiTabCtl_CloseOtherItem(void);

//�õ�Item�ļ�·��
char* SiTabCtl_GetItemFilePath(int index, char* path);

//����Դ�������д��ļ�
void SiTabCtl_OpenFromExplorer(void);

//���ù��߶Դ���������޸�
void SiTabCtl_FixCodeStyle(void);

//��Ӳ˵�
void SiTabCtl_AddPopupMenu(POINT pt);

//�õ�Item����
void SiTabCtl_GetItemTitle(int index, char *text);

//�õ�Item HWND
HWND SiTabCtl_GetItemHwnd(int index);

//�õ���굱ǰ����Item
int SiTabCtl_GetCursorIndex(void);

//������굱ǰItemΪ�Item
void SiTabCtl_SetCursorItem(void);

//���ı���ӵ����а�
void SiTabCtl_SetTextToClipboard(const char *source);

//����ļ�ͷ
void SiTabCtl_AddFileHeader(HWND hwnd);
//��Ӻ���ͷ
void SiTabCtl_AddFunctionContent(HWND hwnd);

//�õ������ι켣
void SiTabCtl_GetEditPoints(POINT pt[], RECT rect, int dest);
void SiTabCtl_GetItemPoints(POINT pt[], RECT rect, int line);

//�õ�ѡ�����������
int SiTabCtl_GetDrawTitleType(int index, char *title);

//���bug�޶���¼
void SiTabCtl_AddBugContent(HWND hwnd);

#endif

