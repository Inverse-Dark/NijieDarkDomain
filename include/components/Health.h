#pragma once
#include "ecs/Component.h"

/// @brief 生命值组件 - 表示实体的生命值状态
/// @details 包含当前生命值、最大生命值和基础最大生命值（不受腐蚀影响）
/// 
/// 设计思路：
/// 1. 存储当前生命值和最大值
/// 2. 提供基础生命值和临时修改
/// 
/// 为何这样做：
/// - 统一管理生命值状态
/// - 支持腐蚀系统对生命值的修改
struct Health : public Component {
	float current;	// 当前生命值
	float max;	// 最大生命值
	float baseMax;	// 基础最大生命值（不受腐蚀影响）

	/// @brief 默认构造函数
	/// @details 初始化当前生命值和最大生命值为100.0f，基础最大生命值也为100.0f
	Health()
		: current(100.0f), max(100.0f), baseMax(100.0f)
	{}
	/// @brief 带参数的构造函数
	/// @details 用于初始化生命值状态
	/// @param current [IN] 当前生命值
	/// @param max [IN] 最大生命值
	/// @param baseMax [IN] 基础最大生命值（不受腐蚀影响）
	Health(float current, float max, float baseMax)
		: current(current), max(max), baseMax(baseMax) {
	}
};