#pragma once
#include "ecs/Component.h"
#include "core/Timer.h"

#include <vector>
#include <glm/glm.hpp>

/// @brief AI状态枚举
/// @details 定义AI的不同状态
enum class AIState {
	Idle,       // 空闲
	Patrol,     // 巡逻
	Chase,      // 追逐
	Attack      // 攻击
};

/// @brief AI组件 - 赋予实体智能行为
/// @details 该组件用于控制非玩家实体的行为逻辑
/// 
/// 设计思路：
/// 1. 定义AI状态机，包含空闲、巡逻、追逐和攻击四种状态。
/// 2. 每个状态有不同的行为逻辑和持续时间。
/// 3. AI感知范围和移动速度参数可调节，适应不同场景需求。
/// 4. 巡逻路径由多个点组成，AI在巡逻状态下按顺序访问这些点。
/// 
/// 为何这样做：
/// - 通过状态机管理AI行为，使其更易于扩展和维护。
/// - 使用感知范围和移动速度参数，使AI行为更具灵活性和可调节性。
/// - 巡逻路径的设计使AI在巡逻状态下更自然，避免重复路径导致的行为单一。
struct AI : public Component {
	AIState state;  // 当前状态
	Timer stateTimer;        // 当前状态持续时间

	// 感知参数
	float sightRange;       // 视野范围
	float chaseRange;       // 最大追逐距离
	float attackRange;       // 攻击范围

	// 移动参数
	float patrolSpeed;       // 巡逻速度
	float chaseSpeed;        // 追逐速度

	// 状态持续时间
	float idleDuration;      // 空闲状态持续时间
	float patrolDuration;    // 巡逻状态持续时间

	// 巡逻路径
	std::vector<glm::vec3> patrolPoints;	// 巡逻点列表
	int currentPatrolIndex;	// 当前巡逻点索引

	/// @brief 构造函数，初始化AI组件
	/// @details 设置初始状态和参数
	AI()
		: state(AIState::Idle),
		sightRange(10.0f), chaseRange(15.0f), attackRange(2.0f),
		chaseSpeed(4.0f), patrolSpeed(2.0f),
		idleDuration(3.0f), patrolDuration(5.0f),
		currentPatrolIndex(0)
	{
	}
};