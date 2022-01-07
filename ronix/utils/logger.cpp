#include "logger.hpp"

Logger::Logger(const char *output_file)
{
	this->filepath = output_file;
	this->file = std::fopen(output_file, "w");
}

Logger::~Logger()
{
	if (this->file)
		std::fclose(this->file);
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
	// TODO: Only print [RONIX] on new line
	std::fprintf(this->file, "[RONIX] ");
	std::vfprintf(this->file, fmt, arg);
	std::fflush(this->file);
}
