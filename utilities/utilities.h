#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <chrono>
#include <exception>
#include <sstream>
#include <ctime>
#include <sstream>
#include <unordered_map>

namespace utilities
{
    // Ref: Victor Gordan
    // 12th - July - 2023
    // https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%204%20-%20Organizing/shaderClass.cpp
    inline std::string readGLSL(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (file)
        {
            std::string contents;
            file.seekg(0, std::ios::end);
            contents.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(&contents[0], contents.size());
            file.close();
            return (contents);
        }
        throw(errno);
    }
};

namespace Debug {
	enum LOG_TYPE
	{
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};
	class Exception : public std::runtime_error
	{
	public:
		// Warning: Message should length <= 40 because i use a lot for colorlizing the buffer string
		explicit Exception(LOG_TYPE displayType, char const * const message) throw() : std::runtime_error(message)
		{
			_type = displayType;
		}

		// Warning: Message should length <= 40 because i use a lot for colorlizing the buffer string
		explicit Exception(LOG_TYPE displayType, std::string const &message) throw() : std::runtime_error(message)
		{
			_type = displayType;
		}

		virtual char const* what() const throw()
		{
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			std::stringstream res;
			res << "\033[1;32m["
				<< now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " "
				<< now->tm_mday << "-" << (now->tm_mon + 1) << "-" << (now->tm_year + 1900)
				<< "] ";
			switch (_type)
			{
			case LOG_TYPE::INFO:
				res << "\033[1;36m[INFO]\033[0;0m: " << std::exception::what();
				break;
			case LOG_TYPE::WARNING:
				res << "\033[1;33m[WARNING]\033[0;0m: " << std::exception::what();
				break;
			case LOG_TYPE::ERROR:
				res << "\033[1;31m[ERROR]\033[0;0m: " << std::exception::what();
				break;
			case LOG_TYPE::CRITICAL:
				res << "\033[1;31m[CRITICAL]\033[0;0m: " << std::exception::what();
				break;
			}
			res << "\033[0;0m";
			return res.str().c_str();
		}

		static inline void log(LOG_TYPE displayType, std::string const &message, const char* file, const std::size_t line) {
			std::time_t t = std::time(0);
			std::tm* now = std::localtime(&t);
			std::stringstream res;
			res << "\033[1;32m["
				<< now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " "
				<< now->tm_mday << "-" << (now->tm_mon + 1) << "-" << (now->tm_year + 1900)
				<< "]";
			switch (displayType)
			{
			case INFO:
				res << "\033[1;36m [INFO]\033[0;0m: " << message;
				break;
			case WARNING:
				res << "\033[1;33m [WARNING]\033[0;0m: " << message;
				break;
			case ERROR:
				res << "\033[1;31m [ERROR]\033[0;0m: " << message;
				break;
			case CRITICAL:
				res << "\033[1;31m [CRITICAL]\033[0;0m: " << message;
				break;
			}
			if (displayType != LOG_TYPE::INFO) {
				res << std::endl << " __LINE__ " << line
					<< std::endl << " __FILE__ " << file << "\033[0;0m";
			}
			std::cerr << res.str() << std::endl;
		}
	private:
		LOG_TYPE _type = LOG_TYPE::INFO;
	};
};

#define AppLog(message) Debug::Exception::log(Debug::LOG_TYPE::INFO, message, __FILE__, __LINE__)
