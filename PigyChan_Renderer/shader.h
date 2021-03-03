#pragma once
#include "math_Lib.h"
#include "model.h"
#include "camera.h"

#define MAX_VERTEX 10

//���Դ
struct light {
	Vec3 pos;			//λ��
	Vec3 intensity;	//����ǿ��
};

typedef struct {
	//��Ⱦ����Ҫ�ĸ��ּ������
	Mat4 model_matrix;
	Mat4 camera_view_matrix;
	Mat4 light_view_matrix;
	Mat4 camera_perp_matrix;
	Mat4 light_perp_matrix;
	Mat4 mvp_matrix;			//MVP�任����

	//�������
	Camera* camera;
	Model* model;

	//����Я��������
	Vec3 normal_attri[3];
	Vec2 uv_attri[3];
	Vec3 worldcoord_attri[3];
	Vec4 clipcoord_attri[3];

}payload_t;

class IShader {
public:
	payload_t payload;
	virtual void vertex_shader(int nfaces, int nvertex) {}
	//��Щ������x,y,z�ı仯����
	virtual Vec3 fragment_shader(float alpha, float beta, float gamma) { return Vec3{ 0,0,0 }; }
};

class PhongShader :public IShader {
public:
	void vertex_shader(int nfaces, int nvertex);
	Vec3 fragment_shader(float alpha, float beta, float gamma);
};