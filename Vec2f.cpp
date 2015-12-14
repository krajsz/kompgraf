#include "Vec2f.h"

Vec2f::Vec2f()
{
	this->_x = 0;
	this->_y = 0;
}
Vec2f::Vec2f(float _xx, float _yy)
{
	this->_x = _xx;
	this->_y = _yy;
}
Vec2f::~Vec2f()
{
}
Vec2f::Vec2f(const Vec2f& _otherVec2f)
{
	_x = _otherVec2f._x;
	_y = _otherVec2f._y;
}
Vec2f& Vec2f::operator=(const Vec2f& _otherVec2f)
{
	if (this != &_otherVec2f)
	{
		_x = _otherVec2f._x;
		_y = _otherVec2f._y;
	}
	return *this;
}
const float Vec2f::x() const
{
	return _x;
}

const float Vec2f::y() const
{
	return _y;
}

const Vec2f Vec2f::operator+(const Vec2f& _otherVec2f)
{
	return Vec2f(*this) += _otherVec2f;
}
const Vec2f Vec2f::operator-(const Vec2f& _otherVec2f)
{
	return Vec2f(*this) -= _otherVec2f;
}

Vec2f operator/ (Vec2f _vec2, float _constnr)
{
	_vec2._x /= _constnr;
	_vec2._y /= _constnr;
	return _vec2;
}
Vec2f operator*(Vec2f _vec2, float _constnr)
{
	_vec2._x *= _constnr;
	_vec2._y *= _constnr;
	return _vec2;

}
Vec2f& Vec2f::operator+=(const Vec2f& _otherVec2f)
{
	_x = _x +  _otherVec2f._x;
	_y = _y +  _otherVec2f._y;
	return *this;

}
Vec2f& Vec2f::operator-=(const Vec2f& _otherVec2f)
{
	_x -= _otherVec2f._x;
	_y -= _otherVec2f._y;
	return *this;
}
Vec2f& Vec2f::operator*=(const Vec2f& _otherVec2f)
{
	_x *= _otherVec2f._x;
	_y *= _otherVec2f._y;
	return *this;
}