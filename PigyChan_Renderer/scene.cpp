#include "scene.h"

TGAImage* texture_from_file(const char* file_name)
{
	TGAImage* texture = new TGAImage();
	texture->read_tga_file(file_name);
	texture->flip_vertically();
	return texture;
}



void build_fuhua_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera)
{
	m = 4;
	const char* modelname[] =
	{
		"../obj/fuhua/fuhuabody.obj",
		"../obj/fuhua/fuhuahair.obj",
		"../obj/fuhua/fuhuaface.obj",
		"../obj/fuhua/fuhuacloak.obj",
	};

	int i = 0;
	int vertex = 0, face = 0;
	const char* scene_name = "fuhua";
	PhongShader* shader_phong = new PhongShader();

	for (i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i], 1);
		vertex += model[i]->nverts();
		face += model[i]->nfaces();
	}


	shader_phong->payload.camera_perp_matrix = perspective;
	shader_phong->payload.camera = camera;

	*shader_use = shader_phong;

	printf("scene name:%s\n", scene_name);
	printf("model number:%d\n", m);
	printf("vertex:%d faces:%d\n", vertex, face);
}

void build_qiyana_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera)
{
	m = 3;
	const char* modelname[] =
	{
		"../obj/qiyana/qiyanabody.obj",
		"../obj/qiyana/qiyanahair.obj",
		"../obj/qiyana/qiyanaface.obj",
	};


	int i = 0;
	int vertex = 0, face = 0;
	const char* scene_name = "qiyana";
	PhongShader* shader_phong = new PhongShader();

	for (i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i], 1);
		vertex += model[i]->nverts();
		face += model[i]->nfaces();
	}


	shader_phong->payload.camera_perp_matrix = perspective;
	shader_phong->payload.camera = camera;

	*shader_use = shader_phong;

	printf("scene name:%s\n", scene_name);
	printf("model number:%d\n", m);
	printf("vertex:%d faces:%d\n", vertex, face);
}


void build_xier_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera)
{
	m = 5;
	const char* modelname[] =
	{
		"../obj/xier/xierbody.obj",
		"../obj/xier/xierhair.obj",
		"../obj/xier/xierface.obj",
		"../obj/xier/xiercloth.obj",
		"../obj/xier/xierarm.obj",
	};


	int i = 0;
	int vertex = 0, face = 0;
	const char* scene_name = "xier";
	PhongShader* shader_phong = new PhongShader();

	for (i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i], 1);
		vertex += model[i]->nverts();
		face += model[i]->nfaces();
	}


	shader_phong->payload.camera_perp_matrix = perspective;
	shader_phong->payload.camera = camera;

	*shader_use = shader_phong;

	printf("scene name:%s\n", scene_name);
	printf("model number:%d\n", m);
	printf("vertex:%d faces:%d\n", vertex, face);
}


void build_yayi_scene(Model** model, int& m, IShader** shader_use, Mat4 perspective, Camera* camera)
{
	m = 7;
	const char* modelname[] = {
		"../obj/yayi/yayiface.obj",
		"../obj/yayi/yayibody.obj",
		"../obj/yayi/yayihair.obj",
		"../obj/yayi/yayiarmour1.obj",
		"../obj/yayi/yayiarmour2.obj",
		"../obj/yayi/yayidecoration.obj",
		"../obj/yayi/yayisword.obj"
	};

	int i = 0;
	int vertex = 0, face = 0;
	const char* scene_name = "yayi";
	PhongShader* shader_phong = new PhongShader();

	for (i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i], 1);
		vertex += model[i]->nverts();
		face += model[i]->nfaces();
	}


	shader_phong->payload.camera_perp_matrix = perspective;
	shader_phong->payload.camera = camera;

	*shader_use = shader_phong;

	printf("scene name:%s\n", scene_name);
	printf("model number:%d\n", m);
	printf("vertex:%d faces:%d\n", vertex, face);
}

