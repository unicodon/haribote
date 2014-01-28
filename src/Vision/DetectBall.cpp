#include "StdAfx.h"
#include <math.h>
#include <stdio.h>
#include "DetectBall.h"
#include <OPENR/OSyslog.h>
#include <string>
#include "../Utility/MySTL.h"
#include "VisionUtil.h"
#include "../Motion/HeadController.h"
#include "../Motion/MotionMaker.h"
#include <vector>
#include <iostream>
#include "../Utility/Logger.h"

using namespace std;

struct Point{
	int x;
	int y;
};

struct ScanLine{
	int line;
	int min;
	int max;
	int length;
};

circle_param DetectBall::circle;
int DetectBall::orangew;
int DetectBall::orangeh;

static std::vector<Point> edge_points;
static std::vector<ScanLine> lines;

#define BALL_RECOG_TH		30	 // CDTによるボール認識のしきい値
#define CIRCLE_CDT_TH		14000 // 円抽出/CDT結果利用切り替えのしきい値

Predictor DetectBall::pred_x;
Predictor DetectBall::pred_y;


DetectBall::DetectBall(){
}

DetectBall::~DetectBall(){
}

void DetectBall::Execute(){

	static double	br, btt;//ボールの距離，ボールの角度（ｒ，θ）
	static double	bx,by;
	if(!MotionMaker::IsFinished()){
		br = 60000;
		btt = 0;
		state.ball.visible = false;
		state.ball.visibletime = 0;
		state.ball.distance = br;
		state.ball.direction = btt;
		state.ball.SetPosition(br,btt);
		return;
	}

	int orange = state.self.cdt[ORANGE].value;
	int red = state.self.cdt[RED].value;

#define COEF 210.0

	BallInfo info;

	if(orange > 8)
	{
		state.self.ballpos_in_image = state.self.cdt[ORANGE];

		DetectEdge();
		if(!Judge()){
			state.ball.visible = false;
			state.ball.losttime++;
			state.ball.visibletime = 0;

			state.virtual_ball.x = pred_x.Prediction(1);
			state.virtual_ball.y = pred_y.Prediction(1);

			pred_x.AddData(state.virtual_ball.x);
			pred_y.AddData(state.virtual_ball.y);
		}else{
			state.self.ballpos_in_image.x = (int)circle.param[0];
			state.self.ballpos_in_image.y = (int)circle.param[1];
			state.self.ballpos_in_image.value = (int)(circle.param[2]*circle.param[2]*PI);
			circle.param[2] += 1;
			double r = circle.param[2]*COEF/sqrt(circle.param[2]*circle.param[2] + COEF*COEF);

			//tracking用のtilt1を設定
			SetTrackingTilt1(r);
			SetTrackingV(r);

/*			static bool starter = false;
			static int dis = 0;
			if(state.self.sensor.backTouch[1] && starter == false){
				starter = true;
				cin >> dis;
		//		OSYSPRINT(("%.1f,%.1f\n",state.ball.distance,state.ball.direction));
			}

			if(starter){
				static int counter = 0;
				if(counter % 3 == 0){
					OSYSPRINT(("%d,%.1f\n",dis,r));
				}
				counter++;
				if(counter > 90){
					counter = 0;
					starter = false;
				}
			}

*/
			if(r < 2)r = 2;
//			r = 42*COEF/r + 90;//42=ball 90= nose
//			if(r < 180) r = 180;//180=頭部第2関節の高さ-ボール半径
//			r = sqrt(r*r - 180*180)+35;//25=頭部第2関節のy座標

/*
//			br = r;
			if(r < 30){
				br = 37.68 + 8475 / r;
			}else{
				br = 37.68 + 8475 / r;
			}
*/

			//okuzumi
			double d2,d3,d4,d5,d6,d7,psi;
			double y_cent = state.self.ballpos_in_image.y;
			
			d2 = 8400 / r + 30;//8550/r + 50;		//カメラからボールまでの距離
			
			psi = (-1.0*45.2*(y_cent - (double)IMAGE_HEIGHT/2)/(double)IMAGE_HEIGHT);

			double tilt2 = state.self.sensor.jointValue[TILT2];

			d6 = d2/cos(psi*3.14/180);
			d7 = sqrt(d6*d6-81.6*81.6+2*81.6*(81.6+d2));
			
			d4 = d7*cos(tilt2*3.14/180);
			d5 = 65.4 + d7*sin(tilt2*3.14/180);
			d3 = d4*d4 + d5*d5;
			br = sqrt(d3 - 80.0*80.0);
			//ここまで
			btt = PixelToDirection((int)circle.param[0], (int)circle.param[1]);

			info.distance = br;
			info.direction = btt;


//			static bool lasttouch = false;
//			if(!lasttouch && state.self.sensor.backTouch[0]){
//				lasttouch = true;
//				OSYSPRINT(("\n",r,br));
//			}else{
//				lasttouch = false;
//			}
//			if(state.self.sensor.backTouch[0]){
//				OSYSPRINT(("%.1f,%1.f\n",r,br));
//			}

			//スムージング処理 by kase//////////////////////////////////////////////////////////
			static double temp_br=br;
			static double temp_btt=btt;
			//OSYSPRINT(("br = %.1f_", br));
			//OSYSPRINT(("temp = %.1f", temp_br));
			temp_br=(temp_br+br)/2.0;
			temp_btt=(temp_btt+btt)/2.0;
			//OSYSPRINT(("nyu = %.1f\n", temp_br));
			state.ball.SetPosition(temp_br,temp_btt);
			temp_br=br;
			temp_btt=btt;
			///スムージング処理を使わない場合は
			//↓のstate.ball.SetPosition(br,btt);コメントアウトをはずしてください//////////////

			//計算結果を出力
			//state.ball.SetPosition(br,btt);
			state.ball.visible = true;
			state.ball.losttime = 0;
			state.ball.visibletime ++;
		
			//実験用
			pred_x.AddData(state.ball.x);
			pred_y.AddData(state.ball.y);
		}
	}
	else
	{		
		state.ball.visible = false;
		state.ball.losttime++;
		state.ball.visibletime = 0;

		state.virtual_ball.x = pred_x.Prediction(1);
		state.virtual_ball.y = pred_y.Prediction(1);

		pred_x.AddData(state.virtual_ball.x);
		pred_y.AddData(state.virtual_ball.y);
	}

	double ex = pred_x.MakePredictor(10);
	double ey = pred_y.MakePredictor(10);
/*	if(ex > 200 || ey > 200){
		pred_x.Clear();
		pred_y.Clear();
		pred_x.AddData(state.ball.x);
		pred_y.AddData(state.ball.y);
	}
*/
	state.virtual_ball.x = pred_x.Prediction(0);
	state.virtual_ball.y = pred_y.Prediction(0);


	state.virtual_ball.distance = sqrt(state.virtual_ball.x*state.virtual_ball.x + state.virtual_ball.y*state.virtual_ball.y);
	state.virtual_ball.direction = atan2(state.virtual_ball.y,state.virtual_ball.x)/3.14*180;


//	static Logger logger("./MS/BALLLOG.TXT");
//	logger.Register();

	char strbuf[256];



	//state.ballpdf.Progress(33);//たぶん，BallPDF用の処理．コメントアウト by takeshita 06/05/18

//	sprintf(strbuf,"%.1f,%.1f,",state.ball.distance,state.ball.direction);
//	logger.WriteString(strbuf);

	//BallPDF
	if(state.ball.visible){
		/*時間短縮のため，行わない
		//ぼーるPDFの実行
		state.ballpdf.Update(info);

		double bx = state.ballpdf.the_ball.bx + state.ballpdf.the_ball.vx * 20 / 1000.0;
		double by = state.ballpdf.the_ball.by + state.ballpdf.the_ball.vy * 20 / 1000.0;

		double dir = atan2(by,bx)*180/3.1416;
		double dist = sqrt(bx*bx + by*by);
		*/

//		sprintf(strbuf,"%1f,%1f\n",dist,dir);

/*
		static int cnt = 0;
		if(cnt++ > 10){
			OSYSPRINT(("%.1f,%.1f\n",dir,dist));
			cnt = 0;
		}
*/

//パーティクルによるボール推定値を代入
		//キーパーはやらない
		/*if(state.game.robot_id != 1){
			state.ball.x = bx;
			state.ball.y = by;
			state.ball.direction = dir;
			state.ball.distance = dist;
		}*/

	}else{
//		sprintf(strbuf,"0,0\n");
	}
//	logger.WriteString(strbuf);


/*	if(state.ball.losttime > 10){
		state.virtual_ball.visible = false;
	}else{
		state.virtual_ball.visible = true;
	}
	int losttime = state.ball.losttime;
	bool visible = state.ball.visible;
	state.ball = state.virtual_ball;
	state.ball.losttime = losttime;
	state.ball.visible = visible;
*/

	//OSYSPRINT(("d = %.1f,ORANGE = %d\n",state.ball.distance,state.self.cdt[ORANGE].value));
		
}

