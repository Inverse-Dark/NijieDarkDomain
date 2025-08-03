#include "systems/AbilitySystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/DarkEnergy.h"
#include "components/Corruption.h"
#include "components/Transform.h"
#include "components/AbilityInput.h"
#include "components/Cooldown.h"
#include "core/Logger.h"

/// @brief 能力消耗配置
/// @details 每种能力的消耗量
const std::unordered_map<AbilityType, float> ABILITY_COST = {
	{AbilityType::Perception, 10.0f},
	{AbilityType::Manipulation, 20.0f},
	{AbilityType::Distortion, 30.0f},
	{AbilityType::Assimilation, 40.0f},
	{AbilityType::Purification, 50.0f}
};

/// @brief 能力腐蚀增量系数
/// @details 每使用一次能力，增加的腐蚀度
const float CORRUPTION_PER_ENERGY = 0.1f;

void AbilitySystem::update(World& world, float deltaTime)
{
	for (auto& entity : world.getEntities()) {
		// 更新所有实体的冷却时间
		auto* cooldown = entity->getComponent<Cooldown>();
		if (cooldown) {
			cooldown->update(deltaTime);
		}

		auto* abilityInput = entity->getComponent<AbilityInput>();
		if (!abilityInput || abilityInput->requestedAbilities.empty()) {
			continue;
		}

		// 处理所有请求的能力
		for (AbilityType type : abilityInput->requestedAbilities) {
			// 检查冷却状态
			if (cooldown && cooldown->isOnCooldown(type))
				continue;

			if (activateAbility(entity.get(), type))
			{
				if (cooldown)
					cooldown->setCooldown(type, getCooldownDuration(type));
			}
		}

		// 清除已处理的请求
		abilityInput->clear();
	}
}

bool AbilitySystem::activateAbility(Entity* entity, AbilityType type) {
	if(!entity) {
		Logger::instance()->log("无法激活能力，实体为空", Logger::LogLevel::ERROR);
		return false;
	}

	auto* energy = entity->getComponent<DarkEnergy>();
	auto* corruption = entity->getComponent<Corruption>();

	if (!energy || !corruption) {
		Logger::instance()->log("实体缺少暗能量或腐蚀度组件", Logger::LogLevel::WARN);
		return false;
	}

	// 检查能量是否足够
	float cost = ABILITY_COST.at(type);

	if (energy->current < cost) {
		Logger::instance()->log("暗能量不足，无法使用能力", Logger::LogLevel::INFO);
		return false;
	}

	// 消耗能量
	energy->current -= cost;

	// 是否应用能力成功
	bool isSuccess = false;

	// 应用能力效果
	switch (type) {
	case AbilityType::Perception: isSuccess = applyPerception(entity); break;
	case AbilityType::Manipulation: isSuccess = applyManipulation(entity); break;
	case AbilityType::Distortion: isSuccess = applyDistortion(entity); break;
	case AbilityType::Assimilation: isSuccess = applyAssimilation(entity); break;
	case AbilityType::Purification: isSuccess = applyPurification(entity); break;
	}

	if (isSuccess)
	{
		// 增加腐蚀度
		float corruptionIncrease = cost * CORRUPTION_PER_ENERGY;
		corruption->current += corruptionIncrease;

		Logger::instance()->log("能力激活: " + std::to_string(static_cast<int>(type)) +
			" 消耗: " + std::to_string(cost) +
			" 腐蚀度增加：" + std::to_string(corruptionIncrease) +
			" 当前腐蚀度: " + std::to_string(corruption->current));
	}
	else
	{
		Logger::instance()->log("能力激活失败: " + abilityTypeToString(type), Logger::LogLevel::ERROR);	
	}

	return isSuccess;
}

bool AbilitySystem::applyPerception(Entity* entity)
{
	Logger::instance()->log("感知能力激活，显示隐藏元素");
	return true; // 假设感知能力总是成功
}

bool AbilitySystem::applyDistortion(Entity* entity)
{
	auto* transform = entity->getComponent<Transform>();
	if (!transform) return false;

	// 反转重力方向
	transform->gravityDirection *= -1.0f;

	Logger::instance()->log("重力扭曲激活，新重力方向: (" +
		std::to_string(transform->gravityDirection.x) + ", " +
		std::to_string(transform->gravityDirection.y) + ", " +
		std::to_string(transform->gravityDirection.z) + ")");

	// 增加额外腐蚀度作为代价
	if (auto* corruption = entity->getComponent<Corruption>()) {
		corruption->current += 5.0f;
	}
	return true;
}

bool AbilitySystem::applyManipulation(Entity* entity)
{
	Logger::instance()->log("操控能力激活，移动物体");
	return true; // 假设操控能力总是成功
}

bool AbilitySystem::applyPurification(Entity* entity) {
	auto* corruption = entity->getComponent<Corruption>();
	if (!corruption) return false;

	// 减少腐蚀度
	corruption->current -= 15.0f;
	if (corruption->current < 0.0f) corruption->current = 0.0f;

	Logger::instance()->log("净化能力激活，当前腐蚀度: " + std::to_string(corruption->current));
	return true;
}

bool AbilitySystem::applyAssimilation(Entity* entity) {
	Logger::instance()->log("同化能力激活，控制敌人");
	return true; // 假设同化能力总是成功
}

std::string AbilitySystem::abilityTypeToString(AbilityType type) const
{
	switch (type) {
	case AbilityType::Perception: return "感知";
	case AbilityType::Manipulation: return "操控";
	case AbilityType::Distortion: return "扭曲";
	case AbilityType::Assimilation: return "同化";
	case AbilityType::Purification: return "净化";
	default: return "未知";
	}
}

float AbilitySystem::getCooldownDuration(AbilityType type) const
{
	switch (type) {
	case AbilityType::Perception: return 2.0f;
	case AbilityType::Manipulation: return 3.0f;
	case AbilityType::Distortion: return 5.0f;
	case AbilityType::Assimilation: return 8.0f;
	case AbilityType::Purification: return 10.0f;
	default: return 0.0f;
	}
}