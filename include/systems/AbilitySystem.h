#pragma once
#include "ecs/System.h"
#include "core/AbilityTypes.h"
#include "components/DarkEnergy.h"
#include "components/Corruption.h"
#include "components/Transform.h"

// 前向声明
class Entity;

/// @brief 能力系统 - 管理界痕能力的激活和效果
/// @details 处理能力的激活、效果应用和冷却逻辑
/// 
/// 设计思路：
/// 1. 处理能力的激活请求
/// 2. 消耗暗能量并应用能力效果
/// 3. 增加腐蚀度作为风险代价
/// 
/// 为何这样做：
/// - 实现游戏核心玩法机制
/// - 统一管理能力逻辑
/// - 平衡能力收益与风险
class AbilitySystem : public System
{
public:
    /// @brief 更新系统状态
    /// @details 每帧调用一次，处理符合条件的实体和组件
    /// @param world [IN] 当前游戏世界
    /// @param deltaTime [IN] 时间增量
    virtual void update(World& world, float deltaTime) override;

	/// @brief 激活指定类型的能力
	/// @details 检查条件并应用能力效果
	/// @param entity [IN] 实体
	/// @param type [IN] 能力类型
	/// @return 是否成功激活能力
	bool activateAbility(Entity* entity, AbilityType type);

private:
	/// @brief 应用感知能力效果
	/// @details 显示隐藏元素并消耗暗能量
	/// @param entity [IN] 实体
	/// @return 是否成功应用效果
	bool applyPerception(Entity* entity);
	/// @brief 应用操控能力效果
	/// @details 移动物体并消耗暗能量
	/// @param entity [IN] 实体
	/// @return 是否成功应用效果
	bool applyManipulation(Entity* entity);
	/// @brief 应用扭曲能力效果
	/// @details 改变重力并消耗暗能量
	/// @param entity [IN] 实体
	/// @return 是否成功应用效果
	bool applyDistortion(Entity* entity);
	/// @brief 应用同化能力效果
	/// @details 控制敌人并消耗暗能量
	/// @param entity [IN] 实体
	/// @return 是否成功应用效果
	bool applyAssimilation(Entity* entity);
	/// @brief 应用净化能力效果
	/// @details 减少腐蚀并消耗暗能量
	/// @param entity [IN] 实体
	/// @return 是否成功应用效果
	bool applyPurification(Entity* entity);
	/// @brief 将能力类型转换为字符串
	/// @details 用于调试和日志输出
	/// @param type [IN] 能力类型
	/// @return 能力类型字符串表示
	std::string abilityTypeToString(AbilityType type) const;
	/// @brief 获取能力冷却时间
	/// @details 根据能力类型返回冷却时间
	/// @param type [IN] 能力类型
	/// @return 冷却时间（秒）
	float getCooldownDuration(AbilityType type) const;
};