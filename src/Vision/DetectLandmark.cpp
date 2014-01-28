#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "DetectLandmark.h"
#include "../State/State.h"

#include <sstream>
#include <fstream>
#include <stdio.h>


void DetectLandmark::Execute(){

	if(state.self.horizone.perpend)
		return;
	//傾きと切片
	double tilt = state.self.horizone.tilt;
	int intercept = (int)(-IMAGE_WIDTH/2*tilt + state.self.horizone.intercept + IMAGE_HEIGHT/2);

	//雑音除去
	int valid_line = 0;
	int last_lines = 0;

	intercept -= 10;

	LandmarkInformation li;

	li = ScanLine(tilt,intercept);

	const int lines[10] = {2,4,8,12,16,24,32,48,64,72};
	for(int i = 0;i < 10;i++){
		//OSYSPRINT(("input:%d ",lines[i]));
		LandmarkInformation temp_li;
		
		temp_li = ScanLine(tilt,intercept + lines[i]);
		if(temp_li.width > li.width)
		{
			//OSYSPRINT(("change li input = %d width = %d\n",lines[i],li.width));
			li = temp_li;
			last_lines = i;
		}
		if(temp_li.width > 0) valid_line++;
		
		temp_li = ScanLine(tilt,intercept - lines[i]);
		if(temp_li.width > li.width)
		{
			//OSYSPRINT(("change li input = %d width = %d\n",(-lines[i]),li.width));
			li = temp_li;
			last_lines = -i;
		}
		if(temp_li.width > 0) valid_line++;
	}
	//OSYSPRINT(("\n"));
	if(li.width != 0 &&
		!(valid_line == 1 && (abs(last_lines) == 8 || abs(last_lines) == 9)))//最後の2ラインでとれた単独のデータはあまり信用しない
	{
		ObserveLandmark::Observation(li);
#define DATA_LENGTH 1000
#define DATA_WAIT 100
		//static int count_view = 0;
		
		/*static FILE* file;
		if(count_view > DATA_WAIT && count_view < DATA_LENGTH+DATA_WAIT+1)
		{
			if(file == (FILE*) 0)
			{
				OSYSPRINT(("OPEN FILE\n"));
				file = fopen("./MS/landmark.dat","a");
			}
			if(file){
				stringstream ss;
				ss << li.width << "\t" << li.angle <<"\t" << li.pixel<< "\t" << li.max_pixel << "\t" << li.id << "\n";
				fwrite(ss.str().c_str(),1,strlen(ss.str().c_str()),file);
				if(count_view % 100 == 0) fflush(file);
			}
			if(count_view == DATA_LENGTH+DATA_WAIT)
			{
				fclose(file);
				OSYSPRINT(("CLOSE FILE\n"));
			}
		}
		OSYSPRINT(("%d\n",count_view));
		count_view++;*/
		
		/*{
			static int count_save = 0;
			stringstream ss;
			if(count_save < 10)
			{
				ss << "000" << count_save;
			}
			else if(count_save < 100)
			{
				ss << "00" << count_save;
			}
			SaveYUVPPMImage(state.self.image,ss.str().c_str());
			count_save++;
		}*/
	}
}

