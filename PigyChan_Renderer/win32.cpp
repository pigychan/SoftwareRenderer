#define _CRT_SECURE_NO_WARNINGS
#include "win32.h"
#include <assert.h>
#include <stdio.h>

window_t* window = NULL;

//char转Wchar函数
const wchar_t* CharToWchar(const char* ch) {
	const size_t len = strlen(ch) + 1;
	wchar_t* wch = new wchar_t[len];
	mbstowcs(wch, ch, len);
	return wch;
}

//窗口过程函数
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CLOSE:	//关闭信号
		window->is_close = 1;
		break;
	case WM_KEYDOWN:	//键盘按下信号
		window->keys[wParam & 511] = 1;	//TODO:
		break;
	case WM_KEYUP:	//键盘抬起信号
		window->keys[wParam & 511] = 0;
		break;
	case WM_LBUTTONDOWN:	//鼠标左键按下信号,获取此时的鼠标位置
		window->mouse_info.LeftButton_pos = get_mouse_pos();
		window->buttons[0] = 1;
		break;
	case WM_LBUTTONUP:		//鼠标左键抬起信号
		window->buttons[0] = 0;
		break;
	case WM_RBUTTONDOWN:	//鼠标右键按下信号，获取此时的鼠标位置
		window->mouse_info.RightButton_pos = get_mouse_pos();
		window->buttons[1] = 1;
		break;
	case  WM_RBUTTONUP:		//鼠标右键抬起信号
		window->buttons[1] = 0;
		break;
	case WM_MOUSEWHEEL:		//鼠标滚轮信号
		window->mouse_info.wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		break;
	default:
		//为没有处理的窗口消息提供默认处理
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;	//正常退出
}

//设计并注册窗口类
static void design_register_window_class() {
	ATOM atom;
	WNDCLASS wc = { 0 };
	wc.style = CS_BYTEALIGNCLIENT;			//窗口风格
	wc.lpfnWndProc = (WNDPROC)WndProc;		//回调函数
	wc.cbClsExtra = 0;	//紧跟在窗口类尾部的一块额外空间，不用则设为0
	wc.cbWndExtra = 0;	//紧跟在窗口实例尾部的一块额外  空间，不用则设为0
	wc.hInstance = GetModuleHandle(NULL);	//当前实例句柄
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//任务栏图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//光标样式
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//黑色画刷句柄
	wc.lpszMenuName = NULL;					//菜单资源名字
	wc.lpszClassName = CharToWchar("PigyChan_Renderer");	//窗口名字

	atom = RegisterClass(&wc);
	assert(atom != 0);		//注册窗口类
}


//BMP图片文件头初始化
static void init_bm_header(BITMAPINFOHEADER& bi, int width, int height) {
	memset(&bi, 0, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER);	//结构体大小(包括颜色表)
	bi.biWidth = width;		//图片宽度
	bi.biHeight = -height;	//图片高度，从上到下
	bi.biPlanes = 1;		//绘图设备包含的层数，必须是1
	bi.biBitCount = 32;		//图像位数
	bi.biCompression = BI_RGB;				//压缩方式，0表示不压缩
	bi.biSizeImage = width * height * 4;	//图片数据大小，必须是4的倍数
}

