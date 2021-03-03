#include "pipeline.h"


//����������ƽ�����������
static Vec3 compute_barycentric2D(float x, float y, const Vec3* v)
{
	float c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) / (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() - v[2].x() * v[1].y());
	float c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) / (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() - v[0].x() * v[2].y());
	float c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) / (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() - v[1].x() * v[0].y());
	return Vec3(c1, c2, c3);
}

//������ɫ
static void set_color(unsigned char* framebuffer, int x, int y, unsigned char color[])
{
	int i;
	int index = ((WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x) * 4;

	for (i = 0; i < 3; i++)
		framebuffer[index + i] = color[i];
}

//�жϵ��Ƿ�����������
static int is_inside_triangle(float alpha, float beta, float gamma)
{
	int flag = 0;
	if (alpha > -EPSILON2 && beta > -EPSILON2 && gamma > -EPSILON2)
		flag = 1;

	return flag;
}

//��ȡ��������
static int get_index(int x, int y)
{
	return (WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x;
}

//��դ��
void rasterize(Vec4* clipcoord_attri, unsigned char* framebuffer, float* zbuffer, IShader& shader)
{
	Vec3 ndc_pos[3];
	Vec3 screen_pos[3];
	unsigned char c[3];
	int width = window->width;
	int height = window->height;
	int i = 0, j = 0, flag = 0;

	//���������Ķ���λ����Ϊ1
	for (i = 0; i < 3; i++)
	{
		ndc_pos[i][0] = clipcoord_attri[i][0] / clipcoord_attri[i].w();
		ndc_pos[i][1] = clipcoord_attri[i][1] / clipcoord_attri[i].w();
		ndc_pos[i][2] = clipcoord_attri[i][2] / clipcoord_attri[i].w();
	}

	//�ӿڱ任������׶���Ϊ��ά��׼������
	for (i = 0; i < 3; i++)
	{
		screen_pos[i][0] = 0.5 * (width - 1) * (ndc_pos[i][0] + 1.0);
		screen_pos[i][1] = 0.5 * (height - 1) * (ndc_pos[i][1] + 1.0);
		screen_pos[i][2] = -clipcoord_attri[i].w();

		//�����������β�����������������׶���ڵģ�����м򵥵زü�
		if (screen_pos[i][0] < 0 || screen_pos[i][0] >= width
			|| screen_pos[i][1] < 0 || screen_pos[i][1] >= height)
		{
			return;
		}
	}

	//bounding box
	float xmin = 10000, xmax = -10000, ymin = 10000, ymax = -10000;
	for (int i = 0; i < 3; i++) {
		xmin = float_min(xmin, screen_pos[i][0]);
		xmax = float_max(xmax, screen_pos[i][0]);
		ymin = float_min(ymin, screen_pos[i][1]);
		ymax = float_max(ymax, screen_pos[i][1]);
	}

	//��դ��������
	for (int x = (int)xmin; x <= (int)xmax; x++)
	{
		for (int y = (int)ymin; y <= (int)ymax; y++)
		{
			//ͨ�����������Σ���ȡ�������ڲ���ĸ�����Ϣ
			Vec3 result = compute_barycentric2D((float)(x + 0.5), (float)(y + 0.5), screen_pos);
			float alpha = result.x(); float beta = result.y(); float gamma = result.z();

			//���������������
			if (is_inside_triangle(alpha, beta, gamma))
			{
				int index = get_index(x, y);
				//ͨ����ֵ���㣬�õ����
				float normalizer = 1.0 / (alpha / clipcoord_attri[0].w() + beta / clipcoord_attri[1].w() + gamma / clipcoord_attri[2].w());		
				float z = (alpha * screen_pos[0].z() / clipcoord_attri[0].w() + beta * screen_pos[1].z() / clipcoord_attri[1].w() +
					gamma * screen_pos[2].z() / clipcoord_attri[2].w()) * normalizer;

				//����Zbuffer����
				if (zbuffer[index] > z)
				{
					zbuffer[index] = z;
					//������ɫ
					Vec3 color = shader.fragment_shader(alpha, beta, gamma);

					for (i = 0; i < 3; i++){
						c[i] = (int)float_clamp(color[i], 0, 255);
					}
					set_color(framebuffer, x, y, c);
				}
			}
		}
	}
}


//����������
void draw_triangles(unsigned char* framebuffer, float* zbuffer, IShader& shader, int nface)
{
	int i;
	//vertex shader
	for (i = 0; i < 3; i++)
	{
		shader.vertex_shader(nface, i);
		
	}

	rasterize(shader.payload.clipcoord_attri, framebuffer, zbuffer, shader);
}
