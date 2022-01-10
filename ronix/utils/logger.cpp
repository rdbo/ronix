#include "logger.hpp"
#include <sys/stat.h>
#include <fcntl.h>

Logger::Logger(std::string output_file)
{
	this->filepath = output_file;
	this->file = std::fopen(this->filepath.c_str(), "w");
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
	int fd = open(this->filepath.c_str(), O_RDONLY);
	if (fd != -1) {
		char c;
		lseek(fd, -1, SEEK_END);
		int result = read(fd, reinterpret_cast<void *>(&c), sizeof(c));
		if (result == 0 || c == '\n')
			std::fprintf(this->file, "[RONIX] ");
		close(fd);
	}
	std::vfprintf(this->file, fmt, arg);
	std::fflush(this->file);
}
