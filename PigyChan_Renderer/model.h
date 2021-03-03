#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "tgaimage.h"
#include "math_Lib.h"

typedef struct cubemap cubemap_t;

class Model {
private:
	std::vector<Vec3> verts_;
	std::vector<std::vector<int> > faces_; // attention, this Vec3i means vertex/uv/normal
	std::vector<Vec3> norms_;
	std::vector<Vec2> uv_;


	void create_map(const char* filename);
	void load_texture(std::string filename, const char* suffix, TGAImage& img);
	void load_texture(std::string filename, const char* suffix, TGAImage* img);
public:
	Model(const char* filename, int is_from_mmd = 0);
	~Model();

	//map
	int is_from_mmd;
	TGAImage* diffusemap_;
	TGAImage* normalmap_;
	TGAImage* specularmap_;
	TGAImage* roughnessmap_;
	TGAImage* metalnessmap_;
	TGAImage* occlusion_map;
	TGAImage* emision_map;

	int nverts();
	int nfaces();
	Vec3 normal(int iface, int nthvert);
	Vec3 normal(Vec2 uv);
	Vec3 vert(int i);
	Vec3 vert(int iface, int nthvert);

	Vec2 uv(int iface, int nthvert);
	Vec3 diffuse(Vec2 uv);
	float roughness(Vec2 uv);
	float metalness(Vec2 uv);
	Vec3 emission(Vec2 uv);
	float occlusion(Vec2 uv);
	float specular(Vec2 uv);

	std::vector<int> face(int idx);
};
#endif //__MODEL_H__