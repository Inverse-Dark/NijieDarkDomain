#pragma once
#include "ecs/Component.h"
#include "core/Timer.h"

struct Corruption : public Component
{
	/// @brief 腐蚀阶段
	/// @details 用于标记当前腐蚀效果的阶段
	enum Stage
	{
		None,     // 无腐蚀效果
		Low,      // 低腐蚀效果
		Medium,   // 中腐蚀效果
		High,      // 高腐蚀效果
		Critical  // 危险腐蚀效果
	};

	float current; // 当前腐蚀度
	float lowThreshold; // 低腐蚀度阈值
	float mediumThreshold; // 中腐蚀度阈值
	float highThreshold;  // 高腐蚀阶段阈值

	Stage stage; // 当前腐蚀阶段
	Stage lastStage; // 上次腐蚀阶段（用于判断阶段变化）
	Timer effectTimer; // 效果触发计时器

	/// @brief 默认构造函数
	/// @details 初始化腐蚀度为0，阈值为默认值
	Corruption()
		: current(0.0f), lowThreshold(30.0f), mediumThreshold(70.0f), highThreshold(90.0f)
	{
	}
	/// @brief 带参数的构造函数
	/// @details 初始化腐蚀度和阈值，并更新效果标志
	/// @param initial 初始腐蚀度
	/// @param low 低腐蚀度阈值
	/// @param medium 中腐蚀度阈值
	/// @param high 高腐蚀度阈值
	Corruption(float initial, float low, float medium, float high)
		: current(initial), lowThreshold(low), mediumThreshold(medium), highThreshold(high)
	{
	}

	/// @brief 获取腐蚀阶段
	/// @details 根据当前腐蚀度获取当前腐蚀阶段
	/// @return 当前腐蚀阶段
	Stage getStage() const {
		if (current > 0.0f && current < lowThreshold) return Low;
		if (current > lowThreshold && current < mediumThreshold) return Medium;
		if (current > mediumThreshold && current < highThreshold) return High;
		if (current > highThreshold) return Critical;
		return None;
	}

	/// @brief 检查腐蚀阶段是否发生变化
	/// @details 比较当前阶段和上次阶段，如果不同则返回 true
	/// @return 是否发生变化
	bool stageChanged() const {
		return stage != lastStage;
	}

	/// @brief 更新腐蚀阶段并返回是否发生变化
	/// @details 更新当前阶段和上次阶段，并返回是否发生变化
	/// @return 是否发生变化
	bool updateStage() {
		lastStage = stage;
		stage = getStage();
		return stageChanged();
	}
};