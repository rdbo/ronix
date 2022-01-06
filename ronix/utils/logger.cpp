#include "logger.hpp"

Logger::Logger(const char *output_file)
{
	this->file = fopen(output_file, "w");
}

Logger::~Logger()
{
	if (this->file)
		fclose(this->file);
}

void Logger::Log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	this->Log(fmt, arg);
	va_end(arg);
}

void Logger::Log(const char *fmt, va_list arg)
{
	fprintf(this->file, "[RONIX] ");
	vfprintf(this->file, fmt, arg);
	fflush(this->file);
}
