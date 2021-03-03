#pragma once
#include "tgaimage.h"
#include "shader.h"

typedef struct {
	const char* scene_name;
	void (*build_scene)(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera);
} scene_t;

TGAImage* texture_from_file(const char* file_name);

void build_fuhua_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera);
void build_yayi_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera);
void build_qiyana_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera);
void build_xier_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera);