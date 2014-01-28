//
//  時間計測用です．
//  GetTimer(n)は，SetTimer(n)を実行した時からの
//  経過時間をマイクロ秒単位で返します
//

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <map>

#define LOGGER_BUFSIZE 1024*256

class Logger{
public:
	Logger(const char* filename, bool add = false);
	~Logger();

	void Flush();
	void Write(const char* str, int len);

private:
	char fname[256];
	char buf[LOGGER_BUFSIZE];
	int current;

};

#endif
