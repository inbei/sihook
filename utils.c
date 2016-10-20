#include <windows.h>
#include <windef.h>
#include <commctrl.h>
#include <wingdi.h>

#include <string.h>
#include <memory.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "utils.h"



#define ISSPACE(x)  ((x)==' '||(x)=='\r'||(x)=='\n'||(x)=='\f'||(x)=='\b'||(x)=='\t')
//设置渐变参数，GRADLEVEL越小，颜色渐变越细腻，过度效果越好，但速度比较慢
#define GRADLEVEL 1


//输出调试信息
void WINAPI OutputDebugStringEx(LPCTSTR lpcFormatText, ...)
{
    char szBuffer[1024];

    va_list argptr;
    va_start(argptr, lpcFormatText);
    wvsprintf(szBuffer, lpcFormatText, argptr);
    va_end(argptr);

    OutputDebugString(szBuffer);
}

//获取短标题
char* GetSiSwTitle(const char* lpc, char* title, char* path)
{
    int i, len;
    int skip_flag, skip_first;
    int index, pindex;
    skip_flag = skip_first= 0;
    i = index = pindex = 0;
    len = strlen(lpc);

    while(i < len) {
        if(lpc[i] == ')' && strrchr(lpc, ')') - lpc == i ) {
            skip_flag = 0;
            i++;
            continue;
        }

        if(skip_flag == 0) {
            title[index++] = lpc[i];
        }
        if(skip_flag == 1) {
            if(path != NULL)
                path[pindex++] = lpc[i];
        }

        if(lpc[i] == '(' && skip_first == 0) {
            index--;
            skip_flag = 1;
            skip_first++;
        }

        i++;
    }
    title[index] = '\0';
    if(path != NULL) {
        path[pindex] = '\0';
    }

    return title;
}

static const char* strstri(const char* str, const char* subStr)
{
    int len = strlen(subStr);
    if(len == 0)
    {
        return NULL;
    }

    while(*str)
    {
        if(strncasecmp(str, subStr, len) == 0)
        {
            return str;
        }
        ++str;
    }
    return NULL;
}

//获取文件类型
//0 无类型默认
//1 *.c *.cpp *.cxx *.cs
//2 *.h *.hpp *.hxx *.inc
//3 *.txt *.text *.doc
//4 *.php *.php3 *.htm *.js
//5 *.pas *.vb *.pb *.vbs
int GetColorIndex(const char* filename)
{
    const char *p = NULL;
    //type 1
    p = strstri(filename, ".c");
    if(p != NULL)
        return 1;
    p = strstri(filename, ".cpp");
    if(p != NULL)
        return 1;
    p = strstri(filename, ".cxx");
    if(p != NULL)
        return 1;
    p = strstri(filename, ".cs");
    if(p != NULL)
        return 1;
    p = strstri(filename, ".pc");
    if(p != NULL)
        return 1;
    //type 2
    p = strstri(filename, ".h");
    if(p != NULL)
        return 2;
    p = strstri(filename, ".hpp");
    if(p != NULL)
        return 2;
    p = strstri(filename, ".hxx");
    if(p != NULL)
        return 2;
    p = strstri(filename, ".inc");
    if(p != NULL)
        return 2;
    //type 3
    p = strstri(filename, ".txt");
    if(p != NULL)
        return 3;
    p = strstri(filename, ".text");
    if(p != NULL)
        return 3;
    p = strstri(filename, ".doc");
    if(p != NULL)
        return 3;
    //type 4
    p = strstri(filename, ".php");
    if(p != NULL)
        return 4;
    p = strstri(filename, ".php3");
    if(p != NULL)
        return 4;
    p = strstri(filename, ".htm");
    if(p != NULL)
        return 4;
    p = strstri(filename, ".js");
    if(p != NULL)
        return 4;
    //type 5
    p = strstri(filename, ".pas");
    if(p != NULL)
        return 5;
    p = strstri(filename, ".pb");
    if(p != NULL)
        return 5;
    p = strstri(filename, ".vbs");
    if(p != NULL)
        return 5;
    p = strstri(filename, ".vb");
    if(p != NULL)
        return 5;
    return 0;
}

char *Trim( char *String )
{
    char *Tail, *Head;

    for ( Tail = String + strlen( String ) - 1; Tail >= String; Tail -- )
        if ( !ISSPACE( *Tail ) )
            break;
    Tail[1] = 0;
    for ( Head = String; Head <= Tail; Head ++ )
        if ( !ISSPACE( *Head ) )
            break;
    if ( Head != String )
        memcpy( String, Head, ( Tail - Head + 2 ) * sizeof( char ) );
    return String;
}

char* GetRealTime(char* vo_pBuffer)
{
    struct timeval tv = {0};
    struct timezone tz = {0};
    struct tm *p = NULL;
    gettimeofday (&tv, &tz);
    time_t cur = tv.tv_sec;
    p = localtime(&cur);
    wsprintfA(vo_pBuffer, "%04d-%02d-%02d %02d:%02d:%02d",
              (1900 + p->tm_year),
              (1 + p->tm_mon),
              p->tm_mday,
              p->tm_hour,
              p->tm_min,
              p->tm_sec);

    return vo_pBuffer;
}



