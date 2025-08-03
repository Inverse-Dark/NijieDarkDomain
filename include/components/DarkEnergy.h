#pragma once
#include "ecs/Component.h"

/// @brief 暗能量组件 - 表示实体的暗能量状态
/// @details 包含当前暗能量值、最大暗能量值和每秒恢复量
/// 
/// 设计思路：
/// 1. 存储当前和最大暗能量值
/// 2. 提供能量恢复速率
/// 
/// 为何这样做：
/// - 管理界痕能力的使用资源
/// - 为能力系统提供消耗机制
/// - 支持能量恢复机制
struct DarkEnergy : public Component {
	float current; // 当前暗能量值
	float max; // 最大暗能量值
	float recoveryRate; // 每秒恢复量
	float baseRecoveryRate;	// 基础恢复量
	/// @brief 默认构造函数
	/// @details 初始化默认值
	DarkEnergy()
		: current(100.0f), max(100.0f), recoveryRate(5.0f), baseRecoveryRate(5.0f)
	{}
	/// @brief 带参数的构造函数
	/// @details 用于初始化暗能量组件
	/// @param initialEnergy [IN] 初始暗能量值
	/// @param max [IN] 最大暗能量值
	/// @param rate [IN] 每秒恢复量
	/// @param baseRate [IN] 基础恢复量
	DarkEnergy(float initialEnergy, float max, float rate, float baseRate)
		: current(initialEnergy), max(max), recoveryRate(rate), baseRecoveryRate(baseRate)
	{}
};