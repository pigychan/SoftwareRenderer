#pragma once
#include "math_Lib.h"

class Camera {
public:
	Camera(Vec3 e, Vec3 t, Vec3 up, float aspect);
	~Camera();

	Vec3 eye;
	Vec3 target;
	Vec3 up;
	Vec3 x;
	Vec3 y;
	Vec3 z;
	float aspect;
};

void update_camera_pos(Camera& camera);
void handle_events(Camera& camera);