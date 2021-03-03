#define _CRT_SECURE_NO_WARNINGS
#include "win32.h"
#include <assert.h>
#include <stdio.h>

window_t* window = NULL;

//charתWchar����
const wchar_t* CharToWchar(const char* ch) {
	const size_t len = strlen(ch) + 1;
	wchar_t* wch = new wchar_t[len];
	mbstowcs(wch, ch, len);
	return wch;
}

//���ڹ��̺���
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CLOSE:	//�ر��ź�
		window->is_close = 1;
		break;
	case WM_KEYDOWN:	//���̰����ź�
		window->keys[wParam & 511] = 1;	//TODO:
		break;
	case WM_KEYUP:	//����̧���ź�
		window->keys[wParam & 511] = 0;
		break;
	case WM_LBUTTONDOWN:	//�����������ź�,��ȡ��ʱ�����λ��
		window->mouse_info.LeftButton_pos = get_mouse_pos();
		window->buttons[0] = 1;
		break;
	case WM_LBUTTONUP:		//������̧���ź�
		window->buttons[0] = 0;
		break;
	case WM_RBUTTONDOWN:	//����Ҽ������źţ���ȡ��ʱ�����λ��
		window->mouse_info.RightButton_pos = get_mouse_pos();
		window->buttons[1] = 1;
		break;
	case  WM_RBUTTONUP:		//����Ҽ�̧���ź�
		window->buttons[1] = 0;
		break;
	case WM_MOUSEWHEEL:		//�������ź�
		window->mouse_info.wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		break;
	default:
		//Ϊû�д���Ĵ�����Ϣ�ṩĬ�ϴ���
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;	//�����˳�
}

//��Ʋ�ע�ᴰ����
static void design_register_window_class() {
	ATOM atom;
	WNDCLASS wc = { 0 };
	wc.style = CS_BYTEALIGNCLIENT;			//���ڷ��
	wc.lpfnWndProc = (WNDPROC)WndProc;		//�ص�����
	wc.cbClsExtra = 0;	//�����ڴ�����β����һ�����ռ䣬��������Ϊ0
	wc.cbWndExtra = 0;	//�����ڴ���ʵ��β����һ�����  �ռ䣬��������Ϊ0
	wc.hInstance = GetModuleHandle(NULL);	//��ǰʵ�����
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//������ͼ��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//�����ʽ
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//��ɫ��ˢ���
	wc.lpszMenuName = NULL;					//�˵���Դ����
	wc.lpszClassName = CharToWchar("PigyChan_Renderer");	//��������

	atom = RegisterClass(&wc);
	assert(atom != 0);		//ע�ᴰ����
}


//BMPͼƬ�ļ�ͷ��ʼ��
static void init_bm_header(BITMAPINFOHEADER& bi, int width, int height) {
	memset(&bi, 0, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER);	//�ṹ���С(������ɫ��)
	bi.biWidth = width;		//ͼƬ���
	bi.biHeight = -height;	//ͼƬ�߶ȣ����ϵ���
	bi.biPlanes = 1;		//��ͼ�豸�����Ĳ�����������1
	bi.biBitCount = 32;		//ͼ��λ��
	bi.biCompression = BI_RGB;				//ѹ����ʽ��0��ʾ��ѹ��
	bi.biSizeImage = width * height * 4;	//ͼƬ���ݴ�С��������4�ı���
}

//��ʽ��������
int window_init(int width, int height, const char* title) {
	window = (window_t*)malloc(sizeof(window_t));
	memset(window, 0, sizeof(window_t));
	window->is_close = 0;

	RECT rect = { 0,0,width,height };
	int wx, wy;	//�������
	int sx, sy;	//��Ļ����λ��x,y
	LPVOID ptr;
	HDC hDC;				//�豸�������
	BITMAPINFOHEADER bi;	//BMPͼƬ

	//��Ʋ�ע�ᴰ����
	design_register_window_class();

	//��ʽ��������
	window->h_window = CreateWindow(CharToWchar("PigyChan_Renderer"), CharToWchar(title),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
	assert(window->h_window != NULL);

	//��ʼ��BMPͼƬ�ļ�ͷ��ʽ
	init_bm_header(bi, width, height);

	//��ȡ������DC
	hDC = GetDC(window->h_window);
	window->mem_dc = CreateCompatibleDC(hDC);
	ReleaseDC(window->h_window, hDC);		//��GetDC�ɶ�ʹ��

	//����bmpͼ
	window->bm_dib = CreateDIBSection(window->mem_dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &ptr, 0, 0); //�����豸�޹ؾ��
	assert(window->bm_dib != NULL);

	//SelectObject:�������
	window->bm_old = (HBITMAP)SelectObject(window->mem_dc, window->bm_dib);//���´�����λͼ���д��mem_dc
	window->window_fb = (unsigned char*)ptr;

	window->width = width;
	window->height = height;

	AdjustWindowRect(&rect, GetWindowLong(window->h_window, GWL_STYLE), 0);	//�������ڴ�С
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;	//GetSystemMetrics(SM_CXSCREEN)��ȡ��Ļ�ֱ���
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0)sy = 0;

	//�ú����ı�һ���Ӵ��ڣ�����ʽ����ʽ���㴰�ڵĳߴ磬λ�ú�Z��
	SetWindowPos(window->h_window, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	//�������ǰ��
	SetForegroundWindow(window->h_window);
	//����Ļ�Ͻ�������ʾ����
	ShowWindow(window->h_window, SW_NORMAL);

	//��Ϣѭ��
	msg_dispatch();

	//��ʼ��keys,window_fb
	memset(window->window_fb, 0, (width)*height * 4);
	memset(window->keys, 0, sizeof(char) * 512);
	return 0;
}

//�������ٺ���
int window_destroy() {
	if (window->mem_dc) {
		if (window->bm_old) {
			SelectObject(window->mem_dc, window->bm_old);//д��ԭ��HBITMAP�����ͷ�
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


//��Ϣ��ȡ����
void  msg_dispatch() {
	MSG msg = { 0 };
	//���û�յ��˳���Ϣ��һֱ������ȥ 
	while (1)
	{
		//�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��û����Ϣ�ͼ�������ִ��
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;

		TranslateMessage(&msg);	 //ת����Ϣ ���ⰴ��->�ַ�
		DispatchMessage(&msg); //������Ϣ���ص�
	}
}

//����������ʾ
static void words_display() {
	LOGFONT logfont; //�ı����bai����du
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfHeight = 30; //��������Ĵ�С
	HFONT hFont = CreateFontIndirect(&logfont);

	HDC hDC = GetDC(window->h_window);
	//Ŀ����е����Ͻ�(x,y), ��ȣ��߶ȣ�������ָ��
	SelectObject(window->mem_dc, hFont);
	SetTextColor(window->mem_dc, RGB(200, 200, 200));
	SetBkColor(window->mem_dc, RGB(50, 50, 50));
	TextOut(window->mem_dc, 10, 10,
		CharToWchar("Control:hold left buttion to rotate, right button to pan"),
		strlen("Control:hold left buttion to rotate, right button to pan"));
	BitBlt(hDC, 0, 0, window->width, window->height, window->mem_dc, 0, 0, SRCCOPY);
	ReleaseDC(window->h_window, hDC);
}

//����λ�ú���
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