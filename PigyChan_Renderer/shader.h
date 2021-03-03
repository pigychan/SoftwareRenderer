#pragma once
#include "math_Lib.h"
#include "model.h"
#include "camera.h"

#define MAX_VERTEX 10

//点光源
struct light {
	Vec3 pos;			//位置
	Vec3 intensity;	//光照强度
};

typedef struct {
	//渲染所需要的各种计算矩阵
	Mat4 model_matrix;
	Mat4 camera_view_matrix;
	Mat4 light_view_matrix;
	Mat4 camera_perp_matrix;
	Mat4 light_perp_matrix;
	Mat4 mvp_matrix;			//MVP变换矩阵

	//其他组件
	Camera* camera;
	Model* model;

	//顶点携带的属性
	Vec3 normal_attri[3];
	Vec2 uv_attri[3];
	Vec3 worldcoord_attri[3];
	Vec4 clipcoord_attri[3];

}payload_t;

class IShader {
public:
	payload_t payload;
	virtual void vertex_shader(int nfaces, int nvertex) {}
	//这些参数是x,y,z的变化趋势
	virtual Vec3 fragment_shader(float alpha, float beta, float gamma) { return Vec3{ 0,0,0 }; }
};

class PhongShader :public IShader {
public:
	void vertex_shader(int nfaces, int nvertex);
	Vec3 fragment_shader(float alpha, float beta, float gamma);
};