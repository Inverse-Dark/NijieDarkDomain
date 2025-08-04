#pragma once
#include "ecs/System.h"
#include "core/AbilityTypes.h"
#include "core/InputMap.h"
#include "components/Player.h"
#include "components/Transform.h"
#include "components/Velocity.h"

#include <SDL.h>

// 前向声明
class Entity;

/// @brief 玩家控制系统 - 处理玩家输入并控制角色
/// @details 该系统监听玩家输入并更新玩家实体的位置和状态。
/// 
/// 设计思路：
/// 1. 响应键盘输入
/// 2. 控制玩家实体的移动
/// 3. 触发角色能力
/// 
/// 为何这样做：
/// - 集中处理玩家输入
/// - 分离输入逻辑与移动逻辑
class PlayerControlSystem : public System
{
public:
	/// @brief 构造函数
	/// @details 初始化移动状态和速度
	/// @param inputMap [IN] 输入映射，用于处理键盘输入
	PlayerControlSystem(InputMap* inputMap);

	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
	virtual void update(World& world, float deltaTime) override;
private:
	/// @brief 请求角色能力
	/// @details 根据输入触发角色的能力，并判断释放触发过，以避免每帧都请求一次
	/// @param player [IN] 玩家实体
	/// @param action [IN] 绑定的按键映射
	/// @param abilityType [IN] 技能类型
	void checkAbility(Entity* player, InputMap::Action action, AbilityType abilityType);
	/// @brief 处理玩家移动
	/// @details 根据输入更新玩家实体的位置
	/// @param player [IN] 玩家实体
	/// @param deltaTime [IN] 时间增量
	void handleMovement(Entity* player, float deltaTime);
private:
	InputMap* m_pInputMap;	// 输入映射，用于处理玩家输入
};