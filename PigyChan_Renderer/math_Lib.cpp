#include "math_Lib.h"

//Vec2 类成员函数定义部分 
Vec2::Vec2() :e{ 0, 0 } {}
Vec2::Vec2(float e0, float e1) : e{ e0,e1 } {}
float Vec2::x()const { return e[0]; }
float Vec2::y()const { return e[1]; }
Vec2 Vec2::operator-()const { return Vec2(-e[0], -e[1]); }
float Vec2::operator[](int i)const { return e[i]; }
float& Vec2::operator[](int i) { return e[i]; }
Vec2& Vec2::operator+=(const Vec2& v) {
	e[0] += v[0]; 
	e[1] += v[1];
	return *this;
}
Vec2& Vec2::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	return *this;
}
Vec2& Vec2::operator/=(const  float t) {
	return (*this) *= (1 / t);
}
float Vec2::norm_squared() const { return e[0] * e[0] + e[1] * e[1]; }
float Vec2::norm()const { return sqrt(norm_squared()); }


//Vec2 相关函数定义部分
std::ostream& operator<<(std::ostream& out, const Vec2& v) {
	return out << v.e[0] << ' ' << v.e[1];
}
Vec2 operator+(const Vec2& u, const Vec2& v) {
	return Vec2(u.e[0] + v.e[0], u.e[1] + v.e[1]);
}
Vec2 operator-(const Vec2& u, const Vec2& v) {
	return Vec2(u.e[0] - v.e[0], u.e[1] - v.e[1]);
}
Vec2 operator*(const Vec2& u, const Vec2& v) {
	return Vec2(u.e[0] * v.e[0], u.e[1] * v.e[1]);
}
Vec2 operator*(float t, const Vec2& v) {
	return Vec2(t * v.e[0], t * v.e[1]);
}
Vec2 operator* (const Vec2& v, float t) {
	return t * v;
}
Vec2 operator/(Vec2 v, float t) {
	return (1 / t) * v;
}


//Vec3 类成员函数定义部分
Vec3::Vec3() :e{ 0,0,0 } {}
Vec3::Vec3(float e0,float e1,float e2) : e{ e0,e1,e2 } {}
float Vec3::x()const { return e[0]; }
float Vec3::y()const { return e[1]; }
float Vec3::z()const { return e[2]; }
Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
float Vec3::operator[](int i)const { return e[i]; }
float& Vec3::operator[](int i) { return e[i]; }
Vec3& Vec3::operator+=(const Vec3& v) {
	e[0] += v[0];
	e[1] += v[1];
	e[2] += v[2];
	return *this;
}
Vec3& Vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}
Vec3& Vec3::operator/=(const float t) {
	return (*this) *= (1 / t);
}
float Vec3::norm_squared()const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
float Vec3::norm() const { return sqrt(norm_squared()); }

//Vec3 相关函数定义部分
std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
Vec3 operator+(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
Vec3 operator-(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
Vec3 operator*(float t, const Vec3& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
Vec3 operator*(const Vec3& v, float t) {
	return t * v;
}
Vec3 operator/(Vec3 v, float t) {
	return (1 / t) * v;
}

float dot(const Vec3& u, const Vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

Vec3 unit_vector(const Vec3& v) {
	return v / v.norm();
}

Vec3 cwise_product(const Vec3& a, const Vec3& b) {
	return Vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}


//Vec4类成员函数定义部分
Vec4::Vec4() :e{ 0,0,0,0 } {}
Vec4::Vec4(float e0, float e1, float e2, float  e3) : e{ e0,e1,e2,e3 } {}
float Vec4::x()const { return e[0]; }
float Vec4::y()const { return e[1]; }
float Vec4::z()const { return e[2]; }
float Vec4::w()const { return e[3]; }
float Vec4::operator[](int i)const { return e[i]; }
float& Vec4::operator[](int i) { return e[i]; }
Vec4& Vec4::operator*=(const float t){
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	e[3] *= t;
	return *this;
}
Vec4& Vec4::operator/=(const float t) {
	return (*this) *= (1 / t);
}

//Vec4相关函数定义部分
Vec4 to_vec4(const Vec3& u, float w) {
	return Vec4(u[0], u[1], u[2], w);
}

std::ostream& operator<<(std::ostream& out, const Vec4& v) {
	return  out << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3];
}

Vec4 operator-(const Vec4& u, const Vec4& v) {
	return Vec4(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2], u.e[3] - v.e[3]);
}

Vec4 operator+(const Vec4& u, const Vec4& v) {
	return Vec4(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2], u.e[3] + v.e[3]);
}

Vec4 operator*(const Vec4& u, const Vec4& v) {
	return Vec4(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2], u.e[3] * v.e[3]);
}

