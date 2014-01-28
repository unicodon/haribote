#include "MatrixUtil2.h"

#ifndef TMAT_H
#define TMAT_H

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class Vec3D : public ColVec<double,3>
{
public:
	Vec3D() : ColVec<double,3>(){}
	Vec3D(double x,double y,double z) : ColVec<double,3>(){m_mat[0][0] = x;m_mat[1][0] = y;m_mat[2][0] = z;}
	Vec3D(const double *data) : ColVec<double,3>(data){}


	Vec3D outer_product(const Vec3D& A)
	{
		Vec3D ans;

		ans.m_dCoef = m_dCoef*A.m_dCoef;
		ans.m_mat[0][0] = m_mat[1][0]*A.m_mat[2][0] - m_mat[2][0]*A.m_mat[1][0];
		ans.m_mat[1][0] = m_mat[2][0]*A.m_mat[0][0] - m_mat[0][0]*A.m_mat[2][0];
		ans.m_mat[2][0] = m_mat[0][0]*A.m_mat[1][0] - m_mat[1][0]*A.m_mat[0][0];

		return ans;	
	}

	void operator = (const Mat<double,3,1>& A)
	{
		m_dCoef = A.m_dCoef;
		m_mat[0][0] = A.m_mat[0][0];
		m_mat[1][0] = A.m_mat[1][0];
		m_mat[2][0] = A.m_mat[2][0];
	}
	void set(double x,double y,double z)
	{
		m_dCoef = 1.0;
		m_mat[0][0] = x;
		m_mat[1][0] = y;
		m_mat[2][0] = z;
	}
};


