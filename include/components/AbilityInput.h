#pragma once
#include "ecs/Component.h"
#include "core/AbilityTypes.h"

#include <vector>

/// @brief 能力输入组件 - 存储玩家的能力激活请求
/// @details 该组件用于记录玩家希望激活的能力。可以通过系统处理这些输入来触发相应的能力效果。
/// 
/// 设计思路：
/// 1. 存储玩家想要激活的能力类型
/// 2. 作为输入系统和能力系统之间的桥梁
/// 
/// 为何这样做：
/// - 保持系统间解耦
/// - 支持多玩家输入
/// - 符合ECS数据驱动原则
struct AbilityInput : public Component
{
	std::vector<AbilityType> requestedAbilities; /// 存储玩家希望激活的能力类型列表

	/// @brief 添加一个能力请求到输入列表
	/// @details 将指定的能力类型添加到请求列表中，以便后续处理
	/// @param ability [IN] 要请求的能力类型
	void requestAbility(AbilityType ability)
	{
		requestedAbilities.push_back(ability);
	}

	/// @brief 清空所有能力请求
	/// @details 清除当前所有的能力请求，以便重新开始输入收集
	void clear()
	{
		requestedAbilities.clear();
	}
};