double DetectBall::Dist2Ball(int npix)
{
/*	double a = 63;
	double b = 226e6;

	if(npix <= 10)
		return 40000;
	
	return sqrt(b/(double)npix) + a;
*/
	if(npix < 10)return 60000;
	return 16140/sqrt((double)npix);
}


void DetectBall::CutOffHorizon(){
	if(state.self.horizone.perpend)return;
	for(int x = 0;x < IMAGE_WIDTH;x++){
		int y0 = (int)(state.self.horizone.intercept + state.self.horizone.tilt*x) + 50;
		if(y0 > IMAGE_HEIGHT) y0 = IMAGE_HEIGHT;
		for(int y = 0; y < y0; y++){
			if(state.self.CDTimage[x+y*IMAGE_WIDTH] == ORANGE)
				state.self.CDTimage[x+y*IMAGE_WIDTH] = 255;
		}
	}
}

//linescanでrunを検出→runのedgeを見つける
int DetectBall::DetectEdge(){

#define GAP_COEF 0.05

	if(state.self.cdt[ORANGE].value < 40)return 0;
	int gap = (int)(sqrt((double)state.self.cdt[ORANGE].value)*GAP_COEF+1);
	int  n = 0, topline = 0, bottomline = 0, maxnum = 0;
	int tmptop = 0,tmpbottom = 0;
	bool cont = false;//ラインが連続しているか
	bool detectline = false;

	int mode, counter;
	int mass = 0;
	ScanLine myline;
	int maxline = 0;
	int SCAN_HEIGHT = IMAGE_HEIGHT;

	lines.clear();

	if(state.self.cdt[ORANGE].value > 20000)SCAN_HEIGHT = int(IMAGE_HEIGHT*0.7);

	for(int y = 0; y < SCAN_HEIGHT; y += gap){
		mode = 0;
		counter = 0;
		mass = 0;
		detectline = false;
		myline.line = y;
		for(int x = 0; x < IMAGE_WIDTH;x++){
			if(mode){
				if(state.self.CDTimage[x+y*IMAGE_WIDTH] != ORANGE){
					counter++;
					if(counter > 5){
						mode = 0;
						counter = 0;
						myline.length = myline.max - myline.min;
						if(myline.length > 3 && mass > myline.length * 0.7){
							myline.line = y;
							detectline = true;
							if(!cont){
								tmptop = lines.size();
								cont = true;
							}
							tmpbottom = lines.size();
							n++;
							lines.push_back(myline);
							int a = lines[maxline].length;
							int b = myline.length;
							if(lines[maxline].length < myline.length)
								maxline = lines.size() - 1;
						}
						mass = 0;
					}
				}else{
					myline.max = x;
					mass++;
				}
			}else{
				if(state.self.CDTimage[x+y*IMAGE_WIDTH] == ORANGE){
					counter++;;
					mass++;
					if(counter > 1){
						mode = 1;
						counter = 0;
						myline.max = x;
						myline.min = x - 1;
					}
				}
			}
		}
		if(mode == 1){
			myline.length = myline.max - myline.min;
			if(myline.length > 3 && mass > myline.length * 0.7){
				if(lines.size() > 0 && lines[maxline].length < myline.length)
					maxline = lines.size() - 1;
				myline.line = y;
				detectline = true;
				if(!cont){
					tmptop = lines.size();
					cont = true;
				}
				tmpbottom = lines.size();
				n++;
				lines.push_back(myline);
			}
		}
		if(!detectline){
			if(n > maxnum){
				maxnum = n;
				topline = tmptop;
				bottomline = tmpbottom;
			}
			cont = false;
			n = 0;
		}		
	}
	if(detectline){
		if(n > maxnum){
			maxnum = n;
			topline = tmptop;
			bottomline = tmpbottom;
			cont = false;
		}
	}

	edge_points.clear();
	if(maxnum >= 2){
		int cur_line = lines[topline].line;
		int total_length = lines[topline].length;
		int min = lines[topline].min;
		int max = lines[topline].max;
		for(int i = topline;i < bottomline;i++){
			if(cur_line != lines[i].line){
				int length = max - min;
				if(total_length > length*0.7){
					Point point;
					point.y = cur_line;
					point.x = min;
					if(point.x > 15){
						edge_points.push_back(point);
					}
					point.x = max;
					if(point.x < IMAGE_WIDTH - 15){
						edge_points.push_back(point);
					}

				}

				cur_line = lines[i].line;
				total_length = lines[i].length;
				min = lines[i].min;
				max = lines[i].max;
			}else{
				total_length += lines[i].length;
				max = lines[i].max;
			}
		}
	}

	if(lines.size() > 0 && edge_points.size() > 0){
		int v = lines[maxline].line;
		int u = (lines[maxline].max + lines[maxline].min)/2;

		circle.param[0] = u;
		circle.param[1] = v;
		circle.param[2] = (lines[maxline].max - lines[maxline].min)/2;

		if(edge_points.size() >= 6){
			LeastSquare();
		}
	}

	return edge_points.size();

}