///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class Point3D : public Vec3D
{
public:
	Point3D() : Vec3D(){}
	Point3D(double x,double y,double z) : Vec3D(x,y,z){}
	Point3D(const double *data) : Vec3D(data){}

	void operator = (const Mat<double,3,1>& A)
	{
		m_dCoef = A.m_dCoef;
		m_mat[0][0] = A.m_mat[0][0];
		m_mat[1][0] = A.m_mat[1][0];
		m_mat[2][0] = A.m_mat[2][0];
	}
};
/*
void Point3D::operator = (const Mat<double,3,1>& A)
{
	m_dCoef = A.m_dCoef;
	m_mat[0][0] = A.m_mat[0][0];
	m_mat[1][0] = A.m_mat[1][0];
	m_mat[2][0] = A.m_mat[2][0];
}*/
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class TMat{
public:
	TMat(){}
	TMat(double t00,double t01,double t02,double t03,
		 double t10,double t11,double t12,double t13,
		 double t20,double t21,double t22,double t23)
	{
		m_R.m_dCoef = 1.0;
		m_p.m_dCoef = 1.0;

		m_R.m_mat[0][0] = t00;
		m_R.m_mat[0][1] = t01;
		m_R.m_mat[0][2] = t02;
		m_R.m_mat[1][0] = t10;
		m_R.m_mat[1][1] = t11;
		m_R.m_mat[1][2] = t12;
		m_R.m_mat[2][0] = t20;
		m_R.m_mat[2][1] = t21;
		m_R.m_mat[2][2] = t22;
		m_p.m_mat[0][0] = t03;
		m_p.m_mat[1][0] = t13;
		m_p.m_mat[2][0] = t23;
	}

	MatNN<double,3> m_R;
	ColVec<double,3> m_p;

/*	void inv(void)
	{
		m_R.inv();
		m_p = -(m_R*m_p);
	}*/
	void print(void)
	{
		for(int r=0;r<3;r++)
		{
			for(int c=0;c<3;c++)
			{
				cout << m_R.m_dCoef*m_R.m_mat[r][c] << " ";
			}
			cout << " " << m_p.m_dCoef*m_p.m_mat[r][0] <<endl;
		}
		cout << 0 << " " << 0 << " " << 0 << " " << 1 <<endl;
	}
	void random(void)
	{
		m_R.random();
		m_p.random();
	}
	void set(double t00,double t01,double t02,double t03,
			 double t10,double t11,double t12,double t13,
			 double t20,double t21,double t22,double t23)
	{
		m_R.m_dCoef = 1.0;
		m_p.m_dCoef = 1.0;

		m_R.m_mat[0][0] = t00;
		m_R.m_mat[0][1] = t01;
		m_R.m_mat[0][2] = t02;
		m_R.m_mat[1][0] = t10;
		m_R.m_mat[1][1] = t11;
		m_R.m_mat[1][2] = t12;
		m_R.m_mat[2][0] = t20;
		m_R.m_mat[2][1] = t21;
		m_R.m_mat[2][2] = t22;
		m_p.m_mat[0][0] = t03;
		m_p.m_mat[1][0] = t13;
		m_p.m_mat[2][0] = t23;
	}
	TMat operator*(const TMat& A)
	{
		TMat ans;
		
		ans.m_R = m_R*A.m_R;
		ans.m_p = (m_R*A.m_p) + m_p;

		return ans;
	}
	void operator=(const TMat& A)
	{
		m_R = A.m_R;
		m_p = A.m_p;
	}

	Vec3D operator*(const Vec3D& A)
	{
		Vec3D ans;
		
		ans = m_R*A;

		return ans;
	}
	Point3D operator*(const Point3D& A)
	{
		Point3D ans;
		
		ans = m_R*A + m_p;

		return ans;
	}
};
/*
TMat::TMat(double t00,double t01,double t02,double t03,
		   double t10,double t11,double t12,double t13,
		   double t20,double t21,double t22,double t23)
{
	m_R.m_dCoef = 1.0;
	m_p.m_dCoef = 1.0;

	m_R.m_mat[0][0] = t00;
	m_R.m_mat[0][1] = t01;
	m_R.m_mat[0][2] = t02;
	m_R.m_mat[1][0] = t10;
	m_R.m_mat[1][1] = t11;
	m_R.m_mat[1][2] = t12;
	m_R.m_mat[2][0] = t20;
	m_R.m_mat[2][1] = t21;
	m_R.m_mat[2][2] = t22;
	m_p.m_mat[0][0] = t03;
	m_p.m_mat[1][0] = t13;
	m_p.m_mat[2][0] = t23;
}


void TMat::inv(void)
{
	m_R.inv();
	m_p = -(m_R*m_p);
}


void TMat::random(void)
{
	m_R.random();
	m_p.random();
}

void TMat::set(double t00,double t01,double t02,double t03,
			  double t10,double t11,double t12,double t13,
			  double t20,double t21,double t22,double t23)
{
	m_R.m_dCoef = 1.0;
	m_p.m_dCoef = 1.0;

	m_R.m_mat[0][0] = t00;
	m_R.m_mat[0][1] = t01;
	m_R.m_mat[0][2] = t02;
	m_R.m_mat[1][0] = t10;
	m_R.m_mat[1][1] = t11;
	m_R.m_mat[1][2] = t12;
	m_R.m_mat[2][0] = t20;
	m_R.m_mat[2][1] = t21;
	m_R.m_mat[2][2] = t22;
	m_p.m_mat[0][0] = t03;
	m_p.m_mat[1][0] = t13;
	m_p.m_mat[2][0] = t23;
}


void TMat::print(void)
{
#ifndef UE_AIBO
	cout << setiosflags(ios::fixed);
	cout << setprecision(3);
#endif
	for(int r=0;r<3;r++)
	{
		for(int c=0;c<3;c++)
		{
#ifndef UE_AIBO
			cout << setw(10) << m_R.m_dCoef*m_R.m_mat[r][c];
#else
			cout << m_R.m_dCoef*m_R.m_mat[r][c] << " ";
#endif
		}
#ifndef UE_AIBO
		cout << setw(10) << m_p.m_dCoef*m_p.m_mat[r][0] <<endl;
	}
#else
		cout << " " << m_p.m_dCoef*m_p.m_mat[r][0] <<endl;
	}
#endif

#ifndef UE_AIBO
			cout << setw(10) << 0 << setw(10) << 0 << setw(10) << 0 << setw(10) << 1 <<endl;
#else
			cout << 0 << " " << 0 << " " << 0 << " " << 1 <<endl;
#endif
}

TMat TMat::operator *(const TMat& A)
{
	TMat ans;
	
	ans.m_R = m_R*A.m_R;
	ans.m_p = (m_R*A.m_p) + m_p;

	return ans;
}

Vec3D TMat::operator *(const Vec3D& A)
{
	Vec3D ans;
	
	ans = m_R*A;

	return ans;
}

Point3D TMat::operator *(const Point3D& A)
{
	Point3D ans;
	
	ans = m_R*A + m_p;

	return ans;
}

void TMat::operator =(const TMat& A)
{
	m_R = A.m_R;
	m_p = A.m_p;
}*/

#endif

