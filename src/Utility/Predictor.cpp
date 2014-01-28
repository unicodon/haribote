#include "stdafx.h"
#include <math.h>
#include "Predictor.h"

#define MAX_DATA_NUM 60

Predictor::Predictor(void)
{
	x = 1;
	v = 0;
}

Predictor::~Predictor(void)
{
}


void Predictor::AddData(double value){
	data.push_back(value);

	while(data.size() > MAX_DATA_NUM){
		int size = data.size();
		for(int i = 0;i < size - 1; i++){
			data[i] = data[i+1];
		}
		data.pop_back();
	}
}

void Predictor::Clear(){
	data.clear();
}

double Predictor::MakePredictor(int n){
	if(data.size() < n )
		n = data.size();

	if(n <= 0){
		x = 0;
		v = 0;
		return -1;
	}

	if(n <= 1){
		x = data[0];
		v = 0;
		return 0;
	}

	if(n <= 2){
		x = (data[0]+data[1])/2.0;
		v = 0;
		return 0;
	}

	double x_ = 0;
	double ix_ = 0;
	int size = data.size();
	for(int i = 0;i < n;i++){
		x_ += data[size - 1 - i];
		ix_ += data[size - 1 - i]*i;
	}
	x_ /= n;
	ix_ /= n;

	double alpha = (n-1)/2.0;
	double beta = (n-1)*(2*n-1)/6.0;
	double coef = (alpha*alpha - beta);

	x = (-beta*x_ + alpha*ix_)/coef;
	v = (-alpha*x_ + ix_)/coef;

	double ret = 0;
	for(int i = 0;i < n;i++){
		ret += (data[size - 1 - i] - (x - i*v))*(data[size - 1 - i] - (x - i*v)); 
	}

	return sqrt(ret)/n;
}

double Predictor::Prediction(int t){
	double ret;
	ret = x + v*t;

	return ret;
}

