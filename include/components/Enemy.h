#pragma once
#include "ecs/Component.h"

/// @brief 敌人标记组件
/// @details 这是一个空的标记组件，用于标识敌人实体。
/// 
/// 设计思路：
/// 1. 用于标记敌人实体
/// 2. 不包含数据，仅作为标识符
/// 
/// 为何这样做：
/// - 快速识别敌人实体
/// - 避免在多个系统中重复检查
class Enemy : public Component
{
	// 空组件，仅作为标记
};