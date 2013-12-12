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
		NULL,
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
			hdc = BeginPaint(hWnd, &ps);
			LOGBRUSH lb;
			lb.lbColor = RGB(0xff, 0x00, 0x00);
			lb.lbStyle = BS_SOLID;
			HBRUSH hbr = CreateBrushIndirect(&lb);
			RECT rc;
			GetClientRect(hWnd, &rc);
			FillRect(hdc, &rc, hbr);
			DeleteObject(hbr);
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
			RECT *rect = (LPRECT)lParam;
			rect->top += 0;
			rect->left += 8;
			rect->right -= 8;
			rect->bottom -= 8;
			return 0;
		}
	case WM_ERASEBKGND:
		{
			return 1;
		}
	case WM_NCPAINT:
		{
	//		//printf("%d WM_NCPAINT\n", nCount++);
			RECT rcWnd;
			RECT rcClient;
			HRGN nonClient, rgn;
			if (wParam == 1) {
				GetWindowRect(hWnd, &rcWnd);
				rgn = CreateRectRgnIndirect(&rcWnd);
			} else {
				rgn = reinterpret_cast<HRGN>(wParam);
				RECT rcbox;
				GetRgnBox(rgn, &rcbox);
				char buff[256];
				sprintf(buff, "rcbox (%d, %d, %d, %d)\n", rcbox.left, rcbox.top, rcbox.right, rcbox.bottom);
				OutputDebugStringA(buff);
			}
			GetClientRect(hWnd, &rcClient);
			MapWindowPoints(hWnd, NULL, reinterpret_cast<LPPOINT>(&rcClient), 2);
			rcClient.left -= rcWnd.left;
			rcClient.right -= rcWnd.left;
			rcClient.bottom -= rcWnd.top;
			rcClient.top -= rcWnd.top;

			rcWnd.right = rcWnd.right - rcWnd.left;
			rcWnd.bottom = rcWnd.bottom - rcWnd.top;
			rcWnd.left = 0;
			rcWnd.top = 0;
			rgn = CreateRectRgnIndirect(&rcWnd);
			nonClient = CreateRectRgnIndirect(&rcClient);
			CombineRgn(rgn, rgn, nonClient, RGN_DIFF);
			DeleteObject(nonClient);

			HDC hdc = GetWindowDC(hWnd);
			LOGBRUSH lb;
			lb.lbColor = RGB(0x00, 0xff, 0x00);
			lb.lbStyle = BS_SOLID;
			HBRUSH hbr = CreateBrushIndirect(&lb);
			SelectObject(hdc, hbr);
			PaintRgn(hdc, rgn);
			DeleteObject(hbr);
			FillRect(hdc, &rcWnd,  CreateBrushIndirect(&lb));
			ReleaseDC(hWnd, hdc);

			LRESULT ret = DefWindowProc(hWnd, WM_NCPAINT, reinterpret_cast<WPARAM>(rgn), lParam);

			if (rgn != reinterpret_cast<HRGN>(wParam))
				DeleteObject(rgn);
			return ret;
			//return 0;
	//		return DefWindowProc(hWnd, message, wParam, lParam);
		}
	case WM_SIZE:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			//printf("%d WM_SIZE\n", nCount++);
			break;
		}
	//case WM_ACTIVATE:
	//	{
	//		//printf("%d WM_ACTIVATE\n", nCount++);
	//		break;
	//	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}