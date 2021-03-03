#pragma once
#include <Windows.h>
#include "math_Lib.h"

//鼠标结构体定义部分
typedef struct mouse {
	Vec2 LeftButton_pos;		//鼠标左键的位置	
	Vec2 LeftButton_delta;		//鼠标左键的位置变化

	Vec2 RightButton_pos;		//鼠标右键的位置
	Vec2 RightButton_delta;		//鼠标右键的位置变化

	float wheel_delta;		//滚轮变化
}mouse_t;

//窗口结构体定义部分
typedef struct window {
	HWND h_window;		//窗口句柄
	HDC mem_dc;			//Device Context设备环境句柄
	HBITMAP bm_old;		//存储原版的位图句柄，在销毁窗口的时候要它归位
	HBITMAP bm_dib;		//设备无关位图句柄
	unsigned char* window_fb;	//window framebuffer
	int width;			//宽	
	int height;			//高 
	char keys[512];		//TODO:
	char buttons[2];	//鼠标左键右键信息
	int is_close;		//开关标志
	mouse_t mouse_info;	//鼠标信息
}window_t;

extern window_t* window;

//相关功能函数声明部分
int window_init(int width, int height, const char* title);	//窗口初始化
int window_destroy();	//窗口销毁
void window_draw(unsigned char* framebuffer);	//窗口绘制
void msg_dispatch();	//信息截取
Vec2 get_mouse_pos();	//获取鼠标位置
float platform_get_time(void);	//获取时间