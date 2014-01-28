//
//  ���Ԍv���p�ł��D
//  GetTimer(n)�́CSetTimer(n)�����s�����������
//  �o�ߎ��Ԃ��}�C�N���b�P�ʂŕԂ��܂�
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
