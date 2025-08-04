#pragma once
#include "ecs/Component.h"

/// @brief 暗蚀潮汐组件 - 表示周期性爆发的暗蚀能量潮汐
/// @details 该组件用于标记暗蚀潮汐事件，增强所有腐蚀源
/// 
/// 设计思路：
/// 1. 周期性全局环境事件
/// 2. 增强所有腐蚀源的强度
/// 3. 生成更多暗蚀生物
/// 
/// 为何这样做：
/// - 增加游戏动态挑战性
/// - 创造周期性压力事件
struct DarkTide : public Component {
    float duration; // 持续时间
    float powerMultiplier; // 腐蚀强度倍增器
    float spawnRateMultiplier; // 敌人生成率倍增器

	/// @brief 默认构造函数
	/// @details 初始化默认值
    DarkTide()
        : duration(30.0f), powerMultiplier(1.5f), spawnRateMultiplier(2.0f)
    {
	}
	/// @brief 带参数的构造函数
	/// @details 用于自定义暗蚀潮汐事件
	/// @param duration [IN] 持续时间
	/// @param multiplier [IN] 腐蚀强度倍增器
	/// @param spawnRate [IN] 敌人生成率倍增器
    DarkTide(float duration, float multiplier, float spawnRate)
        : duration(duration), powerMultiplier(multiplier), spawnRateMultiplier(spawnRate) {
    }
};