LandmarkInformation DetectLandmark::ScanLine(double tilt, int intercept){
	//OSYSPRINT(("tilt = %f intercept = %d\n",tilt,intercept));
//一列にこれ以上の数のランドマークの色(YELLOW,SKYBLUE)が見えたらランドマークがある
#define NUM_OF_LANDMARK_PIXEL 5
//LANDMARK_FACTOR*width以上のランドマークの色(YELLOW,SKYBLUE)が見えたらランドマークがある
#define LANDMARK_FACTOR 1.5
	int mode = 0;
	const int gap = 2;
	const int gap_of_pink = 6;
	int left = 0;
	int right = 0;
	int gapcount = 0;
	for(int x = 0;x < IMAGE_WIDTH;x++){
		int y = (int)(intercept + x*tilt);
		if(y < 0 || y >= IMAGE_HEIGHT)
			continue;

//		state.self.image[(x+y*IMAGE_WIDTH)*3] = 255;//test

		if(state.self.CDTimage[x+y*IMAGE_WIDTH] == PINK){
			if(mode == 0)
				gapcount++;

			if(mode == 1){
				right = x;
				gapcount = 0;
			}

			if(mode == 0 && gapcount > gap){
				left = x - gap;
				right = x;
				mode = 1;
				gapcount = 0;
				continue;
			}
		}else{
			if(mode == 0)
				gapcount = 0;
			if(mode == 1)
				gapcount++;

			if(mode == 1 && gapcount > gap_of_pink){
				mode = 2;
			}
		}
	}

	int width = right - left + 1;
	//if(width != 1) OSYSPRINT(("left = %d right = %d width = %d\n",left,right,width));
	if(width < 3 || left <= 2 || right >= IMAGE_WIDTH - 3)//幅3pixel未満は除外
	{
		LandmarkInformation info;
		info.id = (LANDMARK_ID)-1;
		info.width = 0;
		return info;
	}

	int pos_x = (right + left) / 2;
	int pos_y = (int)(intercept + pos_x * tilt);


	int sum_yellow = 0;
	int sum_skyblue = 0;
	int max_sum_yellow_line = 0;
	int max_sum_skyblue_line = 0;

	//上方サーチ
	for(int j = 0;j < 3;j++){
		int x = left + (j+1)*width / 4;
		int y = (int)(intercept + x*tilt);
		int sum_yellow_line = 0;
		int sum_skyblue_line = 0;
		for(int i = 0;i < width*3;i++){
			int v = y - i;
			int u = (int)(x + tilt*i);
			if(u < 0 || u >= IMAGE_WIDTH || v < 0 || v > IMAGE_HEIGHT)
				continue;

//			state.self.image[(u+v*IMAGE_WIDTH)*3+2] = 255;//test

			if(state.self.CDTimage[u+v*IMAGE_WIDTH] == YELLOW)
			{
				sum_yellow++;
				sum_yellow_line++;
			}
			if(state.self.CDTimage[u+v*IMAGE_WIDTH] == SKYBLUE)
			{
				sum_skyblue++;
				sum_skyblue_line++;
			}
		}
		if(max_sum_yellow_line < sum_yellow_line)
		{
			max_sum_yellow_line = sum_yellow_line;
		}
		if(max_sum_skyblue_line < sum_skyblue_line)
		{
			max_sum_skyblue_line = sum_skyblue_line;
		}
	}
	if(max_sum_yellow_line >= NUM_OF_LANDMARK_PIXEL || sum_yellow > LANDMARK_FACTOR*width){
		LandmarkInformation li;
		li.id = HIGH_YELLOW_LOW_PINK;
		li.width = width;
		li.angle = PixelToDirection(pos_x,pos_y);


//		OSYSPRINT(("FindLandmark YP: width = %d, direction = %.1f\n",li.width, li.angle));
		return li;
	}
	if(max_sum_skyblue_line >= NUM_OF_LANDMARK_PIXEL || sum_skyblue > LANDMARK_FACTOR*width){

		LandmarkInformation li;
		li.id = HIGH_SKYBLUE_LOW_PINK;
		li.width = width;
		li.angle = PixelToDirection(pos_x,pos_y);

//		OSYSPRINT(("FindLandmark SP: width = %d, direction = %.1f\n",li.width, li.angle));
		return li;
	}
	
	sum_yellow = 0;
	sum_skyblue = 0;
	max_sum_yellow_line = 0;
	max_sum_skyblue_line = 0;
	//下方サーチ
	for(int j = 0;j < 3;j++){
		int x = left + (j+1)*width / 4;
		int y = (int)(intercept + x*tilt);
		int sum_yellow_line = 0;
		int sum_skyblue_line = 0;
		for(int i = 0;i < width*3;i++){
			int v = y + i;
			int u = (int)(x - tilt*i);
			if(u < 0 || u >= IMAGE_WIDTH || v < 0 || v > IMAGE_HEIGHT)
				continue;

//			state.self.image[(u+v*IMAGE_WIDTH)*3+2] = 255;//test

			if(state.self.CDTimage[u+v*IMAGE_WIDTH] == YELLOW)
			{
				sum_yellow++;
				sum_yellow_line++;
			}
			if(state.self.CDTimage[u+v*IMAGE_WIDTH] == SKYBLUE)
			{
				sum_skyblue++;
				sum_skyblue_line++;
			}
		}
		if(max_sum_yellow_line < sum_yellow_line)
		{
			max_sum_yellow_line = sum_yellow_line;
		}
		if(max_sum_skyblue_line < sum_skyblue_line)
		{
			max_sum_skyblue_line = sum_skyblue_line;
		}
	}
	//OSYSPRINT(("sum_skyblue = %d sum_yellow = %d max_skyblue = %d max_yellow = %d\n",
	//	sum_skyblue,sum_yellow,max_sum_skyblue_line,max_sum_yellow_line));

	if(max_sum_yellow_line >= NUM_OF_LANDMARK_PIXEL || sum_yellow > LANDMARK_FACTOR*width){
		//if(sum_yellow > 2.5*width)
		//	width = sum_yellow / 2.5;
		LandmarkInformation li;
		li.id = HIGH_PINK_LOW_YELLOW;
		li.width = width;
		li.angle = PixelToDirection(pos_x,pos_y);

//		OSYSPRINT(("FindLandmark PY: width = %d, direction = %.1f\n",li.width, li.angle));
		return li;
	}
	if(max_sum_skyblue_line >= NUM_OF_LANDMARK_PIXEL || sum_skyblue > LANDMARK_FACTOR*width){
		//if(sum_skyblue > 2.5*width)
		//	width = sum_skyblue / 2.5;

		LandmarkInformation li;
		li.id = HIGH_PINK_LOW_SKYBLUE;
		li.width = width;
		li.angle = PixelToDirection(pos_x,pos_y);

//		OSYSPRINT(("FindLandmark PS: width = %d, direction = %.1f\n",li.width, li.angle));
		return li;
	}

	LandmarkInformation info;
	info.id = (LANDMARK_ID)-1;
	info.width = 0;
	return info;
}

