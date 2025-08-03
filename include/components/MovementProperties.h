#pragma once
#include "ecs/Component.h"

/// @brief 移动属性组件 - 存储实体的移动相关属性
/// @details 包含基础移动速度、旋转速度、加速度、减速度和速度乘数等属性，用于控制实体的移动行为
/// 
/// 设计思路：
/// 1. 集中管理所有移动相关属性
/// 2. 提供默认值
/// 3. 支持运行时修改
/// 
/// 为何这样做：
/// - 分离数据与逻辑
/// - 便于平衡调整
/// - 支持不同实体有不同的移动速度
struct MovementProperties : public Component
{
	float moveSpeed;        // 基础移动速度
	float rotationSpeed;  // 旋转速度（度/秒）
	float acceleration;    // 加速度
	float deceleration;    // 减速度
	float speedMultiplier;  // 移动速度乘数（用于技能效果）

	/// @brief 默认构造函数，初始化移动属性
	/// @details 默认移动速度为5.0f，旋转速度为180.0f，加速度为10.0f，减速度为15.0f，速度乘数为1.0f
	MovementProperties()
		: moveSpeed(5.0f), rotationSpeed(180.0f), acceleration(10.0f), deceleration(15.0f), speedMultiplier(1.0f)
	{
	}
	/// @brief 带参数的构造函数，用于自定义移动属性
	/// @details 默认移动速度为5.0f，旋转速度为180.0f，加速度为10.0f，减速度为15.0f，速度乘数为1.0f
	/// @param moveSpeed [IN] 基础移动速度
	/// @param rotationSpeed [IN] 旋转速度（度/秒）
	/// @param acceleration [IN] 加速度
	/// @param deceleration [IN] 减速度
	/// @param speedMultiplier [IN] 移动速度乘数（用于技能效果）
	MovementProperties(float moveSpeed, float rotationSpeed, float acceleration, float deceleration, float speedMultiplier)
		: moveSpeed(moveSpeed), rotationSpeed(rotationSpeed), acceleration(acceleration), deceleration(deceleration), speedMultiplier(speedMultiplier)
	{
	}
	/// @brief 获取当前有效的移动速度
	/// @details 有效移动速度等于基础移动速度乘以速度乘数
	/// @return 当前有效的移动速度，等于基础移动速度乘以速度乘数
	float getEffectiveSpeed() const {
		return moveSpeed * speedMultiplier;
	}
};