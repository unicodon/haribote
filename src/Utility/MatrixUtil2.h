//MatrixUtil2.h//////////////////////////////
//2001/3/1//////////////////////////////////
//作：上田 隆一/////////////////////////////

#ifndef MATRIX2
#define MATRIX2

#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

template <class T,int row,int col>
class Mat{
public:	
	T m_mat[row][col];
	double m_dCoef;
	Mat(){m_dCoef = 1.0;}

	Mat(const T *data)
	{
		m_dCoef = 1.0;

		for(int r=0;r<row;r++)
			for(int c=0;c<col;c++)
				m_mat[r][c] = data[r*col+c];
	}

	inline T get(int r,int c){return m_dCoef * m_mat[r][c];}

	void put_safety(int r,int c,T value)
	{
		if(m_dCoef == 1.0)
			m_mat[r][c] = value;
		else
		{
			insert_coef();
			m_mat[r][c] = value;
		}
	}

	inline void put(int r,int c,T value){m_mat[r][c] = value/m_dCoef;}

	void random(void)
	{
		srand(rand());
		for(int r=0;r<row;r++)
			for(int c=0;c<col;c++)
				m_mat[r][c] = (double)rand()/(double)(RAND_MAX+1);

		m_dCoef = 1.0;
	}

	void size(void){cout << "Size = " << row << "行" << col << "列" << endl;}

	void print(void)
	{
		cout << m_dCoef << " * " << endl;

		for(int r=0;r<row;r++)
		{
			for(int c=0;c<col;c++)
				cout << m_mat[r][c] << "\t";
			
			cout << endl;
		}
	}

	void insert_coef(void)
	{
		for(int r=0;r<row;r++)
			for(int c=0;c<col;c++)
				m_mat[r][c] *= m_dCoef;

		m_dCoef = 1.0;
	}

	Mat<T,row-1,col-1> minor(int r,int c)
	{
		if(row == 1 || col == 1)
			return NULL;
		else if(row <= r || col <= c)
			return NULL;

		Mat<T,row-1,col-1> ans;
		ans.m_dCoef = m_dCoef;

		int rr[row-1];
		int cc[col-1];

		for(int a=0;a<row-1;a++)
		{
			if(a < r)
				rr[a] = a;
			else
				rr[a] = a+1;
		}
		for(int a=0;a<col-1;a++)
		{
			if(a < c)
				cc[a] = a;
			else
				cc[a] = a+1;
		}


		for(int s=0;s<row-1;s++)
			for(int t=0;t<col-1;t++)
				ans.m_mat[s][t] = m_mat[rr[s]][cc[t]];

		return ans;
	}

	Mat<T,col,row> transpose(void)
	{
		Mat<T,col,row> ans;

		for(int r=0;r<col;r++)
			for(int c=0;c<row;c++)
				ans.m_mat[r][c] = m_mat[c][r];

		return ans;
	}

	//オペレーション
	void operator *= (const Mat<T,col,col>& A)
	{
		Mat<T,row,col> ans;
		int r,c;

		m_dCoef *= A.m_dCoef;
		for(r=0;r<row;r++)
		{
			for(c=0;c<col;c++)
			{
				ans.m_mat[r][c] = m_mat[r][0]*A.m_mat[0][c];
				for(int cc=1;cc<col;cc++)
					ans.m_mat[r][c] += m_mat[r][cc]*A.m_mat[cc][c];
			}
		}

		*this = ans;
	}

	void operator *= (const double& d){m_dCoef *= d;}
	void operator += (const Mat<T,row,col>& A)
	{
		int r,c;

		if(m_dCoef == A.m_dCoef)
		{
			for(r=0;r<row;r++)
				for(c=0;c<col;c++)
					m_mat[r][c] = m_mat[r][c] + A.m_mat[r][c];

			return;
		}

		for(r=0;r<row;r++)
			for(c=0;c<col;c++)
				m_mat[r][c] = m_mat[r][c]*m_dCoef + A.m_mat[r][c]*A.m_dCoef;

			m_dCoef = 1.0;
	}
	void operator -= (const Mat<T,row,col>& A)
	{
		int r,c;

		if(m_dCoef == A.m_dCoef)
		{
			for(r=0;r<row;r++)
				for(c=0;c<col;c++)
					m_mat[r][c] = m_mat[r][c] - A.m_mat[r][c];

			return;
		}

		for(r=0;r<row;r++)
			for(c=0;c<col;c++)
				m_mat[r][c] = m_mat[r][c]*m_dCoef - A.m_mat[r][c]*A.m_dCoef;

		m_dCoef = 1.0;
	}

