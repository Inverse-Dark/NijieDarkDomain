#include "core/Logger.h"

Logger* Logger::s_pInstance = nullptr;

Logger::Logger() {
	// 默认不写入文件
}

Logger::~Logger()
{
	// 如果日志文件被打开了，那么关闭它
	if (m_logFile.is_open())
		m_logFile.close();
}

Logger* Logger::instance()
{
	if (nullptr == s_pInstance)
		s_pInstance = new Logger;
	return s_pInstance;
}

void Logger::destroy()
{
	if (s_pInstance)
		delete s_pInstance;
}

void Logger::setLogFile(const std::string& filename)
{
	// 如果日志文件已经打开过了，先关闭之前的
	if (m_logFile.is_open())
		m_logFile.close();

	m_logFile.open(filename, std::ios::out | std::ios::app);

	if(!m_logFile)
		std::cerr << "无法打开日志文件: " << filename << std::endl;
}

void Logger::log(const std::string& message, const LogLevel& level)
{
	std::string logMsg = "[" + getCurrentTime() + "] [" + getLogLevel(level) + "] " + message;
	std::cout << logMsg << std::endl;
	if (m_logFile.is_open())
		m_logFile << logMsg << std::endl;
}

std::string Logger::getCurrentTime(const char* format) const {
	std::time_t now = std::time(nullptr);
	char buf[80];
	std::strftime(buf, sizeof(buf), format, std::localtime(&now));
	return std::string(buf);
}

std::string Logger::getLogLevel(const LogLevel& level) const
{
	switch (level)
	{
	case LogLevel::FATAL:
		return "FATAL";
	case LogLevel::ERROR:
		return "ERROR";
	case LogLevel::WARN:
		return "WARN";
	case LogLevel::INFO:
		return "INFO";
	case LogLevel::DEBUG:
		return "DEBUG";
	default:
		return "UNKNOW";
	}
}