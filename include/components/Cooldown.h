#pragma once
#include "ecs/Component.h"
#include "core/AbilityTypes.h"

#include <unordered_map>

/// @brief 冷却时间组件 - 管理能力的冷却状态
/// @details 该组件存储每个能力的冷却时间，允许检查和更新冷却状态。
/// 
/// 设计思路：
/// 1. 存储各能力的剩余冷却时间
/// 2. 提供冷却状态查询
/// 
/// 为何这样做：
/// - 防止能力滥用
/// - 增加策略性
struct Cooldown : public Component
{
	std::unordered_map<AbilityType, float> cooldowns;	// 能力冷却时间映射

	/// @brief 设置指定能力的冷却时间
	/// @details 设置指定能力的冷却时间，单位为秒。
	/// @param type [IN] 能力类型
	/// @param duration [IN] 冷却持续时间
	void setCooldown(AbilityType type, float duration)
	{
		cooldowns[type] = duration;
	}

	/// @brief 更新冷却时间
	/// @details 每帧调用，减少各能力的冷却时间。
	/// @param deltaTime [IN] 时间增量，单位为秒
	void update(float deltaTime)
	{
		for (auto& pair : cooldowns) {
			if (pair.second > 0) {
				pair.second -= deltaTime;
			}
		}
	}

	/// @brief 检查指定能力是否在冷却中
	/// @details 检查指定能力的冷却时间是否大于0。
	/// @param type [IN] 能力类型
	/// @return 如果在冷却中返回true，否则返回false
	bool isOnCooldown(AbilityType type) const
	{
		auto it = cooldowns.find(type);
		return it != cooldowns.end() && it->second > 0;
	}
};