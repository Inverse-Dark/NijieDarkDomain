#pragma once
#include "ecs/System.h"
#include "components/SpatialDistortion.h"

#include <glm/glm.hpp>

/// @brief 环境系统 - 管理游戏世界的环境事件和效果
/// @details 该系统负责处理腐蚀源和暗蚀潮汐等环境机制
/// 
/// 设计思路：
/// 1. 通过定时器触发暗蚀潮汐事件，增加游戏的挑战性和紧迫感。
/// 2. 允许玩家在特定位置生成腐蚀源，影响周围环境和生物。
/// 3. 使用实体组件系统（ECS）架构，便于扩展和维护。
/// 
/// 为何这样做：
/// - 环境事件可以增加游戏的动态性和不可预测性，提升玩家体验。
/// - 通过ECS架构，可以轻松添加新环境效果或修改现有逻辑，而无需重构整个系统。
class EnvironmentSystem : public System {
public:
	/// @brief 构造函数
	/// @details 初始化环境系统，设置必要的参数和状态
    EnvironmentSystem();

    /// @brief 更新系统状态
    /// @details 每帧调用一次，处理符合条件的实体和组件
    /// @param world [IN] 当前游戏世界
    /// @param deltaTime [IN] 时间增量
    virtual void update(World& world, float deltaTime) override;

    /// @brief 生成腐蚀源实体
	/// @details 该方法在指定位置生成一个腐蚀源实体，影响周围环境和生物。
    /// @param world [IN] 游戏世界
    /// @param position [IN] 生成位置
    /// @param power [IN] 初始腐蚀强度
	/// @param range [IN] 腐蚀范围
    void spawnCorruptionSource(World& world, const glm::vec3& position, float power, float range);

    /// @brief 触发暗蚀潮汐事件
	/// @details 该方法在游戏世界中触发暗蚀潮汐事件，影响所有生物和环境。
    /// @param world [IN] 游戏世界
	/// @param duration [IN] 暗蚀潮汐持续时间（秒）
	/// @param powerMultiplier [IN] 暗蚀潮汐的强度倍率
	/// @param spawnRate [IN] 生物生成速率（每秒生成多少生物）
    void spawnDarkTide(World& world, float duration = 30.0f, float powerMultiplier = 1.5f, float spawnRate = 2.0f);

	/// @brief 生成空间扭曲效果
	/// @details 该方法在指定位置生成空间扭曲效果，影响周围实体的运动和行为。
	/// @param world [IN] 游戏世界
	/// @param type [IN] 扭曲类型
	/// @param position [IN] 生成位置
	/// @param radius [IN] 扭曲半径
	/// @param strength [IN] 扭曲强度
	/// @param duration [IN] 扭曲持续时间（秒）
    void spawnSpatialDistortion(World& world, DistortionType type, const glm::vec3& position,
        float radius, float strength, float duration);

private:
    float m_darkTideTimer;               // 暗蚀潮汐计时器
    const float m_darkTideInterval;     // 暗蚀潮汐间隔时间（秒）
};