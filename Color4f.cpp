#include "Color4f.h"

Color4f::Color4f()
{
	this->_r = 0;
	this->_g = 0;
	this->_b = 0;
	this->_a = 0;
}

Color4f::Color4f(GLfloat _rr, GLfloat _gg, GLfloat _bb, GLfloat _aa)
{
	this->_r = _rr / 255.0f;
	this->_g = _gg / 255.0f;
	this->_b = _bb / 255.0f;
	this->_r = _aa / 255.0f;

    this->data = new GLfloat[4];
	this->data[0] = _r;
	this->data[1] = _g;
	this->data[2] = _b;
	this->data[3] = _a;
}

Color4f::~Color4f()
{
}

Color4f::Color4f(const Color4f& _otherColor4f)
{
	_r = _otherColor4f._r;
	_g = _otherColor4f._g;
	_b = _otherColor4f._b;
	_a = _otherColor4f._a;
}

Color4f& Color4f::operator=(const Color4f& _otherColor4f)
{
	if (this != &_otherColor4f)
	{
		_r = _otherColor4f._r;
		_g = _otherColor4f._g;
		_b = _otherColor4f._b;
		_a = _otherColor4f._a;
	}
	return *this;
}

const GLfloat Color4f::r() const
{
	return _r;
}

const GLfloat Color4f::g() const
{
	return _g;
}
const GLfloat Color4f::b() const
{
	return _b;
}

const GLfloat Color4f::a() const
{
	return _a;
}

const Color4f Color4f::operator+(const Color4f& _otherColor4f)
{
	return Color4f(*this) += _otherColor4f;
}

const Color4f Color4f::operator-(const Color4f& _otherColor4f)
{
	return Color4f(*this) -= _otherColor4f;
}

Color4f operator/ (Color4f _vec4, GLfloat _constnr)
{
	_vec4._r /= _constnr;
	_vec4._g /= _constnr;
	_vec4._b /= _constnr;
	_vec4._a /= _constnr;

	return _vec4;
}

Color4f operator*(Color4f _vec4, GLfloat _constnr)
{
	_vec4._r *= _constnr;
	_vec4._g *= _constnr;
	_vec4._b *= _constnr;
	_vec4._a *= _constnr;
	return _vec4;

}

Color4f& Color4f::operator+=(const Color4f& _otherColor4f)
{
	_r += _otherColor4f._r;
	_g += _otherColor4f._g;
	_b += _otherColor4f._b;
	_a += _otherColor4f._a;
	return *this;
}

Color4f& Color4f::operator-=(const Color4f& _otherColor4f)
{
	_r -= _otherColor4f._r;
	_g -= _otherColor4f._g;
	_b -= _otherColor4f._b;
	_a -= _otherColor4f._a;
	return *this;
}

Color4f& Color4f::operator*=(const Color4f& _otherColor4f)
{
	_r *= _otherColor4f._r;
	_g *= _otherColor4f._g;
	_b *= _otherColor4f._b;
	_b *= _otherColor4f._a;

	return *this;
}
