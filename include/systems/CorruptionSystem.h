#pragma once
#include "ecs/System.h"
#include "components/Corruption.h"

// 前向声明
class Entity;

/// @brief 腐蚀系统 - 管理腐蚀度的影响和效果
/// @details 该系统处理实体的腐蚀度变化，应用相应的效果，并可能触发其他事件或状态变化。
/// 
/// 设计思路：
/// 1. 更新腐蚀效果
/// 2. 应用腐蚀阶段变化
/// 3. 处理腐蚀相关事件
/// 
/// 为何这样做：
/// - 实现游戏核心风险机制
/// - 统一管理腐蚀效果逻辑
/// - 为视觉效果提供依据
class CorruptionSystem : public System
{
public:
	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
	virtual void update(World& world, float deltaTime);

private:
	/// @brief 处理实体的腐蚀阶段变化
	/// @details 根据实体的腐蚀度变化，打印日志
	/// @param entity [IN] 系统中的实体
	/// @param newStage [IN] 新的腐蚀阶段
	void onStageChanged(Entity* entity, Corruption::Stage newStage);

	/// @brief 更新腐蚀效果
	/// @details 处理实体的腐蚀度变化和效果应用
	/// @param entity [IN] 系统中的实体
	/// @param deltaTime [IN] 时间增量
	void updateCorruptionEffects(Entity* entity, float deltaTime);
	/// @brief 应用低腐蚀阶段变化
	/// @details 处理低腐蚀阶段的效果应用
	/// @param entity [IN] 系统中的实体
	void applyLowCorruptionEffects(Entity* entity);
	/// @brief 应用中腐蚀阶段变化
	/// @details 处理中腐蚀阶段的效果应用
	/// @param entity [IN] 系统中的实体
	void applyMediumCorruptionEffects(Entity* entity);
	/// @brief 应用高腐蚀阶段变化
	/// @details 处理高腐蚀阶段的效果应用
	/// @param entity [IN] 系统中的实体
	void applyHighCorruptionEffects(Entity* entity);
	/// @brief 处理危险腐蚀阶段变化
	/// @details 处理危险腐蚀阶段的效果应用
	/// @param entity [IN] 系统中的实体
	void applyCriticalCorruptionEffects(Entity* entity);

};