#pragma once
#include "ecs/Component.h"

#include <vector>

// 前向声明
class Entity;

/// @brief 作战输入组件 - 存储玩家的攻击请求
/// @details 该组件用于记录实体希望攻击的对象。可以通过系统处理这些输入来触发相应的战斗效果。
/// 
/// 设计思路：
/// 1. 存储实体希望攻击的对象
/// 2. 作为输入系统和战斗系统之间的桥梁
/// 
/// 为何这样做：
/// - 保持系统间解耦
/// - 支持多玩家输入
/// - 符合ECS数据驱动原则
struct CombatInput : public Component
{
	std::vector<Entity*> requestedCombat; // 存储被攻击实体

	/// @brief 添加一个攻击请求到输入列表
	/// @details 将被攻击实体添加到请求列表中，以便后续处理
	/// @param target [IN] 被攻击实体
	void requestCombat(Entity* target)
	{
		requestedCombat.push_back(target);
	}

	/// @brief 清空所有攻击请求
	/// @details 清除当前所有的攻击请求，以便重新开始输入收集
	void clear()
	{
		requestedCombat.clear();
	}
};