#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>

class Predictor
{
public:
	Predictor(void);
	~Predictor(void);

	void AddData(double value);
	double MakePredictor(int num);

	double Prediction(int t);

	void Clear();

	double x;
	double v;
	std::vector<double> data;

private:

};


#endif
