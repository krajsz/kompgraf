
using GLfloat = float;

class  Color4f
{
public:
	Color4f();
	~Color4f();
	Color4f(GLfloat _xx, GLfloat _yy, GLfloat _zz, GLfloat _ww);
	Color4f(const Color4f& _otherColor4f);

	Color4f& operator=(const Color4f& _otherColor4f);

	const Color4f operator+(const Color4f& _otherColor4f);
	const Color4f operator-(const Color4f& _otherColor4f);
	const Color4f operator*(const Color4f& _otherColor4f);

	friend Color4f operator*  (Color4f _otherColor4f, GLfloat _nr);
    friend Color4f operator/ (Color4f _otherColor4f, GLfloat _nr);

	Color4f& operator+=(const Color4f& _otherColor4f);
	Color4f& operator-=(const Color4f& _otherColor4f);
	Color4f& operator*=(const Color4f& _otherColor4f);

	const GLfloat r() const;
	const GLfloat g() const;
	const GLfloat b() const;
	const GLfloat a() const;

	GLfloat _r;
	GLfloat _g;
	GLfloat _b;
	GLfloat _a;
	
	
    GLfloat* data;
private:

};
