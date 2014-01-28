#include "StdAfx.h"
#include ".\ppmreader.h"

PPMReader::PPMReader(void)
{
//	image = NULL;
}

PPMReader::~PPMReader(void)
{
//	if(image)
//		delete[] image;
}

int PPMReader::ReadFile(const char *path, Texture &texture)
{
	CFile file;
	if(!file.Open(path,CFile::modeRead)){
		return -1;
	}

	int maxvalue,width,height;
	width = height = maxvalue= 0;
	char magic[3];
	file.Read(magic,2);
	magic[2] = 0;
	if(strcmp(magic,"P6")) return 0;
	char reader;
	enum mode{WID,HEI,MAV,END} mode;
	mode = WID;
	while(TRUE){
		//ãÊêÿÇËï∂éöîÚÇŒÇµ
		file.Read(&reader,1);
		if((reader == 0x20 || reader == 0x0a || reader == 0x0d))
			break;
	}
	while(TRUE){
		if(reader == '#'){
			while(TRUE){
				file.Read(&reader,1);
				if((reader == 0x20 || reader != 0x0a || reader == 0x0d))
					break;
			}
			continue;
		}
		switch(mode){
		case WID:
			while(TRUE){
				file.Read(&reader,1);
				if(reader < '0' || reader > '9') break;
				width *= 10;
				width += (reader - '0');
			}
			mode = HEI;
			break;
		case HEI:
			while(TRUE){
				file.Read(&reader,1);
				if(reader < '0' || reader > '9') break;
				height *= 10;
				height += (reader - '0');
			}
			mode = MAV;
			break;
		case MAV:
			while(TRUE){
				file.Read(&reader,1);
				if(reader < '0' || reader > '9') break;
				maxvalue *= 10;
				maxvalue += (reader - '0');
			}
			mode = END;
			break;
		}
		if(mode == END)
			break;
	}

//	if(image != NULL)
//		delete[] image;
	BYTE* &image = texture.data;
	image = new BYTE[width*height*3];
	int readsize = file.Read(image,height*width*3);
	file.Close();
	if(readsize < width*height*3){
		delete[] image;
		return -1;
	}

//	WIDTH = width;
//	HEIGHT = height;

	texture.width = width;
	texture.height = height;

	return 0;
}
