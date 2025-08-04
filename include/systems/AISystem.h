#pragma once
#include "ecs/System.h"
#include "components/AI.h"

// 前向声明
class Entity;

/// @brief AI系统 - 管理所有AI实体的行为
/// @details 该系统负责更新AI实体的状态和行为，包括空闲、巡逻、追逐和攻击等状态。
/// 
/// 设计思路：
/// 1. 每个AI实体都有一个AI组件，包含当前状态和相关参数。
/// 2. 系统在每帧更新时遍历所有AI实体，根据其状态调用相应的行为方法。
/// 3. 行为方法实现具体的逻辑，如移动、攻击等。
/// 
/// 为何这样做：
/// - 将AI逻辑集中在一个系统中，便于管理和扩展。
/// - 通过状态机模式实现不同状态的行为，增强代码可读性和可维护性。
/// - 支持多种AI行为，便于未来添加新功能或修改现有逻辑。
class AISystem : public System {
public:
    /// @brief 更新系统状态
    /// @details 每帧调用一次，处理符合条件的实体和组件
    /// @param world [IN] 当前游戏世界
    /// @param deltaTime [IN] 时间增量
    virtual void update(World& world, float deltaTime) override;

private:
    /// @brief 更新AI状态
	/// @details 根据实体的AI组件状态，调用相应的行为方法
	/// @param entity [IN] 当前实体
	/// @param player [IN] 玩家实体，用于追逐和攻击逻辑
	/// @param deltaTime [IN] 时间增量
    void updateAI(Entity* entity, Entity* player, float deltaTime);

    /// @brief 空闲状态行为
	/// @details 实体在空闲状态下的行为逻辑，如随机移动或等待
	/// @param entity [IN] 当前实体
	/// @param deltaTime [IN] 时间增量
    void idleBehavior(Entity* entity, float deltaTime);

    /// @brief 巡逻状态行为
	/// @details 实体在巡逻状态下的行为逻辑，如沿路径移动
	/// @param entity [IN] 当前实体
	/// @param deltaTime [IN] 时间增量
    void patrolBehavior(Entity* entity, float deltaTime);

    /// @brief 追逐状态行为
	/// @details 实体在追逐状态下的行为逻辑，如向玩家移动
	/// @param entity [IN] 当前实体
	/// @param player [IN] 玩家实体，用于追逐逻辑
	/// @param deltaTime [IN] 时间增量
    void chaseBehavior(Entity* entity, Entity* player, float deltaTime);

    /// @brief 攻击状态行为
	/// @details 实体在攻击状态下的行为逻辑，如执行攻击动作
	/// @param entity [IN] 当前实体
	/// @param player [IN] 玩家实体，用于攻击逻辑
	/// @param deltaTime [IN] 时间增量
    void attackBehavior(Entity* entity, Entity* player, float deltaTime);
};