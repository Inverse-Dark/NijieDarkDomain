#pragma once
#include "ecs/Component.h"

#include <glm/glm.hpp>

/// @brief 速度组件 - 表示实体的运动状态
/// @details 包含线速度和角速度，用于描述实体在空间中的运动。
/// 
/// 设计思路：
/// 1. 存储线速度和角速度
/// 2. 提供简单的运动控制基础
/// 
/// 为何这样做：
/// - 分离运动状态与变换
/// - 为物理系统提供基础
/// - 支持多种运动类型（玩家、NPC、抛射体等）
struct Velocity : public Component
{
	glm::vec3 linear;  // 线速度
	glm::vec3 angular; // 角速度
	/// @brief 默认构造函数
	/// @details 初始化线速度和角速度为零向量
	Velocity()
		: linear(glm::vec3(0.0f)), angular(glm::vec3(0.0f)) 
	{}
	/// @brief 带参数的构造函数
	/// @details 用于初始化速度组件
	/// @param linear [IN] 线速度向量
	/// @param angular [IN] 角速度向量
	Velocity(const glm::vec3& linear, const glm::vec3& angular)
		: linear(linear), angular(angular) {}
};