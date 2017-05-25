
#include "Helloworld.h"
#include "resource.h"

#define MAX_LOADSTRING 100

//
XGuiHelloworld	g_Helloworld;


// 全局变量:
HINSTANCE	g_hInstance						= NULL;			// 当前实例
TCHAR		g_szWindowTitle[MAX_LOADSTRING]	= {0};			// 标题栏文本
TCHAR		g_szWindowClass[MAX_LOADSTRING]	= {0};			// 主窗口类名
HWND		g_hWnd							= NULL;
BOOL		g_bActive						= FALSE;

// 此代码模块中包含的函数的前向声明:
BOOL				InitInstance(HINSTANCE);
BOOL				ExitInstance();

ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	MainWindowProc(HWND, UINT, WPARAM, LPARAM);

VOID				MainRender();

//
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CoInitialize( NULL );

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, g_szWindowTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HELLOWORLD, g_szWindowClass, MAX_LOADSTRING);
		
	if(!g_Helloworld.Initialize())
	{
		return FALSE;
	}

	// 执行应用程序初始化:
	if (!InitInstance (hInstance))
	{
		return FALSE;
	}

	// 主消息循环:
	MSG msg = {0};

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE) || !g_bActive)
		{
			if(GetMessage(&msg, NULL, 0U, 0U))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				break;
			}
		}
		else
		{
			MainRender();
		}
	}

	//
	ExitInstance( );
	g_Helloworld.Release();

	//
	CoUninitialize( );
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MainWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOWORLD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_HELLOWORLD);
	wcex.lpszClassName	= g_szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


//
BOOL ExitInstance()
{
	g_Helloworld.DestroyRenderWindow();
	return TRUE;
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance)
{
	g_hInstance = hInstance; // 将实例句柄存储在全局变量中

	if(!MyRegisterClass(hInstance))
	{ return FALSE; }

	RECT	rect = {0, 0, 800, 600};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindow(g_szWindowClass, g_szWindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd){ return FALSE; }	
	
	if(!g_Helloworld.LoadRenderWindow(g_hWnd))
	{
		return FALSE;
	}


	ShowWindow(g_hWnd, SW_SHOWNORMAL);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		{	
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_ACTIVATE:
		{
			if(LOWORD(wParam) == WA_INACTIVE)
			{
				g_bActive	= FALSE;
			}
			else
			{
				g_bActive	= TRUE;
			}
			break;
		}
	case WM_SIZE:
		{
			g_Helloworld.ResizeRenderWindow(LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

//
VOID				MainRender()
{
	g_Helloworld.Rendering();
}
