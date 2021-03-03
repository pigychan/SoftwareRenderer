#pragma once
#include <Windows.h>
#include "math_Lib.h"

//���ṹ�嶨�岿��
typedef struct mouse {
	Vec2 LeftButton_pos;		//��������λ��	
	Vec2 LeftButton_delta;		//��������λ�ñ仯

	Vec2 RightButton_pos;		//����Ҽ���λ��
	Vec2 RightButton_delta;		//����Ҽ���λ�ñ仯

	float wheel_delta;		//���ֱ仯
}mouse_t;

//���ڽṹ�嶨�岿��
typedef struct window {
	HWND h_window;		//���ھ��
	HDC mem_dc;			//Device Context�豸�������
	HBITMAP bm_old;		//�洢ԭ���λͼ����������ٴ��ڵ�ʱ��Ҫ����λ
	HBITMAP bm_dib;		//�豸�޹�λͼ���
	unsigned char* window_fb;	//window framebuffer
	int width;			//��	
	int height;			//�� 
	char keys[512];		//TODO:
	char buttons[2];	//�������Ҽ���Ϣ
	int is_close;		//���ر�־
	mouse_t mouse_info;	//�����Ϣ
}window_t;

extern window_t* window;

//��ع��ܺ�����������
int window_init(int width, int height, const char* title);	//���ڳ�ʼ��
int window_destroy();	//��������
void window_draw(unsigned char* framebuffer);	//���ڻ���
void msg_dispatch();	//��Ϣ��ȡ
Vec2 get_mouse_pos();	//��ȡ���λ��
float platform_get_time(void);	//��ȡʱ��