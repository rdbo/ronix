#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <pch.hpp>

class Logger {
private:
	std::string filepath;
	FILE *file = nullptr;
public:
	Logger(std::string output_file);
	~Logger();
	void Log(const char *fmt, ...);
	void Log(const char *fmt, va_list arg);
};

#endif
