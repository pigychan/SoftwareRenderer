#include "camera.h"
#include "win32.h"

Camera::Camera(Vec3 e, Vec3 t, Vec3  up, float aspect):
	eye(e),target(t),up(up),aspect(aspect){}

Camera::~Camera() {}

//TODO
void update_camera_pos(Camera& camera) {
	Vec3 from_target = camera.eye - camera.target;
	float factor = 1.5 * PI;	//变化权值
	float radius = from_target.norm();
	float phi = (float)atan2(from_target[0], from_target[2]);	//方位角，返回
	float theta = (float)acos(from_target[1] / radius);			//天顶角		
	float x_delta = window->mouse_info.LeftButton_delta[0] / window->width;
	float y_delta = window->mouse_info.LeftButton_delta[1] / window->height;

	//滚轮缩放变化
	radius *= (float)pow(0.95, window->mouse_info.wheel_delta);

	//左键旋转变化
	phi += x_delta * factor;
	theta += y_delta * factor;
	if (theta > PI)theta = PI - 1e-3f;
	if (theta < 0)theta = 1e-3f;

	camera.eye[0] = camera.target[0] + radius * sin(phi) * sin(theta);
	camera.eye[1] = camera.target[1] + radius * cos(theta);
	camera.eye[2] = camera.target[2] + radius * sin(theta) * cos(phi);

	//右键位移变化
	factor = radius * (float)tan(60.0 / 360 * PI) * 2.2;
	x_delta = window->mouse_info.RightButton_delta[0] / window->width;
	y_delta = window->mouse_info.RightButton_delta[1] / window->height;
	Vec3 left = x_delta * factor * camera.x;
	Vec3 up = y_delta * factor * camera.y;

	camera.eye += (left - up);
	camera.target += (left - up);
}

//鼠标事件处理函数
void handle_mouse_events(Camera& camera) {
	//左键按下时得到新坐标，减去旧坐标即可获得位移
	if (window->buttons[0]) {
		Vec2 cur_pos = get_mouse_pos();
		window->mouse_info.LeftButton_delta = window->mouse_info.LeftButton_pos - cur_pos;
		window->mouse_info.LeftButton_pos = cur_pos;
	}
	//右键按下时得到新坐标，减去旧坐标即可获得位移
	if (window->buttons[1]) {
		Vec2 cur_pos = get_mouse_pos();
		window->mouse_info.RightButton_delta = window->mouse_info.RightButton_pos - cur_pos;
		window->mouse_info.RightButton_pos = cur_pos;
	}

	update_camera_pos(camera);
}

//键盘事件处理函数
void handle_key_events(Camera& camera)
{
	float distance = (camera.target - camera.eye).norm();

	if (window->keys['W'])
	{
		camera.eye += -10.0 / window->width * camera.z * distance;
	}
	if (window->keys['S'])
	{
		camera.eye += 0.05f * camera.z;
	}
	if (window->keys[VK_UP] || window->keys['Q'])
	{
		camera.eye += 0.05f * camera.y;
		camera.target += 0.05f * camera.y;
	}
	if (window->keys[VK_DOWN] || window->keys['E'])
	{
		camera.eye += -0.05f * camera.y;
		camera.target += -0.05f * camera.y;
	}
	if (window->keys[VK_LEFT] || window->keys['A'])
	{
		camera.eye += -0.05f * camera.x;
		camera.target += -0.05f * camera.x;
	}
	if (window->keys[VK_RIGHT] || window->keys['D'])
	{
		camera.eye += 0.05f * camera.x;
		camera.target += 0.05f * camera.x;
	}
}

void handle_events(Camera& camera)
{
	//calculate camera axis
	camera.z = unit_vector(camera.eye - camera.target);
	camera.x = unit_vector(cross(camera.up, camera.z));
	camera.y = unit_vector(cross(camera.z, camera.x));

	//mouse and keyboard events
	handle_mouse_events(camera);
	handle_key_events(camera);
}