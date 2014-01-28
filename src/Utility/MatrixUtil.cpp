#include "stdafx.h"
#include "MatrixUtil.h"

#include <iostream>
using namespace std;
#include <math.h>


Vec4::Vec4(double a,double b,double c,double d)
{
	vec[0] = a;
	vec[1] = b;
	vec[2] = c;
	vec[3] = d;
}

void Vec4::PrintVector()
{
	for(int t=0;t<4;t++)
		cout << /*setw(15)*/" " << vec[t];

	cout <<endl;
}

Vec4 Vec4::operator-(Vec4 &a)
{
	Vec4 b;
	for(int t=0;t<4;t++)
		b.vec[t] = vec[t] - a.vec[t];

	return b;
}

Vec4 Vec4::operator+(Vec4 &a)
{
	Vec4 b;
	for(int t=0;t<4;t++)
		b.vec[t] = vec[t] + a.vec[t];

	return b;
}

//yóz
Vec4 Vec4::operator *(double a)
{
	 Vec4 b;
	 for(int t=0; t<4; t++)
		 b.vec[t] = vec[t] * a;
	 return b;
}

Mat44 Mat44::operator *(Mat44& a)
{
	Mat44 b;
	for(int u=0;u<4;u++)
	{
		for(int t=0;t<4;t++)
		{
			b.matrix[t][u] = 0;

			for(int s=0;s<4;s++)
				b.matrix[t][u] += matrix[t][s]*a.matrix[s][u];
		}
	}
	return b;
}

Mat44 Mat44::operator *(double a)
{
	Mat44 b;
	for(int u=0;u<4;u++)
	{
		for(int t=0;t<4;t++)
		{
			b.matrix[t][u] = 0;

			for(int s=0;s<4;s++)
				b.matrix[t][u] += matrix[t][u]*a;
		}
	}
	return b;
}

Vec4 Mat44::operator*(Vec4 &a)
{
	Vec4 b;
	for(int t=0;t<4;t++)
	{
		b.vec[t] = 0;

		for(int s=0;s<4;s++)
			b.vec[t] += matrix[t][s]*a.vec[s];
	}
	return b;
}

Mat44::Mat44(double a00,double a01,double a02,double a03,
	   		 double a10,double a11,double a12,double a13,
			 double a20,double a21,double a22,double a23,
	   		 double a30,double a31,double a32,double a33)
{
	matrix[0][0] = a00;
	matrix[0][1] = a01;
	matrix[0][2] = a02;
	matrix[0][3] = a03;
	matrix[1][0] = a10;
	matrix[1][1] = a11;
	matrix[1][2] = a12;
	matrix[1][3] = a13;
	matrix[2][0] = a20;
	matrix[2][1] = a21;
	matrix[2][2] = a22;
	matrix[2][3] = a23;
	matrix[3][0] = a30;
	matrix[3][1] = a31;
	matrix[3][2] = a32;
	matrix[3][3] = a33;
}

void Mat44::Set(double a00,double a01,double a02,double a03,
	   		    double a10,double a11,double a12,double a13,
			    double a20,double a21,double a22,double a23,
	   		    double a30,double a31,double a32,double a33)
{
	matrix[0][0] = a00;
	matrix[0][1] = a01;
	matrix[0][2] = a02;
	matrix[0][3] = a03;
	matrix[1][0] = a10;
	matrix[1][1] = a11;
	matrix[1][2] = a12;
	matrix[1][3] = a13;
	matrix[2][0] = a20;
	matrix[2][1] = a21;
	matrix[2][2] = a22;
	matrix[2][3] = a23;
	matrix[3][0] = a30;
	matrix[3][1] = a31;
	matrix[3][2] = a32;
	matrix[3][3] = a33;
}

void Mat44::PrintMatrix()
{
//	cout << setprecision(4);
	for(int t=0;t<4;t++)
	{
		for(int s=0;s<4;s++)
			cout << /*setw(15)*/" " <<matrix[t][s];

		cout << endl;
	}
	cout <<endl;
}

Mat44 Mat44::operator -(Mat44 a)
{
	Mat44 b;
	for(int t=0;t<4;t++)
		for(int s=0;s<4;s++)
			b.matrix[s][t] = matrix[s][t] - a.matrix[s][t];
	return b;
}

Mat44 Mat44::Inv(void)
{
	Mat44 a;
	double det = Det();
	for(int s=0;s<4;s++)
		for(int t=0;t<4;t++)
			a.matrix[t][s] = Cofactor(s,t)/det;

	return a;
}


double Mat44::Cofactor(int i, int j)
{
	double copy[3][3];
	int a = -1,b = -1;

	for(int s=0;s<4;s++)
	{
		if(s != i)
		{
			a++;
			for(int t=0;t<4;t++)
			{
				if(t != j)
				{
					b++;

					copy[a][b] = matrix[s][t];
				}
			}
			b = -1;
		}
	}

	if((i+j)%2 == 0)
		return (copy[0][0]*copy[1][1]*copy[2][2] +
				copy[0][1]*copy[1][2]*copy[2][0] +
				copy[0][2]*copy[1][0]*copy[2][1] -
				copy[0][0]*copy[1][2]*copy[2][1] -
				copy[0][1]*copy[1][0]*copy[2][2] - 
				copy[0][2]*copy[1][1]*copy[2][0]);
	else
		return -(copy[0][0]*copy[1][1]*copy[2][2] +
				copy[0][1]*copy[1][2]*copy[2][0] +
				copy[0][2]*copy[1][0]*copy[2][1] -
				copy[0][0]*copy[1][2]*copy[2][1] -
				copy[0][1]*copy[1][0]*copy[2][2] - 
				copy[0][2]*copy[1][1]*copy[2][0]);
}

double Mat44::Det()
{
	return (matrix[0][0]*Cofactor(0,0) +
			matrix[0][1]*Cofactor(0,1) +
			matrix[0][2]*Cofactor(0,2) +
			matrix[0][3]*Cofactor(0,3));
}
