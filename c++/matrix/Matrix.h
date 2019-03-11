#ifndef MATRIX_H
#  define MATRIX_H
#include <initializer_list>
#include <algorithm>
#include <array>
#include <memory>
#include <numeric>
#include <iostream>
#include <sstream>
#include <iomanip>

#undef minor // Some compilers have a macro named minor

using std::initializer_list;
using std::array;
using std::unique_ptr;
using std::max_element;
using std::accumulate;
using std::inner_product;
using std::ostream;
using std::ostringstream;
using std::streamsize;
using std::setw;
using std::endl;
using std::max;


namespace mpcs51044 {

template<int rows, int cols = rows>
class Matrix {
public:
	Matrix() : data{} {}//data是array of array
	Matrix(initializer_list<initializer_list<double>> init) {//列初始化
		auto dp = data.begin();
		for (auto row : init) {
			std::copy(row.begin(), row.end(), dp->begin());
			dp++;
		}
	}
	double& operator()(int x, int y) {
		return data[x][y];
	}

	double operator()(int x, int y) const {
		return data[x][y];
	}

	inline friend//重载<<的不能是成员函数，所以要(inline) friend。
		ostream& operator<<//重载<<符号，使得可以输出。和sales_data里面的print差不多
		//只不过一个是print函数写参数，一个是写在重载符号的左边和右边。类似函数。
		(ostream &os, const Matrix<rows, cols> &m) {
		size_t width = m.longestElementSize() + 2;
		os << "[ " << endl;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				os << setw(static_cast<streamsize>(width)) << m(i, j);
			}
			os << endl;
		}
		os << "]" << endl;
		return os;
	}

	template<int b, int c>
	inline Matrix<b, c>&
	operator+=(Matrix<b, c> const &r)//矩阵加法2
	{
		for (int i = 0; i < b; i++) {
			for (int j = 0; j < c; j++) {
				data[i][j]+=r(i,j);
			}
		}
		return *this;
	}

	Matrix<rows - 1, cols - 1> minor(int r, int c) const {//删除某一行、某一列
		Matrix<rows - 1, cols - 1> result;
		for (int i = 0; i < rows; i++) {//做法是相等就跳过，不等就复制
			if (i == r) {
				continue;
			}
			for (int j = 0; j < cols; j++) {
				if (j == c) {
					continue;
				}
				result(i < r ? i : i - 1, j < c ? j : j - 1) = data[i][j];
			}
		}
		return result;
	}

	double determinant() const {//计算行列式
		double val = 0;
		for (int i = 0; i < rows; i++) {
			val += (i % 2 ? -1 : 1)
				* data[i][0]
				* minor(i, 0).determinant();
		}
		return val;
	}

private:
	static size_t accumulateMax(size_t acc, double d) {
		ostringstream ostr;
		ostr << d;
		return std::max(acc, ostr.str().size());
	}
	static size_t accumulateMaxRow(size_t acc, array<double, cols> row) {
		return std::max(acc, accumulate(row.begin(), row.end(), static_cast<size_t>(0), accumulateMax));
	}
	size_t longestElementSize() const {
		return accumulate(data.begin(), data.end(), 0, accumulateMaxRow);
	}
	array<array<double, cols>, rows> data;
};

template<>
double
Matrix<1, 1>::determinant() const//1*1矩阵特化
{
	return data[0][0];
}

template<>
double
Matrix<2, 2>::determinant() const//2*2矩阵特化
{
	return data[0][0]*data[1][1]-data[1][0]*data[0][1];
}

template<int a, int b, int c>
inline Matrix<a, c>
operator*(Matrix<a, b> const &l, Matrix<b, c> const &r)//矩阵乘法
{
	Matrix<a, c> result;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < c; j++) {
			double total = 0;
			for (int k = 0; k < b; k++)
				total += l(i, k) * r(k, j);
			result(i, j) = total;
		}
	}
	return result;
}

template<int a, int b, int c>
inline Matrix<a, c>
operator+(Matrix<a, b> const &l, Matrix<b, c> const &r)//矩阵加法
{
	Matrix<a, c> result;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < c; j++) {
			result(i, j) = l(i,j)+r(i,j);
		}
	}
	return result;
}

}
#endif