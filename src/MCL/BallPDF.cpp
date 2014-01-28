#include "StdAfx.h"
#include "ballpdf.h"
#include <math.h>
#include <string.h>
#include "../Utility/Random.h"

inline int search_pos(int pos_min, int pos_max, const double value, const double *prob_distribution)
{
	int center;
	while(pos_max > pos_min+1){
		center = (pos_min+pos_max) / 2;
		if(prob_distribution[center] > value)
			pos_max = center;
		else
			pos_min = center;
	}
	return pos_min;
}



BallPDF::BallPDF(void)
{
	sample = new BallParticle[NUM_SAMPLE];
	BallInfo bi;
	bi.direction = 0;
	bi.distance = 0;
	Reset(bi);
}

BallPDF::~BallPDF(void)
{
	delete[] sample;
}

void BallPDF::Reset(BallObserveInfo info){
/*
	double px = info.distance * cos(info.direction * 3.1416 / 180);
	double py = info.distance * sin(info.direction * 3.1416 / 180);


	double er = 0.1*2;
	double ephi = 5*3.1416/180*2;

	double exx =  info.distance * er   * cos(info.direction * 3.1416 / 180);
	double exy = -info.distance * ephi * sin(info.direction * 3.1416 / 180);
	double eyx =  info.distance * er   * sin(info.direction * 3.1416 / 180);
	double eyy =  info.distance * ephi * cos(info.direction * 3.1416 / 180);
*/



	double phi = info.direction * 3.1416 / 180;
	double cosphi = cos(phi);
	double sinphi = sin(phi);


	double px = info.distance * cosphi;
	double py = info.distance * sinphi;

	double er = 0.1*1;
	double ephi = 0.2*3.1416/180*5;

	double d = info.distance + 100;
	d = d*d;
	d *= 2/100000.0;
	d *= 5;

//	double exx =  info.distance * er   * cosphi;
	double exx =  d   * cosphi;
	double exy =  -ephi * sinphi * info.distance;
//	double exy = -info.distance * ephi * sinphi;
//	double eyx =  info.distance * er   * sinphi;
	double eyx =  d * sinphi;
	double eyy =  ephi * cosphi * info.distance;
//	double eyy =  info.distance * ephi * cosphi;


	the_ball.bx = px;
	the_ball.by = py;
	the_ball.vx = 0;
	the_ball.vy = 0;
	the_ball.weight = 1;


	for(int i = 0;i < NUM_SAMPLE;i++){
		double a = GAUSSIAN_RAND;
		double b = GAUSSIAN_RAND;

		double x = a*exx + b*exy + px;
		double y = a*eyx + b*eyy + py;

		sample[i].bx = x;
		sample[i].by = y;

		sample[i].vx = 500*GAUSSIAN_RAND;
		sample[i].vy = 500*GAUSSIAN_RAND;

		sample[i].weight = 1.0 / NUM_SAMPLE;
	}
}