//正式创建窗口
int window_init(int width, int height, const char* title) {
	window = (window_t*)malloc(sizeof(window_t));
	memset(window, 0, sizeof(window_t));
	window->is_close = 0;

	RECT rect = { 0,0,width,height };
	int wx, wy;	//矩阵宽，高
	int sx, sy;	//屏幕重心位置x,y
	LPVOID ptr;
	HDC hDC;				//设备环境句柄
	BITMAPINFOHEADER bi;	//BMP图片

	//设计并注册窗口类
	design_register_window_class();

	//正式创建窗口
	window->h_window = CreateWindow(CharToWchar("PigyChan_Renderer"), CharToWchar(title),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
	assert(window->h_window != NULL);

	//初始化BMP图片文件头格式
	init_bm_header(bi, width, height);

	//获取兼容性DC
	hDC = GetDC(window->h_window);
	window->mem_dc = CreateCompatibleDC(hDC);
	ReleaseDC(window->h_window, hDC);		//与GetDC成对使用

	//创建bmp图
	window->bm_dib = CreateDIBSection(window->mem_dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &ptr, 0, 0); //创建设备无关句柄
	assert(window->bm_dib != NULL);

	//SelectObject:更换句柄
	window->bm_old = (HBITMAP)SelectObject(window->mem_dc, window->bm_dib);//把新创建的位图句柄写入mem_dc
	window->window_fb = (unsigned char*)ptr;

	window->width = width;
	window->height = height;

	AdjustWindowRect(&rect, GetWindowLong(window->h_window, GWL_STYLE), 0);	//调整窗口大小
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;	//GetSystemMetrics(SM_CXSCREEN)获取屏幕分辨率
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0)sy = 0;

	//该函数改变一个子窗口，弹出式窗口式顶层窗口的尺寸，位置和Z序。
	SetWindowPos(window->h_window, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	//激活窗口至前端
	SetForegroundWindow(window->h_window);
	//在屏幕上将窗口显示出来
	ShowWindow(window->h_window, SW_NORMAL);

	//消息循环
	msg_dispatch();

	//初始化keys,window_fb
	memset(window->window_fb, 0, (width)*height * 4);
	memset(window->keys, 0, sizeof(char) * 512);
	return 0;
}

//窗口销毁函数
int window_destroy() {
	if (window->mem_dc) {
		if (window->bm_old) {
			SelectObject(window->mem_dc, window->bm_old);//写入原版HBITMAP才能释放
			window->bm_old = NULL;
		}
		DeleteDC(window->mem_dc);
		window->mem_dc = NULL;
	}
	if (window->bm_dib) {
		DeleteObject(window->bm_dib);
		window->bm_dib = NULL;
	}
	if (window->h_window) {
		CloseWindow(window->h_window);
		window->h_window = NULL;
	}

	free(window);
	return 0;
}


//消息获取函数
void  msg_dispatch() {
	MSG msg = { 0 };
	//如果没收到退出消息就一直运行下去 
	while (1)
	{
		//查看应用程序消息队列，有消息时将队列中的消息派发出去，没有消息就继续往下执行
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;

		TranslateMessage(&msg);	 //转换消息 虚拟按键->字符
		DispatchMessage(&msg); //传送消息给回调
	}
}

//窗口字体显示
static void words_display() {
	LOGFONT logfont; //改变输出bai字体du
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfHeight = 30; //设置字体的大小
	HFONT hFont = CreateFontIndirect(&logfont);

	HDC hDC = GetDC(window->h_window);
	//目标举行的左上角(x,y), 宽度，高度，上下文指针
	SelectObject(window->mem_dc, hFont);
	SetTextColor(window->mem_dc, RGB(200, 200, 200));
	SetBkColor(window->mem_dc, RGB(50, 50, 50));
	TextOut(window->mem_dc, 10, 10,
		CharToWchar("Control:hold left buttion to rotate, right button to pan"),
		strlen("Control:hold left buttion to rotate, right button to pan"));
	BitBlt(hDC, 0, 0, window->width, window->height, window->mem_dc, 0, 0, SRCCOPY);
	ReleaseDC(window->h_window, hDC);
}

//窗口位置函数
void window_draw(unsigned char* framebuffer)
{
	int i, j;
	for (int i = 0; i < window->height; i++)
	{
		for (int j = 0; j < window->width; j++)
		{
			int index = (i * window->width + j) * 4;
			window->window_fb[index] = framebuffer[index + 2];
			window->window_fb[index + 1] = framebuffer[index + 1];
			window->window_fb[index + 2] = framebuffer[index];
		}
	}
	words_display();

}

Vec2 get_mouse_pos() {
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window->h_window, &point);
	return Vec2((float)point.x, (float)point.y);
}

static double get_native_time(void) {
	static double period = -1;
	LARGE_INTEGER counter;
	if (period < 0) {
		LARGE_INTEGER  frequency;
		QueryPerformanceFrequency(&frequency);
		period = 1 / (double)frequency.QuadPart;
	}
	QueryPerformanceCounter(&counter);
	return counter.QuadPart * period;
}

float platform_get_time(void) {
	static double  initial = -1;
	if (initial < 0) {
		initial = get_native_time();
	}
	return (float)(get_native_time() - initial);
}