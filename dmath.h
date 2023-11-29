//--------------------------------------------------------------------------------------------------
// Usefull mathematical tools
// includes Eingen
// includes boost::math
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#ifndef _MAT_DVG_ALGO_
#define _MAT_DVG_ALGO__

#include <vector>
#include <map>
#include <string>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <boost/math/interpolators/cardinal_quintic_b_spline.hpp>
#include <boost/math/interpolators/bilinear_uniform.hpp>
#include "Eigen"
#include "Dense"


namespace math_util
{

	using cubic_spline = boost::math::interpolators::cardinal_cubic_b_spline<double>;
	using quadradic_spline = boost::math::interpolators::cardinal_quintic_b_spline<double>;

	quadradic_spline quadradicspline(std::vector<double>& y, double t0, double time_step);
	cubic_spline cubicspline(std::vector<double>& y, double t0, double time_step);

	std::vector<double> mergeTwo(std::vector<double>& x, std::vector<double>& y);

	// Adapted from
	// https://www.codewithc.com/c-program-for-linear-exponential-curve-fitting/
	// It performs an exponential regression
	int
		exponentialRegression(const std::vector<double>& x,
			const std::vector<double>& y,
			double& a,
			double& b);

	// adapted from
	// https://cp-algorithms.com/linear_algebra/linear-system-gauss.html
	// Solves a linear system of size n, implicit at the vector a size
	// I was looking for an algorithm for this when I stumbled upon the
	// C++ code so why do it myself :-) ?
	// vector a is a augmented matrix where AX=Y
	// AY form a new matrix
	template<typename T >
	int
		gauss(std::vector < std::vector<T> >& a,
			std::vector<T>& ans);


	// My custom matrix class 
	template<typename T>
	class Dmatrix final
	{

	public:

		Dmatrix() {};
		Dmatrix(int _N, int _M) :N(_N), M(_M)
		{
			for (int i = 0; i < _N; i++)
			{
				std::vector<T> tmp(_M, 0);
				data.emplace_back(tmp);
			}
		};

		explicit Dmatrix(int _M) :M(_M)
		{
			this->N = 1;
			std::vector<T> tmp(M, 0);
			data.push_back(tmp);

		};

		Dmatrix(const Dmatrix& m)
		{
			this->N = m.N;
			this->M = m.M;
			this->data = m.data;
		}

		Dmatrix(std::vector<std::vector<T>>& data, int _N, int _M)
		{
			this->N = _N;
			this->M = _M;
			this->data = data;
		}

		Dmatrix& operator=(const Dmatrix& m)
		{
			this->N = m.N;
			this->M = m.M;
			this->data = m.data;
			return *this;
		}

		Dmatrix(Dmatrix&& m) noexcept
		{
			this->N = m.N;
			this->M = m.M;
			copy_data(m.data, this->data);
			m.N = 0;
			m.M = 0;
			m.data.clear();
		}

		Dmatrix& operator=(Dmatrix&& m)
		{

			this->N = m.N;
			this->M = m.M;
			m.N = 0;
			m.M = 0;
			copy_data(m.data, this->data);
			m.data.clear();
			return *this;
		}

		// Access the individual elements                                                                                                                                                                                               
		T& operator()(int& row, const int& col)
		{
			return this->data[row][col];
		}

		const T& operator()(const int& row, const int& col) const
		{
			return this->data[row][col];
		}

		std::vector<std::vector<T>>& getdata() { return this->data; };
		const int& rows() const { return this->N; };
		const int& cols() const { return this->M; };

		friend std::ostream& operator<<(std::ostream& os, const Dmatrix& m)
		{

			for (int i = 0; i < m.N; i++)
			{
				for (int j = 0; j < m.M; j++)
				{
					os << m.data[i][j] << " ";
				}
				os << "\n";
			}

			return os;
		}

		friend Dmatrix operator+(const Dmatrix& m1, const Dmatrix& m2)
		{
			Dmatrix<T> m3(m1.rows(), m1.cols());
			if ((m1.rows() == m2.rows()) && (m1.cols() == m2.cols()))
			{
				for (int i = 0; i < m1.rows(); i++)
				{
					for (int j = 0; j < m1.cols(); j++)
					{
						m3(i, j) = m1(i, j) + m2(i, j);
					}
				}
				return m3;
			}

			return m3;
		}

		friend Dmatrix operator-(const Dmatrix& m1, const Dmatrix& m2)
		{

			Dmatrix<T> m3(m1.rows(), m1.cols());
			if ((m1.rows() == m2.rows()) && (m1.cols() == m2.cols()))
			{
				for (int i = 0; i < m1.rows(); i++)
				{
					for (int j = 0; j < m1.cols(); j++)
					{
						m3(i, j) = m1(i, j) - m2(i, j);
					}
				}
				return m3;
			}

			return m3;
		}

		friend Dmatrix operator*(const Dmatrix& m1, const Dmatrix& m2)
		{
			Dmatrix<T> c(m1.rows(), m2.cols());
			if (m1.cols() != m2.rows())
			{
				return c;
			}
			else
			{
				for (int i = 0; i < m1.rows(); i++)
				{
					for (int j = 0; j < m2.cols(); j++)
					{
						for (int k = 0; k < m2.rows(); k++)
						{
							c(i, j) = c(i, j) + (m1(i, k) * m2(k, j));
						}
					}
				}
			}
			return c;
		}

	private:

		std::vector<std::vector<T>> data;
		int N = 0;
		int M = 0;

		void copy_data(const std::vector<std::vector<T>>& orig, std::vector<std::vector<T>>& dest)
		{
			for (const auto& it : orig)
			{
				dest.emplace_back(it);
			}
		};
	};

	// multiplies a matrix by a value ct
	template<typename M, typename T>
	void
		times(Dmatrix<M>& a, T ct);

	// converts a vector of a vector to a matrix
	Dmatrix<double>
		convertVector2Matrix(std::vector<std::vector<double>>& data);

	// converts a matrix to a vector of a vector
	std::vector<std::vector<double>>
		convertMatrix2vector(Dmatrix<double>& mat);

	// converts a vector ( typically a collumn of a csv file )
	// to a matrix
	Dmatrix<double>
		convertcol2Matrix(std::vector<double>& y);

	// transpose NxN matrix and 1xN or Nx1
	template<typename T>
	Dmatrix<T>
		transpose(const Dmatrix<T>& m1);

}
#endif