void BallPDF::Update(BallObserveInfo info){
	if(info.distance == 0)return;

	static BallParticle last = {0,0,0,0,1};

	double phi = info.direction * 3.1416 / 180;
	double cosphi = cos(phi);
	double sinphi = sin(phi);


	double px = info.distance * cosphi;
	double py = info.distance * sinphi;

	double er = 0.1*1;
	double ephi = 0.2*3.1416/180*5;

	double d = info.distance + 100;
	d = d*d;
	d *= 2/100000.0;
	d *= 5;

//	double exx =  info.distance * er   * cosphi;
	double exx =  d   * cosphi;
	double exy =  -ephi * sinphi * info.distance;
//	double exy = -info.distance * ephi * sinphi;
//	double eyx =  info.distance * er   * sinphi;
	double eyx =  d * sinphi;
	double eyy =  ephi * cosphi * info.distance;
//	double eyy =  info.distance * ephi * cosphi;

	static double probhist[NUM_SAMPLE+1];

	static BallParticle tmp_sample[NUM_SAMPLE];

	memcpy(tmp_sample,sample,sizeof(BallParticle)*NUM_SAMPLE);

	double sum = 0;
	for(int i = 0;i < NUM_SAMPLE;i++){
		
		double a = sample[i].bx - px;
		double b = sample[i].by - py;

		double x = a * cosphi  + b * sinphi;
		double y = -a * sinphi + b * cosphi;

		x /= info.distance * er;
		y /= info.distance * ephi;

		double r = x * x + y * y;
		r *= -0.5;

		sample[i].weight *= exp(r) / (2*3.1416);
		sum += sample[i].weight;
	}

	psum = sum;

	static int mujun_cnt = 0;
	
	if(sum < 0.001){
		mujun_cnt++;

		if(mujun_cnt > 2){
			Reset(info);
			mujun_cnt = 0;
		}else{
			memcpy(sample,tmp_sample,sizeof(BallParticle)*NUM_SAMPLE);
		}
		return;
	}
	
	for(int i = 0;i < NUM_SAMPLE;i++){
		sample[i].weight /= sum;
	}
	
	sum = 0;
	probhist[0] = sum; 
	static int dest[NUM_SAMPLE];
	static int num_div[NUM_SAMPLE];
	for(int q = 0;q < NUM_SAMPLE;q++){
		sum += sample[q].weight;
		probhist[q+1] = sum; 
		num_div[q] = 1;
	}


	//•ªŠ„
	for(int i = 0;i < NUM_SAMPLE;i++){
		if(sample[i].weight <= 0.01/255.0/NUM_SAMPLE){
			int j = search_pos(0,NUM_SAMPLE,DOUBLE_RAND,probhist);

			num_div[j]++;
			num_div[i]--;

			dest[i] = j;
			
/*
			sample[i].bx = sample[j].bx + GAUSSIAN_RAND * 50;
			sample[i].by = sample[j].by + GAUSSIAN_RAND * 50;
//			sample[i].bx = sample[j].bx + (rand1()*2 - 1) * 100;
//			sample[i].by = sample[j].by + (rand1()*2 - 1) * 100;

//			sample[i].vx = sample[j].vx*(1 + (rand1()*2 - 1) * 0.2);
//			sample[i].vy = sample[j].vy*(1 + (rand1()*2 - 1) * 0.2);
			sample[i].vx = sample[j].vx + GAUSSIAN_RAND* 200;
			sample[i].vy = sample[j].vy + GAUSSIAN_RAND* 200;

			sample[i].vx += sample[i].bx - sample[j].bx;
			sample[i].vy += sample[i].by - sample[j].by;

			sample[i].weight += sample[j].weight / 2;
			sample[j].weight /= 2;

			double sum = 0;
			probhist[0] = sum; 
			for(int q = 0;q < NUM_SAMPLE;q++){
				sum += sample[q].weight;
				probhist[q+1] = sum; 
			}
*/
		}else{
			dest[i] = i;
		}
	}

	static BallParticle temp[NUM_SAMPLE];
	memcpy(temp,sample,sizeof(BallParticle)*NUM_SAMPLE);

	for(int i = 0;i < NUM_SAMPLE;i++){
		if(num_div[i] > 1){
			temp[i].weight /= num_div[i];
		}
	}

	for(int i = 0;i < NUM_SAMPLE;i++){
		if(num_div[i] == 0){
			int j = dest[i];
			sample[i].bx = temp[j].bx + GAUSSIAN_RAND * 50;
			sample[i].by = temp[j].by + GAUSSIAN_RAND * 50;
//			sample[i].bx = temp[j].bx + (rand1()*2 - 1) * 100;
//			sample[i].by = temp[j].by + (rand1()*2 - 1) * 100;

//			sample[i].vx = temp[j].vx*(1 + (rand1()*2 - 1) * 0.2);
//			sample[i].vy = temp[j].vy*(1 + (rand1()*2 - 1) * 0.2);
			sample[i].vx = temp[j].vx + GAUSSIAN_RAND* 200;
			sample[i].vy = temp[j].vy + GAUSSIAN_RAND* 200;

			sample[i].vx += temp[i].bx - temp[j].bx;
			sample[i].vy += temp[i].by - temp[j].by;

			sample[i].weight = temp[j].weight + temp[i].weight;
		}else{
			memcpy(&sample[i],&temp[i],sizeof(BallParticle));
		}
	}

	the_ball.bx = 0;
	the_ball.by = 0;
	the_ball.vx = 0;
	the_ball.vy = 0;
	the_ball.weight = 0;

	for(int q = 0;q < NUM_SAMPLE;q++){
		the_ball.bx += sample[q].bx * sample[q].weight;
		the_ball.by += sample[q].by * sample[q].weight;
		the_ball.vx += sample[q].vx * sample[q].weight;
		the_ball.vy += sample[q].vy * sample[q].weight;
		the_ball.weight += sample[q].weight;
	}

	the_ball.bx /= the_ball.weight;
	the_ball.by /= the_ball.weight;
	the_ball.vx /= the_ball.weight;
	the_ball.vy /= the_ball.weight;
	the_ball.weight /= the_ball.weight;

/*	double dvx = (the_ball.bx - last.bx);
	double dvy = (the_ball.by - last.by);
	for(int q = 0;q < NUM_SAMPLE;q++){
		sample[q].vx += dvx;
		sample[q].vy += dvy;
	}
*/
	last = the_ball;
}