Vec4 operator*(float t, const  Vec4& v) {
	return Vec4(t * v.e[0], t * v.e[1], t * v.e[2], t * v.e[3]);
}

Vec4 operator*(const Vec4& v, float t) {
	return t * v;
}


//Mat3 类成员函数定义部分
Mat3::Mat3() {}
Vec3  Mat3::operator[](int i)const { return rows[i]; }
Vec3& Mat3::operator[](int i) { return rows[i]; }

//Mat3 相关函数定义部分
//矩阵求值
static float mat3_determinant(const Mat3& m) {
	float a = +m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	float b = -m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	float c = +m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
	return a + b + c;
}

//伴随矩阵
static Mat3 mat3_adjoint(const Mat3& m) {
	Mat3 adjoint;
	adjoint[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]);
	adjoint[0][1] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
	adjoint[0][2] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]);
	adjoint[1][0] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
	adjoint[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]);
	adjoint[1][2] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);
	adjoint[2][0] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]);
	adjoint[2][1] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
	adjoint[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]);
	return adjoint;
}

//矩阵初始化
Mat3 Mat3::identity() {
	Mat3 mat;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] = (i == j);
		}
	}
	return mat;
}

//矩阵转置
Mat3 Mat3::transpose() const {
	Mat3 mat;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] = rows[j][i];
		}
	}
	return mat;
}

//逆矩阵转置
Mat3 Mat3::inverse_transpose() const {
	Mat3 inverse_trans, adjoint;
	float  determinant, inv_determinant;

	adjoint = mat3_adjoint(*this);
	determinant = mat3_determinant(*this);
	inv_determinant = 1 / determinant;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			inverse_trans[i][j] = adjoint[i][j] * inv_determinant;		//???为啥可以这样计算得到
		}
	}

	return inverse_trans;
}

//逆矩阵
Mat3 Mat3::inverse() const {
	return ((*this).inverse_transpose()).transpose();
}

std::ostream& operator<<(std::ostream& out, const Mat3& m) {
	return out << m[0] << "\n" << m[1] << "\n" << m[2];
}


//Mat4类成员函数定义部分
Mat4::Mat4() {}
Vec4 Mat4::operator[](int i)const { return rows[i]; }
Vec4& Mat4::operator[](int i) { return rows[i]; }

//Mat4类相关函数定义部分
//求子式阵值
static float mat4_minor(Mat4 m, int r, int c) {
	Mat3 cut_down;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			int row = i < r ? i : i + 1;
			int col = j < c ? j : j + 1;
			cut_down[i][j] = m[row][col];
		}
	}
	return mat3_determinant(cut_down);
}

//求余子式矩阵值
static float mat4_cofactor(Mat4 m, int r, int c) {
	float sign = (r + c) % 2 == 0 ? 1.0f : -1.0f;
	float minor = mat4_minor(m, r, c);
	return sign * minor;
}

//求矩阵值
static Mat4 mat4_adjoint(Mat4 m) {
	Mat4 adjoint;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			adjoint[i][j] = mat4_cofactor(m, i, j);
		}
	}
	return adjoint;
}

//初始化
Mat4 Mat4::identity() {
	Mat4 mat;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			mat[i][j] = (i == j);
		}
	}
	return mat;
}

//逆矩阵
Mat4 Mat4::transpose()const {
	Mat4 transpose;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			transpose[i][j] = rows[j][i];
		}
	}
	return transpose;
}

//逆反矩阵
Mat4 Mat4::inverse_transpose() const {
	float determinant, inv_determinant;
	Mat4 adjoint, inverse_transpose;

	adjoint = mat4_adjoint(*this);
	determinant = 0;
	for (int i = 0; i < 4; ++i) {
		determinant += rows[0][i] * adjoint[0][i];
	}
	inv_determinant = 1 / determinant;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			inverse_transpose[i][j] = adjoint[i][j] * inv_determinant;
		}
	}
	return inverse_transpose;
}

//逆矩阵
Mat4 Mat4::inverse() const {
	return ((*this).inverse_transpose()).transpose();
}

std::ostream& operator<<(std::ostream& out, const Mat4& m) {
	return  out << m[0] << "\n" << m[1] << "\n" << m[2] << "\n" << m[3];
}

