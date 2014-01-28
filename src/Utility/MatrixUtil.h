#ifndef MAT_UTIL
#define MAT_UTIL

class Vec4{
public:
	Vec4(){};
	Vec4(double,double,double,double);
	void PrintVector();
	double vec[4];
	Vec4 operator-(Vec4 &);
	Vec4 operator+(Vec4 &);
	Vec4 operator*(double a);//ÅyêÛÅz
};

/////////////////////////////////////////////////
class Mat44{
public:
	double Det();
	double Cofactor(int i,int j);
	Mat44(){}
	Mat44(double,double,double,double,
	   	  double,double,double,double,
		  double,double,double,double,
	   	  double,double,double,double);
	double matrix[4][4];
	Mat44 operator-(Mat44);
	Mat44 operator*(Mat44 &);
	Mat44 operator*(double);
	Vec4 operator*(Vec4 &);
	Mat44 Inv(void);
	void Set(double,double,double,double,
	   	     double,double,double,double,
		     double,double,double,double,
	   	     double,double,double,double);

	void PrintMatrix();
};

#endif


