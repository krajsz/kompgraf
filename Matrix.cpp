#include "Matrix.h"

template <class _Type, size_t _N, size_t _M>
std::array<_Type, _M>& Matrix<_Type, _N, _M>::operator[](size_t _index)
{
    return mMatrix[_index];
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N, _M> Matrix<_Type, _N, _M>::operator+(const Matrix& _otherMatrix)
{
    Matrix _result;

    for (size_t i = 0; i< _result.mDimension; i++)
    {
        for (size_t j = 0; j < _result.mDimension; j++)
        {
            _result(i, j) = mMatrix[i][j] + _otherMatrix(i, j);
        }
    }
    return _result;
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N, _M>& Matrix<_Type, _N, _M>::operator+=(const Matrix<_Type, _N, _M>& _otherMatrix)
{
    for (size_t i = 0; i<_otherMatrix.GetDimension(); i++)
    {
        for (size_t j = 0; j < _otherMatrix.GetDimension(); j++)
        {
            mMatrix[i][j] += _otherMatrix(i, j);
        }
    }

    return *this;
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N, _M> Matrix<_Type, _N, _M>::operator-(const Matrix<_Type, _N, _M>& _otherMatrix)
{
    Matrix _result;

    for (size_t i = 0; i< _result.mDimension; i++)
    {
        for (size_t j = 0; j < _result.mDimension; j++)
        {
            _result(i, j) = mMatrix[i][j] - _otherMatrix(i, j);
        }
    }

    return _result;
}

template <class _Type, size_t _N, size_t _M>
template < size_t  _K>
Matrix<_Type, _N, _K> Matrix<_Type, _N, _M>::operator*(const Matrix<_Type, _M, _K>& _otherMatrix)
{
    Matrix<_Type, _N, _K> _result;

    for (size_t i = 0; i< _N; i++)
    {
        for (size_t j = 0; j < _K; j++)
        {
            _result.mMatrix[i][j] = 0;
            for (size_t k = 0; k < _M; k++)
            {
                _result.mMatrix[i][j] += mMatrix[i][k] * _otherMatrix.mMatrix[k][j];
            }
        }
    }
    return _result;
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N, _M>& Matrix<_Type, _N, _M>::operator*=(const Matrix<_Type, _N, _M>& _otherMatrix)
{
    return (*this) = (*this) * _otherMatrix;
}


template <class _Type, size_t _N, size_t _M>
const _Type& Matrix<_Type, _N, _M>::operator() (const size_t& _i, const size_t& _j) const
{
    return mMatrix[_i][_j];
}

template <class _Type, size_t _N, size_t _M>
_Type& Matrix<_Type, _N, _M>::operator() (const size_t& _i, const size_t& _j)
{
    return mMatrix[_i][_j];
}

template <class _Type, size_t _N, size_t _M>
void Matrix<_Type, _N, _M>::Print()
{
    for (auto it : mMatrix)
    {
        for (auto i : it)
        {
            std::cout << std::setw(6) << i << " ";
        }
        std::cout << "\n";
    }
}

template <class _Type, size_t _N, size_t _M>
double Matrix<_Type, _N, _M>::Determinant()
{
    double _x = 0;
    double d = 1.0;
    double mDeterminant = 1.0;
    Matrix _tmp = *this;

    if (_N == _M)
    {
        for (size_t i = 0; i < _N; i++)
        {
            if (_tmp[i][i] == 0)
            {
                for (size_t j = i + 1; j < _M; j++) // mColumns
                {
                    if (_tmp[j][i] != 0)
                    {
                        //							std::cout << "SWAP" << i << " " << j << std::endl;
                        _tmp[j].swap(_tmp[i]);
                        d *= -1.0;
                        break;
                    }
                }
            }
            for (size_t k = i + 1; k < _N; k++)
            {
                _x = -_tmp[k][i] / _tmp[i][i];

                for (size_t j = i; j < _N; j++)
                {
                    _tmp[k][j] += _x * _tmp[i][j];
                }
                _tmp[k][i] = _x*_tmp[k][i];
            }
        }
        for (size_t i = 0; i < _N; i++)
        {
            mDeterminant *= _tmp[i][i];
        }
        //			PrintMatrix(_tmp);
    }
    else
    {
        //std::cout << "Not negyzetes!" << std::endl;
    }
    if (mDeterminant == 0)
    {
        return 0;
    }
    else

        return mDeterminant*d;
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N, _M> Matrix<_Type, _N, _M>::Inverse()
{
    Matrix<_Type, _N, _M> _unitMatr = { 0 };
    Matrix<_Type, _N, _M> _tmp;
    Matrix<_Type, _N, _M> mInverse = *this;

    if (_N == _M)
    {
        if (Determinant() != 0)
        {
            if (_N == 2)
            {
                std::swap(mInverse.mMatrix[0][0], mInverse[1][1]);
                std::swap(mInverse[0][1], mInverse[1][0]);

                mInverse[0][1] *= -1;
                mInverse[1][0] *= -1;
                const _Type _det = mInverse[0][0] * mInverse[1][1] - mInverse[0][1] * mInverse[1][0];
                MultByConst(mInverse, 1 / _det);

                return mInverse;
            }
            else if (_N > 2)
            {
                for (size_t i = 0; i < _N; i++)
                {
                    _unitMatr[i][i] = 1;
                }

                //					std::cout << "UnitMatrix: " << std::endl;
                //PrintMatrix(_unitMatr);
                _tmp = *this;;
                //	std::cout << "_tmpMat: " << std::endl;
                //	PrintMatrix(_tmp);
                _Type _x;
                // foatlo
                for (size_t i = 0; i < _N; i++)
                {
                    if (_tmp[i][i] == 0)
                    {
                        for (size_t j = i + 1; j < _M; j++) // mColumns
                        {
                            if (_tmp[j][i] != 0)
                            {
                                //									std::cout << "SWAP" << i << " " << j << std::endl;

                                std::swap(_tmp[j], _tmp[i]);
                                std::swap(_unitMatr[j], _unitMatr[i]);

                                break;
                            }
                        }
                    }
                    _Type oszt = _tmp[i][i];
                    for (size_t l = 0; l < _N; l++)
                    {
                        _unitMatr[i][l] /= oszt;
                        _tmp[i][l] /= oszt;

                    }

                    for (size_t k = 0; k < _N; k++)
                    {
                        if (k == i)
                        {
                            continue;
                        }
                        _x = -_tmp[k][i];

                        for (size_t j = 0; j < _N; j++)
                        {

                            _tmp[k][j] += _x * _tmp[i][j];
                            _unitMatr[k][j] += _x* _unitMatr[i][j];

                        }
                    }
                    /*std::cout << "i done: " << std::endl;
                    PrintMatrix(_unitMatr);*/
                }
                /*std::cout << "_tmpInverse: " << std::endl;
                PrintMatrix(_tmp);
                std::cout << "_unitMatr" << std::endl;
                PrintMatrix(_unitMatr);*/

            }
            return _unitMatr;

        }
    }
    else
    {
        //std::cout << "Nem kepezheto inverz, nem negyzetes!" << std::endl;
        return *this;
    }
    return *this;
}

template <class _Type, size_t _N, size_t _M>
const Matrix<_Type, _N,_M> Matrix<_Type, _N, _M>::GetInverse()
{
    return (*this).Inverse();
}

template <class _Type, size_t _N, size_t _M>
const Matrix<_Type, _N, _M> Matrix<_Type, _N, _M>::GetTransposed()
{
    return (*this).Transpose();
}

template <class _Type, size_t _N, size_t _M>
void Matrix<_Type, _N, _M>::MultByConst(Matrix<_Type, _N,_M>& _matrix, const _Type& _num)
{
    for (size_t i = 0; i < _N; i++)
    {
        for (size_t j = 0; j< _M; j++)
        {
            _matrix[i][j] *= _num;
        }
    }
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N, _M> Matrix<_Type, _N, _M>::Transpose()
{
    /*Matrix<_Type, _N, _M> mTransposed;
    if (mRows == mColumns)
    {
        for (size_t i = 0; i < mRows; i++)
        {
            for (size_t j = 0; j < i; j++)
            {
                if (i != j)
                {
                    std::swap(mTransposed[i][j], mTransposed[j][i]);
                }
            }
        }
    }
    else
    {
        //std::cout << "Rows != Columns" << std::endl;
    }
    return mTransposed;*/
}

template <class _Type, size_t _N, size_t _M>
Matrix<_Type, _N,_M> Matrix<_Type, _N, _M>::GetMatrix() const
{
    return mMatrix;
}

template <class _Type, size_t _N, size_t _M>

std::istream& operator>>(std::istream& _in, Matrix<_Type, _N, _M>& _matr)
{
    for (size_t i = 0; i< _N; i++)
    {
        for (size_t j = 0; j < _M; j++)
        {
            _in >> _matr.mMatrix[i][j];

        }
    }
    return _in;
}

template <class _Type, size_t _N, size_t _M>
std::array<_Type, _N> operator*(Matrix<_Type, _N, _M> _matr, std::array<_Type, _M> _vec)
{
    std::array<_Type, _N> _tmpVec;
    //std::cout << "Szorozing!" << std::endl;
    for (size_t i = 0; i< _N; i++)
    {
        for (size_t j = 0; j < _M; j++)
        {
            _tmpVec[i] += _vec[j] * _matr.mMatrix[i][j];
        }
    }

    return _tmpVec;
}

template <class _Type, size_t _N, size_t _M,size_t  _K>
Matrix<_Type, _N, _M>& operator*(Matrix<_Type, _N, _M>& _matr, _Type _num)
{
    for (size_t i = 0; i< _matr.GetRows(); i++)
    {
        for (size_t j = 0; j < _matr.GetColumns(); j++)
        {
            _matr(i, j) *= _num;
        }
    }
}

Vec4f operator*(Matrix<float, 4, 4>& _matr, Vec4f _vec)
{
    Vec4f _result = Vec4f(

         _matr[0][0] * _vec.x() + _matr[0][1] * _vec.y() + _matr[0][2] * _vec.z() + _matr[0][3] * _vec.w(),
         _matr[1][0] * _vec.x() + _matr[1][1] * _vec.y() + _matr[1][2] * _vec.z() + _matr[1][3] * _vec.w(),
         _matr[2][0] * _vec.x() + _matr[2][1] * _vec.y() + _matr[2][2] * _vec.z() + _matr[2][3] * _vec.w(),
         _matr[3][0] * _vec.x() + _matr[3][1] * _vec.y() + _matr[3][2] * _vec.z() + _matr[3][3] * _vec.w()
        );
    return _result;
}

template <class _Type, size_t _N, size_t _M>
void Matrix<_Type, _N, _M>::BuildUnitMatrixFrom(Matrix& _from)
{
    for (size_t i = 0; i < _from.size(); i++)
    {
        for (size_t j = 0; j < _from.size(); j++)
        {
            if (i == j)
            {
                _from[i][j] = 1;
            }
            else
            {
                _from[i][j] = 0;
            }
        }
    }
}