double DetectBall::LeastSquare(){
	if(edge_points.size() < 4)return -1;

	double u = 0,uu = 0,uuu = 0, v = 0, vv = 0, vvv = 0;
	double uv = 0,uuv = 0,uvv = 0;
	int num = 0;
	int size = edge_points.size();
	for(int i = 0;i < size; i++){
		int x = edge_points[i].x;
		u += x;x *= edge_points[i].x;
		uu += x;x *= edge_points[i].x;
		uuu += x;
		int y = edge_points[i].y;
		v += y;y *= edge_points[i].y;
		vv += y;y *= edge_points[i].y;
		vvv += y;
		uv  += edge_points[i].x * edge_points[i].y;
		uuv += edge_points[i].x * edge_points[i].x * edge_points[i].y;
		uvv += edge_points[i].x * edge_points[i].y * edge_points[i].y;
	}
	u /= size;uu /= size; uuu /= size;
	v /= size;vv /= size; vvv /= size;
	uv /= size; uuv /= size; uvv /= size;

	double a,b,c,d,e,f;
	a = 2*(uu - u*u);
	b = c = 2*(uv - u*v);
	d = 2*(vv - v*v);
	e = uuu - u*uu + uvv - u*vv;
	f = vvv - v*vv + uuv - uu*v;

	double det = a*d - b*c;
	if(fabs(det) < 0.01)return -1;

	double A,B,C,D;
	A = d / det;
	B = -b / det;
	C = -c / det;
	D = a / det;

	double x = e*A + f*C;
	double y = e*B + f*D;

	double r = sqrt(x*x - 2*x*u + uu + y*y - 2*y*v + vv);

	circle.param[0] = x;
	circle.param[1] = y;
	circle.param[2] = r;

/*
	static int counter = 0;
	if(counter++ > 50){
		static int the = 0;
		counter = 0;
		BYTE buf[IMAGE_WIDTH*IMAGE_HEIGHT*3];
		memcpy(buf,state.self.image,IMAGE_WIDTH*IMAGE_HEIGHT*3);
		for(int X = 0;X < 208;X++){
			for(int Y = 0;Y<160;Y++){
				if(fabs(r*r - (x-X)*(x-X)-(y-Y)*(y-Y)) < 4)
					buf[(X+Y*208)*3] = 0;
			}
		}
		for(int i = 0;i < size;i++){
			buf[(edge_points[i].x+edge_points[i].y*208)*3] = 255;
		}
		char name[64];
		sprintf(name,"CI%03d",the++);
		SaveYUVPPMImage(buf,name);

	}
*/
	return 0;
}

