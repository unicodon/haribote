#ifndef LOGGER_H
#define LOGGER_H

#include <set>

#define LOGGER_BUFSIZE 1024*256

class Logger{
public:
	Logger(const char* filename, bool add = false);
	~Logger();

	void Flush();
	void Write(const char* str, int len);
	void WriteString(const char* str);
	void Register();

	static void FlushAll();

private:
	static std::set<Logger*> pointer_set;
	char fname[256];
	char buf[LOGGER_BUFSIZE];
	int current;
};



#endif