Vec4 operator*(const Mat4& m, const Vec4 v) {
	float  product[4];
	for (int i = 0; i < 4; ++i) {
		float a = m[i][0] * v[0];
		float b = m[i][1] * v[1];
		float c = m[i][2] * v[2];
		float d = m[i][3] * v[3];
		product[i] = a + b + c + d;
	}

	return Vec4(product[0], product[1], product[2], product[3]);
}

Mat4 operator*(const Mat4& m1, const Mat4& m2) {
	Mat4 m;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				m[i][j] += (m1[i][k] * m2[k][j]);
			}
		}
	}
	return m;
}


//Mat4相关矩阵定义部分
//位移矩阵
Mat4 mat4_translate(float tx, float ty, float tz) {
	Mat4 m = Mat4::identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
	return m;
}

//缩放矩阵
Mat4 mat4_scale(float sx, float sy, float sz) {
	Mat4 m = Mat4::identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
	return m;
}

//绕x轴旋转的旋转矩阵
Mat4 mat4_rotate_x(float angle) {
	Mat4 m = Mat4::identity();
	angle = angle / 180.0 * PI;
	float c = cos(angle);
	float s = sin(angle);
	m[1][1] = c;
	m[1][2] = -s;
	m[2][1] = s;
	m[2][2] = c;
	return m;
}

//绕y轴旋转的旋转矩阵
Mat4 mat4_rotate_y(float angle) {
	Mat4 m = Mat4::identity();
	angle = angle / 180.0 * PI;
	float c = cos(angle);
	float s = sin(angle);
	m[0][0] = c;
	m[0][2] = s;
	m[2][0] = -s;
	m[2][2] = c;
	return m;
}

//绕z轴旋转的旋转矩阵
Mat4 mat4_rotate_z(float angle) {
	Mat4 m = Mat4::identity();
	angle = angle / 180.0 * PI;
	float c = cos(angle);
	float s = sin(angle);
	m[0][0] = c;
	m[0][1] = -s;
	m[1][0] = s;
	m[1][1] = c;
	return m;
}

//View Transport
Mat4 mat4_lookat(Vec3 eye, Vec3 target, Vec3 up) {
	Mat4 m = Mat4::identity();

	Vec3 z = unit_vector(eye - target);
	Vec3 x = unit_vector(cross(up, z));
	Vec3 y = unit_vector(cross(z, x));

	m[0][0] = x[0];
	m[0][1] = x[1];
	m[0][2] = x[2];

	m[1][0] = y[0];
	m[1][1] = y[1];
	m[1][2] = y[2];

	m[2][0] = z[0];
	m[2][1] = z[1];
	m[2][2] = z[2];

	m[0][3] = -dot(x, eye);
	m[1][3] = -dot(y, eye);
	m[2][3] = -dot(z, eye);

	return m;
}

//正交投影变换矩阵
Mat4 mat4_ortho(float left, float right, float bottom, 
	float top, float near, float far) {
	float x_range = right - left;
	float y_range = top - bottom;
	float z_range = near - far;
	Mat4 m = Mat4::identity();
	m[0][0] = 2 / x_range;
	m[1][1] = 2 / y_range;
	m[2][2] = 2 / z_range;
	m[0][3] = -(left + right) / x_range;
	m[1][3] = -(bottom + top) / y_range;
	m[2][3] = -(near + far) / z_range;
	return m;
}

//透视投影变换矩阵
Mat4 mat4_perspective(float fovy, float aspect, float near, float far) {
	Mat4 m = Mat4::identity();
	fovy = fovy / 180.0 * PI;
	float t = fabs(near) * tan(fovy / 2);
	float r = aspect * t;

	m[0][0] = near / r;
	m[1][1] = near / t;
	m[2][2] = (near + far) / (near - far);
	m[2][3] = 2 * near * far / (far - near);
	m[3][2] = 1;
	m[3][3] = 0;
	return m;
}

//其他功能函数
float float_clamp(float f, float min, float max) {
	return f < min ? min : (f > max ? max : f);
}

float float_max(float a, float b) {
	return a > b ? a : b;
}

float float_min(float a, float b) {
	return a < b ? a : b;
}

//float插值计算
float float_lerp(float start, float end, float alpha) {
	return start + (end - start) * alpha;
}

//vec2插值计算
Vec2 vec2_lerp(Vec2& start, Vec2& end, float alpha) {
	return start + (end - start) * alpha;
}

//vec3插值计算
Vec3 vec3_lerp(Vec3& start, Vec3& end, float alpha) {
	return start + (end - start) * alpha;
}

//vec4插值计算
Vec4 vec4_lerp(Vec4& start, Vec4& end, float alpha) {
	return start + (end - start) * alpha;
}