	void operator /= (const double& d){m_dCoef /= d;}

	void operator = (const Mat<T,row,col>& A)
	{
		int r,c;

		m_dCoef = A.m_dCoef;

		for(r=0;r<row;r++)
			for(c=0;c<col;c++)
				m_mat[r][c] =  A.m_mat[r][c];
	}

	Mat<T,row,col> operator *(const double d)
	{
		Mat<T,row,col> ans = *this;
		ans.m_dCoef *= d;

		return ans;
	}

	Mat<T,row,col> operator *(const int n)
	{
		Mat<T,row,col> ans = *this;
		ans.m_dCoef *= (double)n;

		return ans;
	}

	Mat<T,row,col> operator +(const Mat<T,row,col>& A)
	{
		Mat<T,row,col> ans;
		int r,c;

		if(m_dCoef == A.m_dCoef)
		{
			ans.m_dCoef = m_dCoef;
			for(r=0;r<row;r++)
				for(c=0;c<col;c++)
					ans.m_mat[r][c] = m_mat[r][c] + A.m_mat[r][c];

			return ans;
		}

		for(r=0;r<row;r++)
			for(c=0;c<col;c++)
				ans.m_mat[r][c] = A.m_mat[r][c]*A.m_dCoef + m_mat[r][c]*m_dCoef;

		m_dCoef = 1.0;
		return ans;
	}

	Mat<T,row,col> operator -(const Mat<T,row,col>& A)
	{
		int c,r;
		Mat<T,row,col> ans;

		if(m_dCoef == A.m_dCoef)
		{
			ans.m_dCoef = m_dCoef;
			for(r=0;r<row;r++)
				for(c=0;c<col;c++)
					ans.m_mat[r][c] = m_mat[r][c] - A.m_mat[r][c];

			return ans;
		}

		for(r=0;r<row;r++)
			for(c=0;c<col;c++)
				ans.m_mat[r][c] = m_mat[r][c]*m_dCoef - A.m_mat[r][c]*A.m_dCoef;

		return ans;
	}
	Mat<T,row,col> operator -(void) const
	{
		Mat<T,row,col> ans = *this;
		ans.m_dCoef *= -1.0;
		return ans;
	}

	Mat<T,row,col> operator /(const double d)
	{
		Mat<T,row,col> ans = *this;

		ans.m_dCoef /= d;

		return ans;
	}

	Mat<T,row,col> operator /(const int n)
	{
		Mat<T,row,col> ans = *this;
		ans.m_dCoef /= (double)n;
		return ans;
	}

	bool operator ==(const Mat<T,row,col>& A)
	{
		for(int r=0;r<row;r++)
			for(int c=0;c<col;c++)
				if(m_mat[r][c]*m_dCoef != A.m_mat[r][c]*A.m_dCoef)
					return false;

		return true;
	}

	bool operator !=(const Mat<T,row,col>& A)
	{
		for(int r=0;r<row;r++)
			for(int c=0;c<col;c++)
				if(m_mat[r][c]*m_dCoef != A.m_mat[r][c]*A.m_dCoef)
					return true;

		return false;
	}

	friend Mat<T,row,col> operator *(const Mat<T,row,1>& A, const Mat<T,1,col>& B)
	{
		Mat<T,row,col> ans;

		ans.m_dCoef = A.m_dCoef*B.m_dCoef;
		for(int r=0;r<row;r++)
			for(int c=0;c<col;c++)
				ans.m_mat[r][c] = A.m_mat[r][0]*B.m_mat[0][c];

		return ans;
	}

	friend Mat<T,row,col> operator *(const Mat<T,row,2>& A, const Mat<T,2,col>& B)
	{
		Mat<T,row,col> ans;

		ans.m_dCoef = A.m_dCoef*B.m_dCoef;
		for(int r=0;r<row;r++)
		{
			for(int c=0;c<col;c++)
			{
				ans.m_mat[r][c] = A.m_mat[r][0]*B.m_mat[0][c];
				for(int cc=1;cc<2;cc++)
					ans.m_mat[r][c] += A.m_mat[r][cc]*B.m_mat[cc][c];
			}
		}

		return ans;
	}
	friend Mat<T,row,col> operator *(const Mat<T,row,3>& A, const Mat<T,3,col>& B)
	{
		Mat<T,row,col> ans;

		ans.m_dCoef = A.m_dCoef*B.m_dCoef;
		for(int r=0;r<row;r++)
		{
			for(int c=0;c<col;c++)
			{
				ans.m_mat[r][c] = A.m_mat[r][0]*B.m_mat[0][c];
				for(int cc=1;cc<3;cc++)
					ans.m_mat[r][c] += A.m_mat[r][cc]*B.m_mat[cc][c];
			}
		}

		return ans;
	}

