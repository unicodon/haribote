#include "stdafx.h"
#include <map>
#include "logger.h"

using namespace std;

std::set<Logger*> Logger::pointer_set;//全Loggerインスタンスのポインタを管理(FlushAll用)


Logger::Logger(const char* filename, bool add){
	strcpy(fname,filename);
	current = 0;

	if(!add){
		fclose(fopen(fname,"w"));
	}
}

Logger::~Logger(){
	Flush();
	set<Logger*>::iterator it = pointer_set.find(this);
	pointer_set.erase(it);
}

void Logger::WriteString(const char* str){
	Write(str,strlen(str));
}

void Logger::Register(){
	pointer_set.insert(this);
}

void Logger::Write(const char* str, int len){
	if(len > LOGGER_BUFSIZE - current){//書き込む
		FILE* fp = fopen(fname,"a");
		if(!fp)return;
		int index = 0;
		while(current - index > 0){
			index += fwrite(buf+index,1,current-index,fp);
		}
		current = index = 0;
		while(len - index > 0){
			index += fwrite(str+index,1,len-index,fp);			
		}
		fclose(fp);
	}else{
		memcpy(buf+current,str,len);
		current += len;
	}
}

void Logger::Flush(){
	FILE* fp = fopen(fname,"a");
	if(!fp)return;
	int index = 0;
	while(current - index > 0){
		index += fwrite(buf+index,1,current-index,fp);
	}
	current = index = 0;
	fclose(fp);
}

void Logger::FlushAll(){
	set<Logger*>::iterator it;
	for(it = pointer_set.begin(); it != pointer_set.end();it++){
		(*it)->Flush();
	}
};