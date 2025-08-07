#pragma once
#include "ecs/Component.h"

#include <glm/glm.hpp>

/// @brief 定义空间扭曲类型
/// @details 该组件用于描述游戏世界中的空间扭曲效果，如重力偏移、时间膨胀和空间裂隙等。
enum class DistortionType {
    GravityShift, // 重力偏移
    TimeDilation, // 时间膨胀
    SpatialRift   // 空间裂隙
};

/// @brief 空间扭曲组件
/// @details 该组件用于描述游戏世界中的空间扭曲效果。
/// 
/// 设计思路：
/// 1. 使用枚举类型 DistortionType 来定义不同的扭曲类型。
/// 2. 使用 glm::vec3 来表示扭曲中心位置，便于在三维空间中定位。
/// 3. 使用 float 类型来表示扭曲影响半径、强度和持续时间等参数。
/// 4. 添加特定参数以支持不同类型的空间扭曲效果：
/// 
/// 为何这样做：
/// - 使用枚举类型可以清晰地表示不同的扭曲类型，便于扩展和维护。
/// - 使用 glm::vec3 可以方便地处理三维空间中的位置和方向。
/// - 使用 float 类型可以精确控制扭曲的影响范围和强度。
/// - 添加特定参数可以使组件更具灵活性和可配置性，支持不同的游戏机制和效果。
struct SpatialDistortion : public Component {
	DistortionType type;    // 扭曲类型
	glm::vec3 center;   // 扭曲中心位置
	float radius;   // 扭曲影响半径
	float strength; // 扭曲强度
	float duration; // 扭曲持续时间

    glm::vec3 gravityDirection; // 重力偏移特定参数

    float timeScale;    // 时间膨胀特定参数

    float damagePerSecond;  // 空间裂隙特定参数

	/// @brief 默认构造函数
	/// @details 初始化扭曲类型为重力偏移，中心位置为原点，半径、强度和持续时间为默认值。
	SpatialDistortion()
		: type(DistortionType::GravityShift)
		, center(0.0f, 0.0f, 0.0f)
		, radius(5.0f)
		, strength(1.0f)
		, duration(10.0f)
		, gravityDirection(0.0f, -1.0f, 0.0f) // 默认重力方向向下
		, timeScale(1.0f) // 默认时间缩放为1
		, damagePerSecond(10.0f) // 默认空间裂隙每秒伤害
	{}
};