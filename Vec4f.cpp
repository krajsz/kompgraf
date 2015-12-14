#include "Vec4f.h"

Vec4f::Vec4f()
{
	this->_x = 0;
	this->_y = 0;
	this->_z = 0;
	this->_w = 0;
}
Vec4f::Vec4f(float _xx, float _yy, float _zz, float _ww)
{
	this->_x = _xx;
	this->_y = _yy;
	this->_z = _zz;
	this->_w = _ww;
}
Vec4f::~Vec4f()
{
}
Vec4f::Vec4f(const Vec4f& _otherVec4f)
{
	_x = _otherVec4f._x;
	_y = _otherVec4f._y;
	_z = _otherVec4f._z;
	_w = _otherVec4f._w;
}
Vec4f& Vec4f::operator=(const Vec4f& _otherVec4f)
{
	if (this != &_otherVec4f)
	{
		_x = _otherVec4f._x;
		_y = _otherVec4f._y;
		_z = _otherVec4f._z;
		_w = _otherVec4f._w;
	}
	return *this;
}
const float Vec4f::x() const
{
	return _x;
}

const float Vec4f::y() const
{
	return _y;
}
const float Vec4f::z() const
{
	return _z;
}

const float Vec4f::w() const
{
	return _w;
}

const Vec4f Vec4f::operator+(const Vec4f& _otherVec4f)
{
	return Vec4f(*this) += _otherVec4f;
}
const Vec4f Vec4f::operator-(const Vec4f& _otherVec4f)
{
	return Vec4f(*this) -= _otherVec4f;
}

Vec4f operator/ (Vec4f _vec4, float _constnr)
{
	_vec4._x /= _constnr;
	_vec4._y /= _constnr;
	_vec4._z /= _constnr;
	_vec4._w /= _constnr;
	
	return _vec4;
}
Vec4f operator*(Vec4f _vec4, float _constnr)
{
	_vec4._x *= _constnr;
	_vec4._y *= _constnr;
	_vec4._z *= _constnr;
	_vec4._w *= _constnr;
	return _vec4;

}
Vec4f& Vec4f::operator+=(const Vec4f& _otherVec4f)
{
	_x += _otherVec4f._x;
	_y += _otherVec4f._y;
	_z += _otherVec4f._z;
	_w += _otherVec4f._w;
	return *this;

}
Vec4f& Vec4f::operator-=(const Vec4f& _otherVec4f)
{
	_x -= _otherVec4f._x;
	_y -= _otherVec4f._y;
	_z -= _otherVec4f._z;
	_w -= _otherVec4f._w;
	return *this;
}
Vec4f& Vec4f::operator*=(const Vec4f& _otherVec4f)
{
	_x *= _otherVec4f._x;
	_y *= _otherVec4f._y;
	_z *= _otherVec4f._z;
	_w *= _otherVec4f._w;

	return *this;
}