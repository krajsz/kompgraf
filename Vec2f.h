
class  Vec2f
{
public:
	 Vec2f();
	~ Vec2f();
	Vec2f(float _xx, float _yy);
	Vec2f(const Vec2f& _otherVec2f);
	
	Vec2f& operator=(const Vec2f& _otherVec2f);

	const Vec2f operator+(const Vec2f& _otherVec2f);
	const Vec2f operator-(const Vec2f& _otherVec2f);
	const Vec2f operator*(const Vec2f& _otherVec2f);

	friend Vec2f operator*(Vec2f _otherVec2f, float _nr);
	friend Vec2f operator / (Vec2f _vec2f, float _nr);

	Vec2f& operator+=(const Vec2f& _otherVec2f);
	Vec2f& operator-=(const Vec2f& _otherVec2f);
	Vec2f& operator*=(const Vec2f& _otherVec2f);
	
	const float x() const;
	const float y() const;
	float _x;
	float _y;
private:
	
};

using Point = Vec2f;
