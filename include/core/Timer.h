#pragma once
#include <chrono>

/// @brief 计时器类，用于测量代码执行时间
/// @details 该类使用高精度时钟来记录时间点，并提供测量代码执行时间的功能
/// 
/// 设计思路：
/// 1. 使用C++11 chrono库实现高精度计时
/// 2. 提供简单的开始/结束接口
/// 3. 计算帧时间（Delta Time）
/// 
/// 为何这样做：
/// - 精确控制游戏帧率
/// - 实现时间相关的游戏逻辑
/// - 独立于平台的时间计算
class Timer
{
public:
	/// @brief 构造函数，创建一个计时器
	/// @details 该函数会初始化计时器
	Timer();

	/// @brief 启动计时器
	/// @details 该函数会记录当前时间为计时器的开始时间
	void start();

	/// @brief 重新开始计时并返回经过的时间
	/// @return 返回从上次开始到现在的时间间隔
	float restart();

	/// @brief 更新计时器
	/// @details 该函数会计算从上次更新到现在的时间间隔，并更新计时器状态
	/// @param deltaTime [IN] 上次更新到现在的时间间隔
	void update(float deltaTime);

	/// @brief 获取经过的时间
	/// @return 返回从计时器开始到现在的时间间隔
	float elapsed() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;	// 开始时间点
};