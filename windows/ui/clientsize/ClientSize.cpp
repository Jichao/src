#include <windows.h>
#include <tchar.h>
#include <stdio.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE	 hInst;
static TCHAR szAppName[] = TEXT ("non-client window") ;
static TCHAR szTitle[] = _T("non-client window");
static int	nTextButtonSpacing = 7;
static int	nWidth = 100;
static int  nCount = 0;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	AllocConsole();
	FILE *fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	MSG          msg ;
	WNDCLASS     wndclass ;
	HWND         hwnd ;

	hInst = hInstance;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	//wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	LOGBRUSH lb;
	lb.lbColor = RGB(0xff, 0x00, 0x00);
	lb.lbStyle = BS_SOLID;
	wndclass.hbrBackground = CreateBrushIndirect(&lb);
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}

	hwnd = CreateWindowEx(
		WS_EX_COMPOSITED,
		//NULL,
		szAppName,                  // window class name
		TEXT ("The Hello Program"), // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		CW_USEDEFAULT,              // initial x size
		CW_USEDEFAULT,              // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL) ;                     // creation parameters

	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}

	fclose(fp);
	return msg.wParam ;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		{
			printf("%d WM_PAINT\n", nCount++);
			hdc = BeginPaint(hWnd, &ps);
			TCHAR test[] = 
				TEXT("缩小缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子缩小客户区的例子客户区的例子");
			TextOut(hdc,0,0,test,lstrlen(test));
			EndPaint(hWnd, &ps);
			return 0;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	
	case WM_NCCALCSIZE: 
		{
			printf("%d WM_NCCALCSIZE\n", nCount++);
			RECT *rect = (LPRECT)lParam;
			if (wParam == TRUE)
			{
				InflateRect(rect, -50, -50);
				/*RECT dstRect = *(LPRECT)lParam;
				RECT srcRect = rect[1];
				rect[1] = dstRect;
				rect[2] = srcRect;*/

				return 0;
			}
			else
			{
				InflateRect(rect, -50, -50);
				return 0;
			}
		}

	case WM_ERASEBKGND:
		{
			printf("%d WM_ERASEBKGND called\n", nCount++);
			break;
		}

	case WM_NCPAINT:
		{
			printf("%d WM_NCPAINT\n", nCount++);
			HDC hdc = GetWindowDC(hWnd);
			RECT rcWnd;
			GetWindowRect(hWnd, &rcWnd);
			rcWnd.right -= rcWnd.left;
			rcWnd.bottom -= rcWnd.top;
			rcWnd.left = rcWnd.top = 0;
			LOGBRUSH lb;
			lb.lbColor = RGB(0x00, 0xff, 0x00);
			lb.lbStyle = BS_SOLID;
			
			FillRect(hdc, &rcWnd,  CreateBrushIndirect(&lb));
			break;
		}

	case WM_SIZE:
		{
			printf("%d WM_SIZE\n", nCount++);
			break;
		}

	case WM_ACTIVATE:
		{
			printf("%d WM_ACTIVATE\n", nCount++);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}