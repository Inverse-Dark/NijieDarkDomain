#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

/// @brief 日志记录器
/// @details 该类用于记录日志信息到文件中。
/// 
/// 设计思路：
/// 1. 提供统一的日志接口
/// 2. 支持控制台和文件输出
/// 3. 添加时间戳和日志级别
/// 
/// 为何这样做：
/// - 便于调试和错误追踪
/// - 记录游戏运行状态
/// - 可扩展为更高级的日志系统
class Logger
{
public:
	/// @brief 日志级别
	/// @details 定义了不同的日志级别，便于过滤和分类日志信息。
	enum class LogLevel
	{
		FATAL,   // 致命错误
		ERROR,   // 错误
		WARN,    // 警告
		INFO,    // 信息
		DEBUG,   // 调试信息
	};

public:
	/// @brief 获取日志记录器实例
	/// @details 使用单例模式，确保全局只有一个日志记录器实例。
	static Logger* instance();
	/// @brief 销毁日志记录器实例
	/// @details 使用单例模式，确保全局只有一个日志记录器实例。
	static void destroy();

	/// @brief 记录日志信息
	/// @details 记录日志信息到控制台和文件中，包含时间戳和日志级别。
	/// @param message [IN] 日志信息内容
	/// @param level [IN] 日志级别
	void log(const std::string& message, const LogLevel& level = LogLevel::INFO);

	/// @brief 设置日志文件
	/// @details 设置日志输出到指定文件，默认输出到控制台。
	/// @param filename [IN] 日志文件名
	void setLogFile(const std::string& filename);
private:
	/// @brief 私有构造函数
	/// @details 防止外部实例化，确保单例模式。
	Logger();
	/// @brief 私有析构函数
	/// @details 确保日志文件正确关闭。
	~Logger();

	/// @brief 获取当前时间字符串
	/// @details 获取格式化的当前时间字符串，用于日志时间戳。
	/// @param format [IN] 时间格式，默认"%Y-%m-%d %H:%M:%S"
	/// @return 格式化的时间字符串
	std::string getCurrentTime(const char* format = "%Y-%m-%d %H:%M:%S") const;

	/// @brief 获取日志级别字符串
	/// @details 将日志级别枚举转换为字符串，便于输出。
	/// @param level [IN] 日志级别
	/// @return 日志级别字符串
	std::string getLogLevel(const LogLevel& level) const;

private:
	static Logger* s_pInstance; // 单例实例

	std::ofstream m_logFile; // 日志文件流
};