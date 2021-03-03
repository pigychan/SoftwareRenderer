#include <iostream>
#include <time.h>
#include "tgaimage.h"
#include "shader.h"
#include "model.h"
#include "win32.h"
#include "camera.h"
#include "pipeline.h"
#include "scene.h"
const int MAX_MODEL_NUM = 10;

using namespace std;

Vec3 eye(0, 1, 3);
Vec3 up(0, 1, 0);
Vec3 target(0, 1, 0);
Mat4 model_mat;
Mat4 view;
Mat4 perspective;

static scene_t scenes[]
{
	{"fuhua",build_fuhua_scene},
};

void clear_zbuffer(int width, int height, float* zbuffer)
{
	for (int i = 0; i < width * height; i++)
		zbuffer[i] = 100000;
}

void clear_framebuffer(int width, int height, unsigned char* framebuffer)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int index = (i * width + j) * 4;

			framebuffer[index + 2] = 80;
			framebuffer[index + 1] = 56;
			framebuffer[index] = 56;
		}
	}
}

void update_matrix_data(Camera& camera, Mat4 perspective, IShader* shader_model)
{
	view = mat4_lookat(camera.eye, camera.target, camera.up);
	Mat4 mvp = perspective * view;
	shader_model->payload.camera_view_matrix = view;
	shader_model->payload.mvp_matrix = mvp;
}


int main()
{
	int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;

	//malloc memory for zbuffer and framebuffer
	float* zbuffer;
	unsigned char* framebuffer;
	zbuffer = (float*)malloc(sizeof(float) * width * height);
	framebuffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
	memset(framebuffer, 0, sizeof(unsigned char) * width * height * 4);

	//set mvp matrix
	model_mat = Mat4::identity();
	view = mat4_lookat(eye, target, up);
	perspective = mat4_perspective(60, (float)(width) / height, -0.1, -10000);

	//create camera
	Camera camera(eye, target, up, (float)(width) / height);

	//initialize models and shaders
	int model_num = 0;
	Model* model[MAX_MODEL_NUM];
	IShader* shader_model;

	//build scene randomly
	srand((unsigned int)time(NULL));
	int i = rand() % 2;
	scenes[i].build_scene(model, model_num, &shader_model, perspective, &camera);

	//start to render
	int num_frames = 0;
	window_init(width, height, "PigyChan_Renderer");
	float print_time = platform_get_time();
	while (!window->is_close)
	{
		float curr_time = platform_get_time();

		//reset buffer
		clear_framebuffer(width, height, framebuffer);
		clear_zbuffer(width, height, zbuffer);

		//handle camera events and update view, perspective matrix
		handle_events(camera);
		update_matrix_data(camera, perspective, shader_model);

		//draw loop
		for (int m = 0; m < model_num; m++)
		{
			shader_model->payload.model = model[m];

			IShader* shader;
			shader = shader_model;

			int num = model[m]->nfaces();
			for (int i = 0; i < num; i++)
			{
				draw_triangles(framebuffer, zbuffer, *shader, i);
			}
		}

		//calculate FPS
		num_frames += 1;
		if (curr_time - print_time >= 1) {
			int sum_millis = (int)((curr_time - print_time) * 1000);
			int avg_millis = sum_millis / num_frames;
			printf("fps: %3d, avg: %3d ms\n", num_frames, avg_millis);
			num_frames = 0;
			print_time = curr_time;
		}

		//reset mouse information
		window->mouse_info.wheel_delta = 0;
		window->mouse_info.LeftButton_delta = Vec2(0, 0);
		window->mouse_info.RightButton_delta = Vec2(0, 0);

		//display image
		window_draw(framebuffer);
		msg_dispatch();
	}


	//free memory
	free(framebuffer);
	free(zbuffer);
	delete shader_model;
	for (int i = 0; i < model_num; i++)
	{
		if (model[i] != NULL)
			delete model[i];
	}
	window_destroy();

	system("pause");
	return 0;
}