bool DetectBall::Judge(){
	int top    = (int)(circle.param[1] - circle.param[2]) - 2;
	int left   = (int)(circle.param[0] - circle.param[2]) - 2;
	int right  = (int)(circle.param[0] + circle.param[2]) + 2;
	int bottom = (int)(circle.param[1] + circle.param[2]) + 2;
	int cx = (int)circle.param[0];
	int cy = (int)circle.param[1];
	int rr = (int)(circle.param[2]*circle.param[2]);

	if(top < 0)top = 0;if(top > IMAGE_HEIGHT)top = IMAGE_HEIGHT;
	if(left < 0)left = 0;if(left > IMAGE_WIDTH)left = IMAGE_WIDTH;
	if(right < 0)right = 0;if(right > IMAGE_WIDTH)right = IMAGE_WIDTH;
	if(bottom < 0)bottom = 0;if(bottom > IMAGE_HEIGHT)bottom = IMAGE_HEIGHT;

	int ugap = (right - left)/10 + 3;
	int vgap = (bottom - top)/10 + 3;

	int sumin = 0;
	int sumout = 0;
	int in = 0;
	int out = 0;

	for(int u = left; u < right;u+=ugap){
		for(int v = top; v < bottom; v+=vgap){
			if((u-cx)*(u-cx) + (v-cy)*(v-cy) < rr){
				sumin++;
				if(state.self.CDTimage[u+v*IMAGE_WIDTH] == ORANGE){
					in++;
				}
			}else{
				sumout++;
				if(state.self.CDTimage[u+v*IMAGE_WIDTH] == ORANGE){
					out++;
				}
			}
		}
	}

	double alpha = (double)in/sumin-(double)out/sumout;
	int r = (int)circle.param[2];

	if(r > 2){
		if(alpha > 0.4 && r > 40)
			return true;
		if(alpha > 0.5 && r > 20)
			return true;
		if(alpha > 0.7 || alpha > r*0.03){
			return true;
		}
	}else{
		if((double)out/sumout < 0.1 && alpha > 0.5)
			return true;
	}

	if(state.self.cdt[ORANGE].value > 6000){
		circle.param[0] = state.self.cdt[ORANGE].x;
		circle.param[1] = state.self.cdt[ORANGE].y;
		circle.param[2] = sqrt(state.self.cdt[ORANGE].value*1.2/PI);
		return true;
	}
	return false;
}

