#include <vector>
#include "shader.h"
#include "sample.hpp"

//TODO:计算法线，不知道为啥这样实现
static Vec3 cal_normal (Vec3& normal, Vec3* world_coords, const Vec2* uvs,
	const Vec2& uv, TGAImage* normal_map){
	//计算纹理坐标变化
	float x1 = uvs[1][0] - uvs[0][0];
	float y1 = uvs[1][1] - uvs[0][1];
	float x2 = uvs[2][0] - uvs[0][0];
	float y2 = uvs[2][1] - uvs[0][1];
	float delta = (x1 * y2 - x2 * y1);

	//计算世界坐标变化
	Vec3 e1 = world_coords[1] - world_coords[0];
	Vec3 e2 = world_coords[2] - world_coords[0];

	Vec3 t = e1 * y2 + e2 * (-y1);
	Vec3 b = e1 * (-x2) + e2 * x1;
	t /= delta;
	b /= delta;

	//TODO:Schmidt orthogonalization
	normal = unit_vector(normal);
	t = unit_vector(t - dot(t, normal) * normal);
	b = unit_vector(b - dot(b, normal) * normal - dot(b, t) * t);

	Vec3 sample = texture_sample(uv, normal_map);
	//将sample范围从0~1变为-1~+1
	sample = Vec3(sample[0] * 2 - 1, sample[1] * 2 - 1, sample[2] * 2 - 1);

	Vec3 normal_new = t * sample[0] + b * sample[1] + normal * sample[2];
	return normal_new;
}

//Bling_Phong渲染模型
void PhongShader::vertex_shader(int nfaces, int nvertex) {
	Vec4 temp_vert = to_vec4(payload.model->vert(nfaces, nvertex), 1.0f);
	Vec4 temp_normal = to_vec4(payload.model->normal(nfaces, nvertex), 1.0f);

	//获取各顶点对应的纹理
	payload.uv_attri[nvertex] = payload.model->uv(nfaces, nvertex);
	//各顶点进行MVP变换
	payload.clipcoord_attri[nvertex] = payload.mvp_matrix * temp_vert;

	//存储世界坐标和顶点信息
	for (int i = 0; i < 3; ++i) {
		payload.worldcoord_attri[nvertex][i] = temp_vert[i];
		payload.normal_attri[nvertex][i] = temp_normal[i];
	}
}


Vec3 PhongShader::fragment_shader(float alpha, float beta, float gamma) {
	//设置光源信息
	float p = 150.0;
	Vec3 amb_light_intensity(5, 5, 5);
	light light1{ {2,4,2},{50,50,50} };
	light light2{ {-1,0.2,5},{10,10,10} };
	std::vector<light> lights = { light1 };

	//获取顶点上 的各类信息
	Vec4* clip_coords = payload.clipcoord_attri;
	Vec3* world_coords = payload.worldcoord_attri;
	Vec3* normals = payload.normal_attri;
	Vec2* uvs = payload.uv_attri;

	//通过各顶点插值计算得到当前着色点的各类信息
	float Z = 1.0 / (alpha / clip_coords[0].w() + beta / clip_coords[1].w() + gamma / clip_coords[2].w());
	Vec3 normal = (alpha * normals[0] / clip_coords[0].w() + beta * normals[1] / clip_coords[1].w() +
		gamma * normals[2] / clip_coords[2].w()) * Z;
	Vec2 uv = (alpha * uvs[0] / clip_coords[0].w() + beta * uvs[1] / clip_coords[1].w() +
		gamma * uvs[2] / clip_coords[2].w()) * Z;
	Vec3 worldpos = (alpha * world_coords[0] / clip_coords[0].w() + beta * world_coords[1] / clip_coords[1].w() +
		gamma * world_coords[2] / clip_coords[2].w()) * Z;

	//TODO:???
	if (payload.model->normalmap_) {
		normal = cal_normal(normal, world_coords, uvs, uv, payload.model->normalmap_);
	}

	//Bling_Phong光照模型系数初始化
	Vec3 ka(0.0005, 0.0005, 0.0005);
	Vec3 ks(0.7000, 0.7000, 0.7000);
	Vec3 kd = payload.model->diffuse(uv);


	//计算当前点的着色颜色
	Vec3 rst_color(0, 0, 0);
	for (auto& light : lights) {
		Vec3 ambient, diffuse, specular;
		normal = unit_vector(normal);
		Vec3 l = unit_vector(Vec3(1, 3, 1));
		Vec3 v = unit_vector(payload.camera->eye - worldpos);
		Vec3 h = unit_vector(l + v);
		float radius = (light.pos - worldpos).norm_squared();;

		ambient = cwise_product(ka, amb_light_intensity);
		diffuse = cwise_product(kd, light.intensity) * float_max(0, dot(l, normal)) / radius;
		diffuse = kd * float_max(0, dot(l, normal));
		specular = cwise_product(ks, light.intensity) * float_max(0, std::pow(dot(normal, h), p)) / radius;

		rst_color += (0.9 * kd + diffuse * 0.2);
	}

	return rst_color * 255.f;
}