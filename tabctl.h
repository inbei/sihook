#ifndef __TABCTL__H__
#define __TABCTL__H__

#include <windows.h>
#include <windef.h>
#include <commctrl.h>
#include <wingdi.h>


//创建TabCtl
void SiTabCtl_Create(HWND parent);
//获取页个数
int SiTabCtl_GetItemCount(void);
//获取行数
int SiTabCtl_GetRowCount(void);
//获取高度
int SiTabCtl_GetHeight(void);
//添加Item
void SiTabCtl_AddItem(char* title,HWND hwnd);
//查找item
int SiTabCtl_FindItem(HWND hwnd);
//删除Item
void SiTabCtl_DelItem(HWND hwnd);
//设置Item文字
void SiTabCtl_SetItemText(HWND hwnd,char* text);
//选中Item
void SiTabCtl_SetCurItem(HWND hwnd);

//获取当前Item
int SiTabCtl_GetCurItem(void);
//判断是否行数改变
BOOL SiTabCtl_IsRowChanged(void);
//位置改变
void SiTabCtl_OnPosChanging(WINDOWPOS* pos);
//选项卡改变
void SiTabCtl_OnSelChange(void);
//双击选项卡
//void SiTabCtl_OnLButtonDblClk(void);

//单击选项卡
void SiTabCtl_OnLButtonClk(void);
//重绘选项卡
void SiTabCtl_OnDrawItem(DRAWITEMSTRUCT* item);
//关闭当前
void SiTabCtl_CloseCurrentItem(void);
//关闭所有
void SiTabCtl_CloseAllItem(void);

//关闭其他
void SiTabCtl_CloseOtherItem(void);

//得到Item文件路径
char* SiTabCtl_GetItemFilePath(int index, char* path);

//从资源管理器中打开文件
void SiTabCtl_OpenFromExplorer(void);

//利用工具对代码风格进行修改
void SiTabCtl_FixCodeStyle(void);

//添加菜单
void SiTabCtl_AddPopupMenu(POINT pt);

//得到Item标题
void SiTabCtl_GetItemTitle(int index, char *text);

//得到Item HWND
HWND SiTabCtl_GetItemHwnd(int index);

//得到鼠标当前所在Item
int SiTabCtl_GetCursorIndex(void);

//设置鼠标当前Item为活动Item
void SiTabCtl_SetCursorItem(void);

//将文本添加到剪切板
void SiTabCtl_SetTextToClipboard(const char *source);

//添加文件头
void SiTabCtl_AddFileHeader(HWND hwnd);
//添加函数头
void SiTabCtl_AddFunctionContent(HWND hwnd);

//得到三角形轨迹
void SiTabCtl_GetEditPoints(POINT pt[], RECT rect, int dest);
void SiTabCtl_GetItemPoints(POINT pt[], RECT rect, int line);

//得到选项卡名称与类型
int SiTabCtl_GetDrawTitleType(int index, char *title);

//添加bug修订记录
void SiTabCtl_AddBugContent(HWND hwnd);

#endif