	friend Mat<T,row,col> operator *(const Mat<T,row,4>& A, const Mat<T,4,col>& B)
	{
		Mat<T,row,col> ans;

		ans.m_dCoef = A.m_dCoef*B.m_dCoef;
		for(int r=0;r<row;r++)
		{
			for(int c=0;c<col;c++)
			{
				ans.m_mat[r][c] = A.m_mat[r][0]*B.m_mat[0][c];
				for(int cc=1;cc<4;cc++)
					ans.m_mat[r][c] += A.m_mat[r][cc]*B.m_mat[cc][c];
			}
		}

		return ans;
	}

	friend Mat<T,row,col> operator *(const double d, const Mat<T,row,col>& A)
	{
		Mat<T,row,col> ans = A;
		ans.m_dCoef *= d;

		return ans;
	}

	friend Mat<T,row,col> operator *(const int n, const Mat<T,row,col>& A)
	{
		Mat<T,row,col> ans = A;
		ans.m_dCoef *= n;

		return ans;
	}
};

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template <class T,int n>
class MatNN : public Mat<T,n,n>
{
public:
	MatNN() : Mat<T,n,n>(){}
	MatNN(const T *data) : Mat<T,n,n>(data){}

	MatNN<T,n> operator *(const MatNN<T,n>& A)
	{
		MatNN<T,n> ans;

		ans.m_dCoef = m_dCoef*A.m_dCoef;

		for(int r=0;r<n;r++)
		{
			for(int c=0;c<n;c++)
			{
				ans.m_mat[r][c] = m_mat[r][0]*A.m_mat[0][c];
				for(int cc=1;cc<n;cc++)
					ans.m_mat[r][c] += m_mat[r][cc]*A.m_mat[cc][c];
			}
		}

		return ans;	
	}

	void operator =(Mat<T,n,n> A)
	{
		m_dCoef = A.m_dCoef;

		for(int r=0;r<n;r++)
			for(int c=0;c<n;c++)
					m_mat[r][c] = A.m_mat[r][c];

	}

	double cofactor(int r,int c)
	{
		MatNN<T,n-1> m = minor(r,c);
		int a;
		if((r+c)%2 == 0)
			a = 1;
		else
			a = -1;
		return m.det()*a*m_dCoef;
	}

	double det_none_coef(void)
	{
		double ans;
		if(n == 4)
			ans =  m_mat[0][0] *((m_mat[1][1] * m_mat[2][2] * m_mat[3][3]) +
				     			 (m_mat[1][2] * m_mat[2][3] * m_mat[3][1]) +
				 				 (m_mat[1][3] * m_mat[2][1] * m_mat[3][2]) -
								 (m_mat[1][3] * m_mat[2][2] * m_mat[3][1]) -
								 (m_mat[1][2] * m_mat[2][1] * m_mat[3][3]) -
								 (m_mat[1][1] * m_mat[2][3] * m_mat[3][2]))
				  -m_mat[0][1] *((m_mat[1][0] * m_mat[2][2] * m_mat[3][3]) +
				     			 (m_mat[1][2] * m_mat[2][3] * m_mat[3][0]) +
				 				 (m_mat[1][3] * m_mat[2][0] * m_mat[3][2]) -
								 (m_mat[1][3] * m_mat[2][2] * m_mat[3][0]) -
								 (m_mat[1][2] * m_mat[2][0] * m_mat[3][3]) -
								 (m_mat[1][0] * m_mat[2][3] * m_mat[3][2]))
				  +m_mat[0][2] *((m_mat[1][0] * m_mat[2][1] * m_mat[3][3]) +
				     			 (m_mat[1][1] * m_mat[2][3] * m_mat[3][0]) +
				 				 (m_mat[1][3] * m_mat[2][0] * m_mat[3][1]) -
								 (m_mat[1][3] * m_mat[2][1] * m_mat[3][0]) -
								 (m_mat[1][1] * m_mat[2][0] * m_mat[3][3]) -
								 (m_mat[1][0] * m_mat[2][3] * m_mat[3][1]))
				  -m_mat[0][3] *((m_mat[1][0] * m_mat[2][1] * m_mat[3][2]) +
								 (m_mat[1][1] * m_mat[2][2] * m_mat[3][0]) +
								 (m_mat[1][2] * m_mat[2][0] * m_mat[3][1]) -
								 (m_mat[1][2] * m_mat[2][1] * m_mat[3][0]) -
								 (m_mat[1][1] * m_mat[2][0] * m_mat[3][2]) -
								 (m_mat[1][0] * m_mat[2][2] * m_mat[3][1]));
		if(n == 3)
			ans = (m_mat[0][0] * m_mat[1][1] * m_mat[2][2]) +
				  (m_mat[0][1] * m_mat[1][2] * m_mat[2][0]) +
				  (m_mat[0][2] * m_mat[1][0] * m_mat[2][1]) -
				  (m_mat[0][2] * m_mat[1][1] * m_mat[2][0]) -
				  (m_mat[0][1] * m_mat[1][0] * m_mat[2][2]) -
				  (m_mat[0][0] * m_mat[1][2] * m_mat[2][1]);
		else if(n == 2)
			ans = m_mat[0][0]*m_mat[1][1] - m_mat[0][1]*m_mat[1][0];
		else if(n == 1)
			ans = m_mat[0][0];
		else
		{
			ans = 0;
			for(int t=0;t<n;t++)
			{
				T v[n*n];
				minor(0,t,v);

				MatNN<T,(n < 4 ? 4 : n-1)> tmp(v);

				if(t%2 == 0)
					ans += m_mat[0][t]*tmp.det_none_coef();
				else
					ans -= m_mat[0][t]*tmp.det_none_coef();
			}
		}
		return ans;
	}

