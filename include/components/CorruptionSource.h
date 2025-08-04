#pragma once
#include "ecs/Component.h"
#include "core/Timer.h"

/// @brief 腐蚀源组件 - 表示环境中的暗蚀能量源头
/// @details 该组件用于标记环境中的腐蚀源实体，具有腐蚀强度和影响范围
/// 
/// 设计思路：
/// 1. 表示环境中持续释放暗蚀能量的源头
/// 2. 随时间增强腐蚀强度
/// 3. 影响范围内实体的腐蚀度
/// 
/// 为何这样做：
/// - 实现动态环境威胁
/// - 为腐蚀系统提供环境来源
struct CorruptionSource : public Component {
    float power;    // 腐蚀强度
    float radius;     // 影响范围（单位）
    Timer pulseTimer; // 脉冲计时器

	/// @brief 默认构造函数
	/// @details 初始化默认腐蚀强度和范围
    CorruptionSource()
        : power(20.0f), radius(8.0f)
    {
	}
	/// @brief 带参数的构造函数
	/// @details 用于创建具有特定腐蚀强度和范围的腐蚀源
	/// @param power [IN] 腐蚀强度
	/// @param radius [IN] 影响范围
    CorruptionSource(float power, float radius)
        : power(power), radius(radius) {
    }
};