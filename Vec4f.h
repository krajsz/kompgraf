#pragma once

class  Vec4f
{
public:
	Vec4f();
	~Vec4f();
	Vec4f(float _xx, float _yy, float _zz, float _ww);
	Vec4f(const Vec4f& _otherVec4f);

	Vec4f& operator=(const Vec4f& _otherVec4f);

	const Vec4f operator+(const Vec4f& _otherVec4f);
	const Vec4f operator-(const Vec4f& _otherVec4f);
	const Vec4f operator*(const Vec4f& _otherVec4f);

	friend Vec4f operator*  (Vec4f _otherVec4f, float _nr);
	friend Vec4f operator / (Vec4f _otherVec4f, float _nr);

	Vec4f& operator+=(const Vec4f& _otherVec4f);
	Vec4f& operator-=(const Vec4f& _otherVec4f);
	Vec4f& operator*=(const Vec4f& _otherVec4f);

	const float x() const;
	const float y() const;
	const float z() const;
	const float w() const;

	float _x;
	float _y;
	float _z;
	float _w;

private:

};