void BallPDF::Move(double dx, double dy, double dtt, double dtime){
	dtt *= 3.1416 / 180;
	dtime /= 1000;


//	double a = DOUBLE_RAND;
	double a = GAUSSIAN_RAND*0.5 + 0.5;

	dx *= a;
	dy *= a;
	dtt *= a;

	double costheta = cos(dtt);
	double sintheta = sin(dtt);

	for(int i = 0;i < NUM_SAMPLE;i++){
		double bx = sample[i].bx + sample[i].vx * dtime;
		double by = sample[i].by + sample[i].vy * dtime;

		bx -= dx;
		by -= dy;


		double nbx = bx * costheta + by * sintheta;
		double nby = -bx * sintheta + by * costheta;
		double dbx = nbx - sample[i].bx;
		double dby = nby - sample[i].by;
		sample[i].bx += dbx;
		sample[i].by += dby;


		bx = sample[i].vx;
		by = sample[i].vy;
		sample[i].vx =  bx * costheta + by * sintheta;
		sample[i].vy = -bx * sintheta + by * costheta;

	}

	for(int q = 0;q < NUM_SAMPLE;q++){
		the_ball.bx += sample[q].bx * sample[q].weight;
		the_ball.by += sample[q].by * sample[q].weight;
		the_ball.vx += sample[q].vx * sample[q].weight;
		the_ball.vy += sample[q].vy * sample[q].weight;
		the_ball.weight += sample[q].weight;
	}

	the_ball.bx /= the_ball.weight;
	the_ball.by /= the_ball.weight;
	the_ball.vx /= the_ball.weight;
	the_ball.vy /= the_ball.weight;
	the_ball.weight /= the_ball.weight;
/*
	for(int i = 0;i < NUM_SAMPLE;i++){
		double bx = sample[i].bx;
		double by = sample[i].by;

		bx -= dx;
		by -= dy;


		double nbx = bx * costheta + by * sintheta;
		double nby = -bx * sintheta + by * costheta;
		double dbx = nbx - sample[i].bx;
		double dby = nby - sample[i].by;

		sample[i].bx = nbx;
		sample[i].by = nby;

		bx = sample[i].vx;
		by = sample[i].vy;

		sample[i].vx =  ( bx * costheta + by * sintheta);
		sample[i].vy =  (-bx * sintheta + by * costheta);
		sample[i].vx -= dbx / dtime;
		sample[i].vy -= dby / dtime;
	}
*/
}

void BallPDF::Progress(double dtime){
	dtime /= 1000;

	double rr = pow(0.5,dtime / 1000);
//	rr = 1;

	for(int i = 0;i < NUM_SAMPLE;i++){
		sample[i].bx += sample[i].vx * dtime;
		sample[i].by += sample[i].vy * dtime;

		sample[i].vx *= rr;
		sample[i].vy *= rr;
	}
}
