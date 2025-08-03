#pragma once
#include "ecs/System.h"

/// @brief 移动系统 - 根据速度更新位置
/// @details 该系统处理所有具有位置和速度组件的实体，更新它们的位置
/// 
/// 设计思路：
/// 1. 遍历所有拥有Transform和Velocity组件的实体
/// 2. 根据速度和时间更新位置
/// 
/// 为何这样做：
/// - 分离运动计算逻辑
/// - 支持物理和控制的统一处理
class MovementSystem : public System
{
public:
	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
	virtual void update(World& world, float deltaTime) override;
};