#include <array>
#include <iostream>
#include <iomanip>

#include "Vec4f.h"
template <class _Type, size_t _N, size_t _M>
class Matrix
{

public:
	~Matrix() = default;
	Matrix() = default;

    std::array<_Type, _M>& operator[](size_t _index);

    Matrix operator+(const Matrix& _otherMatrix);

    Matrix& operator+=(const Matrix& _otherMatrix);

    Matrix operator-(const Matrix& _otherMatrix);

	template < size_t  _K>
    Matrix<_Type, _N, _K> operator*(const Matrix<_Type, _M, _K>& _otherMatrix);

    Matrix& operator*=(const Matrix& _otherMatrix);

    const _Type& operator() (const size_t& _i, const size_t& _j) const;

    _Type& operator() (const size_t& _i, const size_t& _j);

    void Print();

    double Determinant();

    Matrix<_Type, _N, _M> Inverse();
    const Matrix<_Type, _N,_M> GetInverse();

    const Matrix<_Type, _N, _M> GetTransposed();

    void MultByConst(Matrix<_Type, _N,_M>& _matrix, const _Type& _num);

    Matrix<_Type, _N, _M> Transpose();
    Matrix<_Type, _N,_M> GetMatrix() const;


    friend std::istream& operator>>(std::istream& _in, Matrix& _matr);

    friend std::array<_Type, _N> operator*(Matrix<_Type, _N, _M> _matr, std::array<_Type, _M> _vec);
    friend Matrix<_Type, _N, _M>& operator*(Matrix<_Type, _N, _M>& _matr, _Type _num);

    friend Vec4f operator*(Matrix<float, 4, 4>& _matr, Vec4f _vec);

	std::array<std::array<_Type, _M>, _N > mMatrix;


private:

    void BuildUnitMatrixFrom(Matrix& _from);
};

using Mat1_1i = Matrix<int, 1, 1>;
using Mat1_1f = Matrix<float, 1, 1>;
using Mat1_1d = Matrix<double, 1, 1>;

using Mat1_2i = Matrix<int, 1, 2>;
using Mat1_2f = Matrix<float, 1, 2>;
using Mat1_2d = Matrix<double, 1, 2>;

using Mat2_1i = Matrix<int, 2, 1>;
using Mat2_1f = Matrix<float, 2, 1>;
using Mat2_1d = Matrix<double, 2, 1>;

using Mat2_2i = Matrix<int, 2, 2>;
using Mat2_2f = Matrix<float, 2, 2>;
using Mat2_2d = Matrix<double, 2, 2>;

using Mat2_3i = Matrix<int, 2, 3>;
using Mat2_3f = Matrix<float, 2, 3>;
using Mat2_3d = Matrix<double, 2, 3>;

using Mat2_4i = Matrix<int, 2, 4>;
using Mat2_4f = Matrix<float, 2, 4>;
using Mat2_4d = Matrix<double, 2, 4>;

using Mat3_1i = Matrix<int, 3, 1>;
using Mat3_1f = Matrix<float, 3, 1>;
using Mat3_1d = Matrix<double, 3, 1>;

using Mat3_2i = Matrix<int, 3, 2>;
using Mat3_2f = Matrix<float, 3, 2>;
using Mat3_2d = Matrix<double, 3, 2>;

using Mat3_3i = Matrix<int, 3, 3>;
using Mat3_3f = Matrix<float, 3, 3>;
using Mat3_3d = Matrix<double, 3, 3>;

using Mat3_4i = Matrix<int, 3, 4>;
using Mat3_4f = Matrix<float, 3, 4>;
using Mat3_4d = Matrix<double, 3, 4>;

using Mat4_3i = Matrix<int, 4, 3>;
using Mat4_3f = Matrix<float, 4, 3>;
using Mat4_3d = Matrix<double, 4, 3>;

using Mat4_1i = Matrix<int, 4, 1>;
using Mat4_1f = Matrix<float, 4, 1>;
using Mat4_1d = Matrix<double, 4, 1>;

using Mat4_4i = Matrix<int, 4, 4>;
using Mat4_4f = Matrix<float, 4, 4>;
using Mat4_4d = Matrix<double, 4, 4>;

using Mat4_5i = Matrix<int, 4, 5>;
using Mat4_5f = Matrix<float, 4, 5>;
using Mat4_5d = Matrix<double, 4, 5>;

using Mat5_5i = Matrix<int, 5, 5>;
using Mat5_5f = Matrix<float, 5, 5>;
using Mat5_5d = Matrix<double, 5, 5>;

using Mat6_6i = Matrix<int, 6, 6>;
using Mat6_6f = Matrix<float, 6, 6>;
using Mat6_6d = Matrix<double, 6, 6>;

using Mat7_7i = Matrix<int, 7, 7>;
using Mat7_7f = Matrix<float, 7, 7>;
using Mat7_7d = Matrix<double, 7, 7>;
