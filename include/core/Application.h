#pragma once
#include <SDL.h>
#include <glad/glad.h>
#include <string>
#include <memory>

#include "core/Timer.h"

// 前向声明
class InputMap;
class Logger;

/// @brief 应用程序类 - 管理整个游戏生命周期
/// @details 该类负责初始化SDL和OpenGL环境，处理事件循环，并在应用程序退出时清理资源。
/// 
/// 设计思路
/// 1. 封装SDL窗口创建和OpenGL上下文初始化
/// 2. 提供主循环框架
/// 3. 管理帧率和时间
/// 4. 作为所有子系统的入口点
/// 
/// 为何这样做：
/// - 集中管理游戏生命周期，避免全局变量
/// - 提供一致的初始化流程
/// - 简化主函数逻辑
class Application {
public:
	/// @brief 构造函数，初始化应用程序
	/// @details 该函数设置窗口标题、宽度和高度。
	/// @param title [IN] 窗口标题
	/// @param width [IN] 窗口宽度
	/// @param height [IN] 窗口高度
    Application(const std::string title, int width, int height);
	/// @brief 析构函数，清理资源
	/// @details 该函数销毁OpenGL上下文和SDL窗口，并清理SDL资源。
    ~Application();

	/// @brief 运行应用程序的主循环
	/// @details 该函数进入主循环，并在退出时清理资源。
    void run();

	/// @brief 初始化应用程序
	/// @details 该函数调用SDL和OpenGL的初始化函数，并设置应用程序状态为运行。
	bool initialize();

private:

	/// @brief 处理SDL事件
	/// @details 该函数处理SDL事件，如窗口关闭、键盘输入等。
	void processEvents();

	/// @brief 更新应用程序状态
	/// @details 该函数更新应用程序状态，处理游戏逻辑和输入。
	/// @param deltaTime [IN] 上一帧到当前帧的时间差
	void update(float deltaTime);

	/// @brief 渲染一帧
	/// @details 该函数清除屏幕并绘制内容，调用子系统的渲染函数。
	void render();

	/// @brief 清理资源
	/// @details 该函数销毁OpenGL上下文和SDL窗口，并清理SDL资源。
    void shutdown();
	/// @brief 初始化OpenGL状态
	/// @details 该函数设置OpenGL的视口、深度测试等状态。
    void initOpenGLState();

private:
	SDL_Window* m_pWindow;   // SDL窗口指针
	SDL_GLContext m_glContext;	// OpenGL上下文指针
	std::string m_windowTitle;	// 窗口标题
	int m_screenWidth;	// 窗口宽度
	int m_screenHeight;	// 窗口高度
	bool m_bIsRunning;	// 应用程序是否正在运行标志

	std::unique_ptr<InputMap> m_pInputMap;	// 输入映射
	
	Timer m_frameTimer;	// 帧率计时器
	Logger* m_pLogger;	// 日志记录器
};