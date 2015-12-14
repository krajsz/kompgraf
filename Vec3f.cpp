#include "Vec3f.h"

Vec3f::Vec3f()
{
	this->_x = 0;
	this->_y = 0;
	this->_z = 0;
}
Vec3f::Vec3f(float _xx, float _yy, float _zz)
{
	this->_x = _xx;
	this->_y = _yy;
	this->_z = _zz;
}
Vec3f::~Vec3f()
{
}
Vec3f::Vec3f(const Vec3f& _otherVec3f)
{
	_x = _otherVec3f._x;
	_y = _otherVec3f._y;
	_z = _otherVec3f._z;
}
Vec3f& Vec3f::operator=(const Vec3f& _otherVec3f)
{
	if (this != &_otherVec3f)
	{
		_x = _otherVec3f._x;
		_y = _otherVec3f._y;
		_z = _otherVec3f._z;
	}
	return *this;
}
const float Vec3f::x() const
{
	return _x;
}

const float Vec3f::y() const
{
	return _y;
}
const float Vec3f::z() const
{
	return _z;
}

const Vec3f Vec3f::operator+(const Vec3f& _otherVec3f)
{
	return Vec3f(*this) += _otherVec3f;
}
const Vec3f Vec3f::operator-(const Vec3f& _otherVec3f)
{
	return Vec3f(*this) -= _otherVec3f;
}

Vec3f operator/ (Vec3f _vec3, float _constnr)
{
	_vec3._x /= _constnr;
	_vec3._y /= _constnr;
	_vec3._z /= _constnr;
	return _vec3;
}
Vec3f operator*(Vec3f _vec3, float _constnr)
{
	_vec3._x *= _constnr;
	_vec3._y *= _constnr;
	_vec3._z *= _constnr;
	return _vec3;

}
Vec3f& Vec3f::operator+=(const Vec3f& _otherVec3f)
{
	_x = _x + _otherVec3f._x;
	_y = _y + _otherVec3f._y;
	_z = _z + _otherVec3f._z;
	return *this;

}
Vec3f& Vec3f::operator-=(const Vec3f& _otherVec3f)
{
	_x -= _otherVec3f._x;
	_y -= _otherVec3f._y;
	_z -= _otherVec3f._z;
	return *this;
}
Vec3f& Vec3f::operator*=(const Vec3f& _otherVec3f)
{
	_x *= _otherVec3f._x;
	_y *= _otherVec3f._y;
	_z *= _otherVec3f._z;

	return *this;
}