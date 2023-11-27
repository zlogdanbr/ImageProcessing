#include "dmath.h"

namespace math_util
{
	quadradic_spline quadradicspline(std::vector<double>& y, double t0, double time_step)
	{
		quadradic_spline p1(y.data(), y.size(), t0, time_step);
		return p1;
	}

	cubic_spline cubicspline(std::vector<double>& y, double t0, double time_step)
	{
		cubic_spline p1(y.data(), y.size(), t0, time_step);
		return p1;
	}

	template<typename T1, typename T2>
	auto Sub(T1&& t1, T2&& t2) -> decltype(std::forward<T1>(t1) - std::forward<T2>(t2))
	{
		return std::forward<T1>(t1) - std::forward<T2>(t2);
	}

	std::vector<double> mergeTwo(std::vector<double>& x, std::vector<double>& y)
	{
		std::vector<double> f;
		return f;
	}

	// Adapted from
	// https://www.codewithc.com/c-program-for-linear-exponential-curve-fitting/
	// It performs an exponential regression
	int
		exponentialRegression(const std::vector<double>& x,
			const std::vector<double>& y,
			double& a,
			double& b)
	{

		double sumx = 0;
		double sumy = 0;
		double sumxy = 0;
		double sumx2 = 0;
		double A;
		std::vector<double> Y;

		for (const auto& item : y)
		{
			double tmp = item;
			Y.push_back(log(tmp));
		}

		int n = static_cast<int>(Y.size());

		for (int i = 0; i <= n - 1; i++)
		{
			sumx = sumx + x[i];
			sumx2 = sumx2 + x[i] * x[i];
			sumy = sumy + Y[i];
			sumxy = sumxy + x[i] * Y[i];

		}

		A = ((sumx2 * sumy - sumx * sumxy) * 1.0 / (n * sumx2 - sumx * sumx) * 1.0);
		b = ((n * sumxy - sumx * sumy) * 1.0 / (n * sumx2 - sumx * sumx) * 1.0);
		a = exp(A);
		//printf("The curve is Y= %4.3fe^%4.3fX\n", a, b);
		return 0;
	}

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
			std::vector<T>& ans)
	{
		const double EPS = 1e-9;
		const int INF = 2;
		int n = (int)a.size();
		int m = (int)a[0].size() - 1;

		std::vector<int> where(m, -1);
		for (int col = 0, row = 0; col < m && row < n; ++col)
		{
			int sel = row;
			for (int i = row; i < n; ++i)
				if (abs(a[i][col]) > abs(a[sel][col]))
					sel = i;
			if (abs(a[sel][col]) < EPS)
				continue;
			for (int i = col; i <= m; ++i)
				swap(a[sel][i], a[row][i]);
			where[col] = row;

			for (int i = 0; i < n; ++i)
				if (i != row) {
					T c = a[i][col] / a[row][col];
					for (int j = col; j <= m; ++j)
						a[i][j] -= a[row][j] * c;
				}
			++row;
		}

		ans.assign(m, 0);
		for (int i = 0; i < m; ++i)
			if (where[i] != -1)
				ans[i] = a[where[i]][m] / a[where[i]][i];
		for (int i = 0; i < n; ++i) {
			T sum = 0;
			for (int j = 0; j < m; ++j)
				sum += ans[j] * a[i][j];
			if (abs(sum - a[i][m]) > EPS)
				return 0;
		}

		for (int i = 0; i < m; ++i)
			if (where[i] == -1)
				return INF;
		return 1;
	}


	// multiplies a matrix by a value ct
	template<typename M, typename T>
	void
		times(Dmatrix<M>& a, T ct)
	{
		for (int i = 0; i < a.rows(); i++)
		{
			for (int j = 0; j < a.cols(); j++)
			{
				a(i, j) = ct * a(i, j);
			}
		}
	}

	// converts a vector of a vector to a matrix
	Dmatrix<double>
		convertVector2Matrix(std::vector<std::vector<double>>& data)
	{

		int N = static_cast<int>(data.size());
		int M = static_cast<int>(data[0].size() - 1);

		Dmatrix<double> t(N, M);

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				t(i, j) = data[i][j];
			}
		}

		return t;
	}

	// converts a matrix to a vector of a vector
	std::vector<std::vector<double>>
		convertMatrix2vector(Dmatrix<double>& mat)
	{

		int N = mat.rows();
		int M = mat.cols();

		std::vector<std::vector<double>> out;

		for (int i = 0; i < N; i++)
		{
			std::vector<double> tmp;
			for (int j = 0; j < M; j++)
			{
				tmp.push_back(mat(i, j));
			}
			out.push_back(tmp);
		}

		return out;
	}

	// converts a vector ( typically a collumn of a csv file )
	// to a matrix
	Dmatrix<double>
		convertcol2Matrix(std::vector<double>& y)
	{

		int N = static_cast<int>(y.size());

		Dmatrix<double> t(N, 1);

		for (int i = 0; i < N; i++)
		{
			t(i, 0) = y[i];
		}
		return t;
	}


	// transpose NxN matrix and 1xN or Nx1
	template<typename T>
	Dmatrix<T>
		transpose(const Dmatrix<T>& m1)
	{
		int rows = m1.rows();
		int cols = m1.cols();

		Dmatrix<T> out(cols, rows);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				out(j, i) = m1(i, j);
			}
		}

		return out;
	}

}