#pragma once
#include "ecs/System.h"

// 前向声明
class Entity;

/// @brief 战斗系统 - 处理实体之间的战斗逻辑
/// @details 该系统负责处理实体之间的攻击、伤害计算和区域伤害等逻辑。
/// 
/// 设计思路：
/// 1. 实体之间的攻击通过调用 `applyDamage` 方法实现，计算伤害并应用到目标实体上。
/// 2. 区域伤害通过 `applyAreaDamage` 方法实现，影响指定半径内的所有实体。
/// 3. 系统在每帧更新时检查实体状态，处理战斗相关的逻辑。
/// 
/// 为何这样做：
/// - 将战斗逻辑集中在一个系统中，便于管理和扩展。
/// - 使用方法分离攻击和区域伤害逻辑，提高代码的可读性和可维护性。
class CombatSystem : public System {
public:
	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
    void update(World& world, float deltaTime) override;

private:
	/// @brief 应用伤害到目标实体
	/// @details 计算伤害并更新目标实体的生命值
	/// @param attacker [IN] 攻击者实体
	/// @param target [IN] 目标实体
	/// @param damage [IN] 伤害值
    void applyDamage(Entity* attacker, Entity* target, float damage);
	/// @brief 应用区域伤害到指定半径内的所有实体
	/// @details 计算区域内所有实体的伤害并更新其生命值
	/// @param source [IN] 伤害源实体
	/// @param radius [IN] 伤害半径
	/// @param damage [IN] 伤害值
    void applyAreaDamage(Entity* source, float radius, float damage);
};