/****************************************************************************
绘制垂直颜色渐变区域
DrawGradientV(HDC hdc            //绘图刷子
                COLORREF co1    //顶端颜色
                COLORREF co2    //低端颜色
                RECT& DrawRect)    //颜色渐变区域
****************************************************************************/
void DrawGradientV(HDC hdc, COLORREF co1, COLORREF co2, RECT DrawRect )
{
    int r = GetRValue(co1);
    int g = GetGValue(co1);
    int b = GetBValue(co1);

    int r2 = GetRValue(co2);
    int g2 = GetGValue(co2);
    int b2 = GetBValue(co2);

    //计算宽,高
    int DrawRectWidth = DrawRect.right - DrawRect.left;
    int DrawRectHeight = DrawRect.bottom - DrawRect.top;

    if ( DrawRectWidth <= 0)
        return;

    //初始化rect
    RECT rect = {0, 0, DrawRectWidth, GRADLEVEL};

    //准备GDI
    HDC hMemDC = CreateCompatibleDC(hdc);              //创建内存DC
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, DrawRectWidth, DrawRectHeight); //创建位图
    SelectObject(hMemDC, hBitmap);       //把位图选进内存DC
    HBRUSH hbr;

    int i = 0;
    for(i = DrawRectHeight; i > 0; i -= GRADLEVEL )
    {
        //创建刷子
        hbr = CreateSolidBrush( RGB( r, g, b ) );
        FillRect( hMemDC, &rect, hbr );
        DeleteObject( hbr );

        //改变小正方体的位置
        rect.top += GRADLEVEL;
        rect.bottom += GRADLEVEL;

        //判断小正方体是否超界
        if( rect.bottom > DrawRect.bottom )
            rect.bottom = DrawRect.bottom;

        //改变颜色
        r += ( r2 - r + i / 2 ) / i * GRADLEVEL;
        g += ( g2 - g + i / 2 ) / i * GRADLEVEL;
        b += ( b2 - b + i / 2 ) / i * GRADLEVEL;
    }

    //内存DC映射到屏幕DC
    BitBlt(hdc, DrawRect.left, DrawRect.top, DrawRectWidth, DrawRectHeight, hMemDC, 0, 0, SRCCOPY);

    //删除
    DeleteDC(hMemDC) ;
    DeleteObject(hBitmap);
}

/****************************************************************************
绘制水平颜色渐变区域
DrawGradientV(    HDC hdc            //绘图刷子
                COLORREF co1    //左端颜色
                COLORREF co2    //右端颜色
                RECT& DrawRect)    //颜色渐变区域
****************************************************************************/
void DrawGradientH( HDC hdc, COLORREF co1, COLORREF co2, RECT DrawRect )
{
    int r = GetRValue( co1 );
    int g = GetGValue( co1 );
    int b = GetBValue( co1 );

    int r2 = GetRValue( co2 );
    int g2 = GetGValue( co2 );
    int b2 = GetBValue( co2 );

    //计算宽,高
    int DrawRectWidth = DrawRect.right - DrawRect.left;
    int DrawRectHeight = DrawRect.bottom - DrawRect.top;

    if ( DrawRectHeight <= 0)
        return;

    //初始化rect
    RECT rect = {0, 0, GRADLEVEL, DrawRectHeight};

    //准备GDI
    HDC hMemDC = CreateCompatibleDC(hdc);              //创建内存DC
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, DrawRectWidth, DrawRectHeight); //创建位图
    SelectObject(hMemDC, hBitmap);       //把位图选进内存DC
    HBRUSH hbr;
    int i = 0;
    for(i = DrawRectWidth; i > 0; i -= GRADLEVEL )
    {
        //创建刷子
        hbr = CreateSolidBrush( RGB( r, g, b ) );
        FillRect( hMemDC, &rect, hbr );
        DeleteObject( hbr );

        //改变小正方体的位置
        rect.left += GRADLEVEL;
        rect.right += GRADLEVEL;

        //判断小正方体是否超界
        if( rect.right > DrawRect.right )
            rect.right = DrawRect.right;

        //改变颜色
        r += ( r2 - r + i / 2 ) / i * GRADLEVEL;
        g += ( g2 - g + i / 2 ) / i * GRADLEVEL;
        b += ( b2 - b + i / 2 ) / i * GRADLEVEL;
    }

    //内存DC映射到屏幕DC
    BitBlt(hdc, DrawRect.left, DrawRect.top, DrawRectWidth, DrawRectHeight, hMemDC, 0, 0, SRCCOPY);

    //删除
    DeleteDC(hMemDC) ;
    DeleteObject(hBitmap);
}




/**
* @Description  ##_Function_Description##
* @Input        ##[IN]_Input_Parameter_##
* @Output       ##[OUT]_Output_Parameter_##
* @Return       ##Return_Code_Description##
* @Others       ##__Other___Description__##
*/
/*
int GetExePathFromHWND(HWND hwnd, TCHAR* ExePath)
{
    TCHAR szPath[MAX_PATH];
    int ErrorCode = 0;
    DWORD idProcess;
    GetWindowThreadProcessId(hwnd, &idProcess);
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, idProcess );
    if( NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;
        if( EnumProcessModules( hProcess, &hMod, sizeof( hMod ), &cbNeeded ) )
        {
            DWORD dw = GetModuleFileNameExA( hProcess, hMod, szPath, MAX_PATH );
            if(dw > 0)
            {
                lstrcpy(ExePath, szPath);
            }
            else
            {
                ErrorCode = GetLastError();
            }
        }
        CloseHandle( hProcess );
    }

    return ErrorCode;
}
*/