//ボールとの距離に応じてトラッキングのTilt1を変更
void DetectBall::SetTrackingTilt1(double radius){
	//OSYSPRINT(("r = %.1f,",radius));
	double t1;
	double theta1,theta2,d2;
//	t1 = 17.22 - 0.60551 * radius + 0.0016*radius*radius;//ボールの大きさ（半径）と距離を計測後，エクセルで最小二乗法を行うことでパラメータを推定
//	t1 -= 7;

//*okuzumi
	d2 = 8000/(radius+1);
	theta2 = atan((d2+80)/65);
	theta1 = acos(80/65*cos(theta2));
	t1 = (theta1 + theta2)*180.0/3.14 -180;

	//OSYSPRINT(("oku_d = %.1f\n",d2));

	if(t1 > 0) t1 = 0;
	if(t1 < -25) t1 = -25;
//*/
	HeadController::SetTrackingTilt1(t1);
	HeadController::SetRadius(radius);
}

void DetectBall::SetTrackingV(double radius){
	double v;
	if(radius < 10){
		v = IMAGE_HEIGHT *0.5;//0.80;/// 2;
	}else{
		v = IMAGE_HEIGHT *(-0.00005*(radius-10)*(radius-10)+0.0075*(radius-10)+0.5);
	}
	HeadController::SetTrackingV(v);
	//OSYSPRINT(("r = %.1f t_v = %.1f\n",radius,v));
}

