
class  Vec3f
{
public:
	Vec3f();
	~Vec3f();
	Vec3f(float _xx, float _yy, float _zz);
	Vec3f(const Vec3f& _otherVec3f);

	Vec3f& operator=(const Vec3f& _otherVec3f);

	const Vec3f operator+(const Vec3f& _otherVec3f);
	const Vec3f operator-(const Vec3f& _otherVec3f);
	const Vec3f operator*(const Vec3f& _otherVec3f);

	friend Vec3f operator*  (Vec3f _otherVec3f,   float _nr);
	friend Vec3f operator / (Vec3f _otherVec3f, float _nr);

	Vec3f& operator+=(const Vec3f& _otherVec3f);
	Vec3f& operator-=(const Vec3f& _otherVec3f);
	Vec3f& operator*=(const Vec3f& _otherVec3f);

	const float x() const;
	const float y() const;
	const float z() const;
	float _x;
	float _y;
	float _z;

private:

};