	double det(void){return det_none_coef()*m_dCoef;}

	void equal(Mat<T,n,n>& A)
	{
		m_dCoef = A.m_dCoef;
		for(int r=0;r<n;r++)
			for(int c=0;c<n;c++)
				m_mat[r][c] = A.m_mat[r][c];
	}

	void I(void)
	{
		m_dCoef = 1.0;
		for(int r=0;r<n;r++)
			for(int c=0;c<n;c++)
				m_mat[r][c] = r==c ? 1 : 0;
	}

/*	void inv(void)
	{
		T co_fact[n][n];
		T mat[n];
		int s;
		double x = 0.0;

		for(s=0;s<n;s++)
			mat[s] = m_mat[0][s];

		for(s=0;s<n;s++)
			for(int t=0;t<n;t++)
				co_fact[s][t] = cofactor_none_coef(s,t);

		for(s=0;s<n;s++)
			for(int t=0;t<n;t++)
				m_mat[s][t] = co_fact[t][s];

		for(s=0;s<n;s++)
			x += m_mat[s][0] * mat[s];

		m_dCoef = 1.0/(x*m_dCoef);
	}*/

	MatNN<T,n-1> minor(int r,int c)
	{
		if(n == 1)
			return NULL;
		else if(n <= r || n <= c)
			return NULL;

		MatNN<T,n-1> ans;
		ans.m_dCoef = m_dCoef;
		
		int a;
		int rr[n-1];
		int cc[n-1];

		for(a=0;a<n-1;a++)
		{
			if(a < r)
				rr[a] = a;
			else
				rr[a] = a+1;
		}
		for(a=0;a<n-1;a++)
		{
			if(a < c)
				cc[a] = a;
			else
				cc[a] = a+1;
		}


		for(int s=0;s<n-1;s++)
			for(int t=0;t<n-1;t++)
				ans.m_mat[s][t] = m_mat[rr[s]][cc[t]];

		return ans;
	}

	double trace(void)
	{
		double ans = 0;
		for(int t=0;t<n;t++)
			ans += m_mat[t][t];

		return (ans*m_dCoef);
	}

	void transpose(void)
	{
		MatNN<T,n> ans;

		for(int r=0;r<n;r++)
			for(int c=0;c<n;c++)
				ans.m_mat[r][c] = m_mat[c][r];

		*this = ans;
	}

protected:
	double cofactor_none_coef(int r,int c)
	{
		MatNN<T,n-1> m = minor(r,c);
		int a;
		if((r+c)%2 == 0)
			a = 1;
		else
			a = -1;
		return m.det()*a;
	}

