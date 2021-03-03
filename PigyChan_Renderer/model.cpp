#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h> 
#include "model.h"
#include "shader.h"

void Model::create_map(const char* filename)
{
	diffusemap_ = NULL;
	normalmap_ = NULL;
	specularmap_ = NULL;
	roughnessmap_ = NULL;
	metalnessmap_ = NULL;
	occlusion_map = NULL;
	emision_map = NULL;


	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");

	texfile = texfile.substr(0, dot) + std::string("_diffuse.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		diffusemap_ = new TGAImage();
		load_texture(filename, "_diffuse.tga", diffusemap_);
	}

	texfile = texfile.substr(0, dot) + std::string("_normal.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		normalmap_ = new TGAImage();
		load_texture(filename, "_normal.tga", normalmap_);
	}

	texfile = texfile.substr(0, dot) + std::string("_spec.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		specularmap_ = new TGAImage();
		load_texture(filename, "_spec.tga", specularmap_);
	}

	texfile = texfile.substr(0, dot) + std::string("_roughness.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		roughnessmap_ = new TGAImage();
		load_texture(filename, "_roughness.tga", roughnessmap_);
	}

	texfile = texfile.substr(0, dot) + std::string("_metalness.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		metalnessmap_ = new TGAImage();
		load_texture(filename, "_metalness.tga", metalnessmap_);
	}

	texfile = texfile.substr(0, dot) + std::string("_emission.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		emision_map = new TGAImage();
		load_texture(filename, "_emission.tga", emision_map);
	}

	texfile = texfile.substr(0, dot) + std::string("_occlusion.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		occlusion_map = new TGAImage();
		load_texture(filename, "_occlusion.tga", metalnessmap_);
	}
}


Model::Model(const char* filename, int is_from_mmd)
	: verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_(), is_from_mmd(is_from_mmd)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail())
	{
		printf("load model failed\n");
		return;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3 v;
			for (int i = 0; i < 3; i++)
			{
				iss >> v[i];
			}
			v[1] = v[1];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			Vec3 n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			norms_.push_back(n);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec2 uv;
			for (int i = 0; i < 2; i++) iss >> uv[i];

			if (is_from_mmd)
				uv[1] = 1 - uv[1];
			uv_.push_back(uv);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int tmp[3];
			iss >> trash;
			while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
				for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
				f.push_back(tmp[0]); f.push_back(tmp[1]); f.push_back(tmp[2]);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;



	create_map(filename);
}

Model::~Model() {}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
	std::vector<int> face;
	for (int i = 0; i < 3; i++) face.push_back(faces_[idx][i * 3]);
	return face;
}

Vec3 Model::vert(int i) {
	return verts_[i];
}

Vec3 Model::vert(int iface, int nthvert) {
	return verts_[faces_[iface][nthvert * 3]];
}

Vec2 Model::uv(int iface, int nthvert) {
	return uv_[faces_[iface][nthvert * 3 + 1]];
}

Vec3 Model::normal(int iface, int nthvert) {
	int idx = faces_[iface][nthvert * 3 + 2];
	return unit_vector(norms_[idx]);
}

void Model::load_texture(std::string filename, const char* suffix, TGAImage& img) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		//std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img.read_tga_file(texfile.c_str());
		img.flip_vertically();
	}
}

void Model::load_texture(std::string filename, const char* suffix, TGAImage* img) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		//std::cerr << "texture file " << texfile << " loading " << (img->read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img->read_tga_file(texfile.c_str());
		img->flip_vertically();
	}
}

Vec3 Model::diffuse(Vec2 uv)
{
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	//printf("%f %f\n", uv[0], uv[1]);
	int uv0 = uv[0] * diffusemap_->get_width();
	int uv1 = uv[1] * diffusemap_->get_height();
	TGAColor c = diffusemap_->get(uv0, uv1);
	Vec3 res;
	for (int i = 0; i < 3; i++)
		res[2 - i] = (float)c[i] / 255.f;
	return res;
}

Vec3 Model::normal(Vec2 uv) {
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	int uv0 = uv[0] * normalmap_->get_width();
	int uv1 = uv[1] * normalmap_->get_height();
	TGAColor c = normalmap_->get(uv0, uv1);
	Vec3 res;
	for (int i = 0; i < 3; i++)
		res[2 - i] = (float)c[i] / 255.f * 2.f - 1.f; //because the normap_map coordinate is -1 ~ +1
	return res;
}

float Model::roughness(Vec2 uv) {
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	int uv0 = uv[0] * roughnessmap_->get_width();
	int uv1 = uv[1] * roughnessmap_->get_height();
	return roughnessmap_->get(uv0, uv1)[0] / 255.f;
}

float Model::metalness(Vec2 uv) {
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	int uv0 = uv[0] * metalnessmap_->get_width();
	int uv1 = uv[1] * metalnessmap_->get_height();
	return metalnessmap_->get(uv0, uv1)[0] / 255.f;
}

float Model::specular(Vec2 uv) {
	int uv0 = uv[0] * specularmap_->get_width();
	int uv1 = uv[1] * specularmap_->get_height();
	return specularmap_->get(uv0, uv1)[0] / 1.f;
}


float Model::occlusion(Vec2 uv) {
	if (!occlusion_map)
		return 1;
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	int uv0 = uv[0] * occlusion_map->get_width();
	int uv1 = uv[1] * occlusion_map->get_height();
	return occlusion_map->get(uv0, uv1)[0] / 255.f;
}

Vec3 Model::emission(Vec2 uv)
{
	if (!occlusion_map)
		return Vec3(0.0f, 0.0f, 0.0f);
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	//printf("%f %f\n", uv[0], uv[1]);
	int uv0 = uv[0] * emision_map->get_width();
	int uv1 = uv[1] * emision_map->get_height();
	TGAColor c = emision_map->get(uv0, uv1);
	Vec3 res;
	for (int i = 0; i < 3; i++)
		res[2 - i] = (float)c[i] / 255.f;
	return res;
}

