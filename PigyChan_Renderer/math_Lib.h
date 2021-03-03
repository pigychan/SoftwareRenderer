#ifndef _MATH_LIB_H_
#define _MATH_LIB_H_

#include <iostream>
#include <cmath>
#define PI 3.1415926


//Vec2 类声明部分
class Vec2 {
public:
	Vec2();
	Vec2(float e0, float e1);

	float x()const;
	float y()const;
	float& operator[](int i);
	float operator[](int i)const;

	Vec2 operator-() const;
	Vec2& operator+=(const Vec2& v);
	Vec2& operator*=(const float t);
	Vec2& operator/=(const float t);

	float norm()const;
	float norm_squared()const;

	//属性
	float e[2];
};


//Vec3 类声明部分
class Vec3 {
public:
	Vec3();
	Vec3(float e0, float e1, float e2);

	float x()const;
	float y()const;
	float z()const;
	float& operator[](int i);
	float operator[](int i)const;

	Vec3 operator-()const;
	Vec3& operator+=(const  Vec3& v);
	Vec3& operator*=(const float t);
	Vec3& operator/=(const float t);

	float norm()const;
	float norm_squared()const;

	//属性
	float e[3];
};


//Vec4 类声明部分
class Vec4 {
public:
	Vec4();
	Vec4(float e0, float e1, float e2, float e3);

	float x() const;
	float y() const;
	float z() const;
	float w() const;
	float& operator[](int i);
	float operator[](int i)const;

	Vec4& operator*=(const float t);
	Vec4& operator/=(const float t);

	//属性
	float e[4];

};


//Mat3 类声明部分
class Mat3 {
public:
	Mat3();

	Vec3& operator[](int i);
	Vec3 operator[](int i)const;

	Mat3 transpose()const;
	Mat3 inverse() const;
	Mat3 inverse_transpose()const;
	static Mat3 identity();

	//属性
	Vec3 rows[3];
};


//Mat4 类声明部分
class Mat4 {
public:
	Mat4();

	Vec4& operator[](int i);
	Vec4  operator[](int i)const;

	Mat4 transpose() const;
	Mat4 inverse() const;
	Mat4 inverse_transpose() const;
	static Mat4 identity();

	//属性
	Vec4 rows[4];
};


//Vec2 相关函数声明部分
std::ostream& operator<<(std::ostream& out, const Vec2& v);
Vec2 operator+(const Vec2& u, const Vec2& v);
Vec2 operator-(const Vec2& u, const Vec2& v);
Vec2 operator*(const Vec2& u, const Vec2& v);
Vec2 operator*(float t, const Vec2& v);
Vec2 operator*(const Vec2& v, float t);
Vec2 operator/(Vec2 v, float t);

/* vec3 related functions */
std::ostream& operator<<(std::ostream& out, const Vec3& v);
Vec3 operator+(const Vec3& u, const Vec3& v);
Vec3 operator-(const Vec3& u, const Vec3& v);
Vec3 operator*(const Vec3& u, const Vec3& v);
Vec3 operator*(float t, const Vec3& v);
Vec3 operator*(const Vec3& v, float t);
Vec3 operator/(Vec3 v, float t);
float dot(const Vec3& u, const Vec3& v);
Vec3 cross(const Vec3& u, const Vec3& v);
Vec3 unit_vector(const Vec3& v);
Vec3 cwise_product(const Vec3& a, const Vec3& b);

/* vec4 related functions */
std::ostream& operator<<(std::ostream& out, const Vec4& v);
Vec4 to_vec4(const Vec3& u, float w);
Vec4 operator-(const Vec4& u, const Vec4& v);
Vec4 operator+(const Vec4& u, const Vec4& v);
Vec4 operator*(float t, const Vec4& v);
Vec4 operator*(const Vec4& v, float t);

/* mat3 related functions */
std::ostream& operator<<(std::ostream& out, const Mat3& m);

/* mat4 related functions */
std::ostream& operator<<(std::ostream& out, const Mat4& m);
Vec4 operator*(const Mat4& m, const Vec4 v);
Mat4 operator*(const Mat4& m1, const Mat4& m2);

/* transformation related functions */
Mat4 mat4_translate(float tx, float ty, float tz);
Mat4 mat4_scale(float sx, float sy, float sz);
Mat4 mat4_rotate_x(float angle);
Mat4 mat4_rotate_y(float angle);
Mat4 mat4_rotate_z(float angle);
Mat4 mat4_lookat(Vec3 eye, Vec3 target, Vec3 up);
Mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far);
Mat4 mat4_perspective(float fovy, float aspect, float near, float far);

/* untility functions */
float float_max(float a, float b);
float float_min(float a, float b);
float float_clamp(float f, float min, float max);
float float_lerp(float start, float end, float alpha);
Vec2 vec2_lerp(Vec2& start, Vec2& end, float alpha);
Vec3 vec3_lerp(Vec3& start, Vec3& end, float alpha);
Vec4 vec4_lerp(Vec4& start, Vec4& end, float alpha);

#endif //_MATH_LIB_H_