	void minor(int r,int c,T *v)
	{
		int i=0;

		for(int s=0;s<n;s++)
			for(int t=0;t<n;t++)
				if(s != r && t != c)
				{
					v[i] = m_mat[s][t];
					i++;
				}
	}
};
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template <class T,int n>
class ColVec : public Mat<T,n,1>
{
public:
	ColVec() : Mat<T,n,1>(){}
	ColVec(const T *data) : Mat<T,n,1>(data){}

	void operator =(const Mat<T,n,1>& A)
	{
		m_dCoef = A.m_dCoef;

		for(int r=0;r<n;r++)
			m_mat[r][0] = A.m_mat[r][0];
	}

	double abs(void)
	{
		double ans = 0;

		for(int t=0;t<n;t++)
			ans += m_mat[t][0]*m_mat[t][0];

		return sqrt(ans);
	}

	T inner_product(const ColVec<T,n>& A)
	{
		T ans = 0;

		for(int t=0;t<n;t++)
			ans += m_mat[t][0]*A.m_mat[t][0];

		return ans*m_dCoef*A.m_dCoef;
	}
};
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template <class T,int n>
class RowVec : public Mat<T,1,n>
{
public:
	RowVec() : Mat<T,1,n>(){}
	RowVec(const T *data) : Mat<T,1,n>(data){}

	double average(void)
	{
		double a = 0;

		for(int t=0;t<n;t++)
			a += m_mat[0][t];

		return (a*m_dCoef/n);
	}

	double abs(void)
	{
		double ans = 0;

		for(int t=0;t<n;t++)
			ans += m_mat[0][t]*m_mat[0][t];

		return sqrt(ans)*m_dCoef;
	}

	T operator*(const ColVec<T,n>& A)
	{
		T ans = 0;
		
		for(int t=0;t<n;t++)
			ans += m_mat[0][t] * A.m_mat[t][0];

		return ans*m_dCoef*A.m_dCoef;
	}

	void operator=(const Mat<T,1,n>& A)
	{
		m_dCoef = A.m_dCoef;
		
		for(int t=0;t<n;t++)
			m_mat[0][t] = A.m_mat[0][t];
	}

	T inner_product(const RowVec<T,n>& A)
	{
		T ans = 0.0;

		for(int t=0;t<n;t++)
			ans += m_mat[0][t]*A.m_mat[0][t];

		return ans*m_dCoef*A.m_dCoef;
	}

	RowVec<double,n> plus_same_num(const T& a)
	{
		RowVec<double,n> ans;

		for(int t=0;t<n;t++)
			ans.m_mat[0][t] = m_dCoef*m_mat[0][t] + a;

		return ans;
	}

	void pp_same_num(const T& a)
	{
		for(int t=0;t<n;t++)
			m_mat[0][t] = m_dCoef*m_mat[0][t] + a;

		m_dCoef = 1.0;
	}

	RowVec<double,n> minus_same_num(const T& a)
	{
		RowVec<double,n> ans;

		for(int t=0;t<n;t++)
			ans.m_mat[0][t] = m_dCoef*m_mat[0][t] - a;

		ans.m_dCoef = 1.0;
		return ans;
	}

	void mm_same_num(const T& a)
	{
		for(int t=0;t<n;t++)
			m_mat[0][t] = m_dCoef*m_mat[0][t] - a;

		m_dCoef = 1.0;
	}

	void mm_same_num(const RowVec<double,n> &A,const double &a)
	{
		m_dCoef = 1.0;

		for(int t=0;t<n;t++)
			m_mat[0][t] = A.m_dCoef*A.m_mat[0][t] - a;
	}

	void pow2_each_element(void)
	{
		m_dCoef *= m_dCoef;

		for(int t=0;t<n;t++)
			m_mat[0][t] *= m_mat[0][t];
	}

	void fill_same_num(const T& a)
	{
		m_dCoef = 1.0;
		for(int t=0;t<n;t++)
			m_mat[0][t] = a;
	}

	double get_max(void)
	{
		double max = m_mat[0][0];
		for(int t=1;t<n;t++)
			max = ((max < m_mat[0][t]) ? m_mat[0][t] : max);

		return max*m_dCoef;
	}

	double get_min(void)
	{
		double min = m_mat[0][0];
		for(int t=1;t<n;t++)
			min = ((min > m_mat[0][t]) ? m_mat[0][t] : min);

		return min*m_dCoef;
	}

	double sum(void)
	{
		T a = m_mat[0][0];
		for(int t=1;t<n;t++)
			a += m_mat[0][t];

		return m_dCoef*(double)a;
	}
};
#endif
