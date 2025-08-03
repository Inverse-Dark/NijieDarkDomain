#include "systems/CorruptionSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Corruption.h"
#include "components/DarkEnergy.h"
#include "components/Health.h"
#include "core/Logger.h"

/// @brief 腐蚀系统更新间隔时间
/// @details 此常量定义了腐蚀系统的更新频率，单位为秒。
const float EFFECT_INTERVAL = 1.0f;

void CorruptionSystem::update(World& world, float deltaTime) {
	for (auto& entity : world.getEntities()) {
		if (auto* corruption = entity->getComponent<Corruption>()) {
			// 更新阶段并检查变化
			if (corruption->updateStage()) {
				onStageChanged(entity.get(), corruption->stage);
			}

			if(corruption->effectTimer.elapsed() >= EFFECT_INTERVAL) {
				corruption->effectTimer.restart(); // 重置计时器
				// 更新腐蚀效果
				updateCorruptionEffects(entity.get(), deltaTime);
			}
		}
	}
}

void CorruptionSystem::onStageChanged(Entity* entity, Corruption::Stage newStage)
{
	auto* corruption = entity->getComponent<Corruption>();
	if (!corruption) return;

	std::string stageName;
	switch (newStage) {
	case Corruption::None: stageName = "无腐蚀"; break;
	case Corruption::Low: stageName = "低腐蚀"; break;
	case Corruption::Medium: stageName = "中腐蚀"; break;
	case Corruption::High: stageName = "高腐蚀"; break;
	case Corruption::Critical: stageName = "临界腐蚀"; break;
	}

	Logger::instance()->log("实体 " + std::to_string(entity->getId()) + " 进入 " + stageName + " 阶段", Logger::LogLevel::INFO);
}

void CorruptionSystem::updateCorruptionEffects(Entity* entity, float deltaTime) {
	auto* corruption = entity->getComponent<Corruption>();

	// 根据腐蚀度应用不同效果
	switch (corruption->getStage()) {
	case Corruption::Low:
		applyLowCorruptionEffects(entity);
		break;
	case Corruption::Medium:
		applyMediumCorruptionEffects(entity);
		break;
	case Corruption::High:
		applyHighCorruptionEffects(entity);
		break;
	case Corruption::Critical:
		applyCriticalCorruptionEffects(entity);
		break;
	default:
		// 无腐蚀阶段，不应用任何效果
		break;
	}
}

void CorruptionSystem::applyLowCorruptionEffects(Entity* entity) {
	auto* corruption = entity->getComponent<Corruption>();

	// 低腐蚀效果：轻微视觉扭曲
	// 在实际游戏中，这里会设置着色器参数
}

void CorruptionSystem::applyMediumCorruptionEffects(Entity* entity) {
	auto* corruption = entity->getComponent<Corruption>();
	auto* health = entity->getComponent<Health>();
	auto* energy = entity->getComponent<DarkEnergy>();

	// 检查组件是否存在
	if (!health || !energy) return;

	// 效果1：随机能力失控
	static float effectTimer = 0.0f;
	effectTimer += 0.1f; // 简化计时

	if (effectTimer > 5.0f) { // 每5秒触发一次
		// 在实际游戏中，这里会随机触发一个能力
		Logger::instance()->log("腐蚀效应：能力失控", Logger::LogLevel::WARN);
		effectTimer = 0.0f;
	}

	// 效果2：生命值上限降低（最多降低10%）
	float maxReduction = 0.1f;
	float reductionRatio = (corruption->current - corruption->lowThreshold) /
		(corruption->mediumThreshold - corruption->lowThreshold);
	health->max = health->baseMax * (1.0f - maxReduction * reductionRatio);

	// 如果当前生命值超过上限，调整到上限
	if (health->current > health->max) {
		health->current = health->max;
	}

	// 效果3：能量恢复速率降低（最多降低30%）
	float recoveryReduction = 0.3f * reductionRatio;
	energy->recoveryRate = energy->baseRecoveryRate * (1.0f - recoveryReduction);
}

void CorruptionSystem::applyHighCorruptionEffects(Entity* entity) {
	auto* corruption = entity->getComponent<Corruption>();
	auto* health = entity->getComponent<Health>();
	auto* energy = entity->getComponent<DarkEnergy>();

	// 检查组件是否存在
	if (!health || !energy) return;

	// 效果1：生命值快速减少
	float healthReduction = health->baseMax * 0.01f; // 每秒减少1%的最大生命值
	health->current -= healthReduction;

	// 生命值上限降低（最多降低30%）
	float maxReduction = 0.3f;
	float reductionRatio = (corruption->current - corruption->mediumThreshold) /
		(corruption->highThreshold - corruption->mediumThreshold);
	health->max = health->baseMax * (1.0f - maxReduction * reductionRatio);

	// 如果当前生命值超过上限，调整到上限
	if (health->current > health->max) {
		health->current = health->max;
	}

	// 效果2：能量恢复速率降低（最多降低30%）
	float recoveryReduction = 0.3f * reductionRatio;
	energy->recoveryRate = energy->baseRecoveryRate * (1.0f - recoveryReduction);

	// 效果3：视野严重扭曲
	// 在实际游戏中，这里会有强烈的视觉效果

}

void CorruptionSystem::applyCriticalCorruptionEffects(Entity* entity)
{
	auto* corruption = entity->getComponent<Corruption>();
	auto* health = entity->getComponent<Health>();
	auto* energy = entity->getComponent<DarkEnergy>();

	// 检查组件是否存在
	if (!health || !energy) return;

	// 效果1：生命值快速减少
	float healthReduction = health->baseMax * 0.1f; // 每秒减少10%的最大生命值
	health->current -= healthReduction;

	// 生命值上限降低（最多降低30%）
	float maxReduction = 0.3f;
	float reductionRatio = (corruption->current - corruption->mediumThreshold) /
		(corruption->highThreshold - corruption->mediumThreshold);
	health->max = health->baseMax * (1.0f - maxReduction * reductionRatio);

	// 如果当前生命值超过上限，调整到上限
	if (health->current > health->max) {
		health->current = health->max;
	}

	// 效果3：能量恢复速率降低（最多降低30%）
	float recoveryReduction = 0.3f * reductionRatio;
	energy->recoveryRate = energy->baseRecoveryRate * (1.0f - recoveryReduction);

	// 效果3：极端视觉扭曲和音效
	// 在实际游戏中，这里会有极端的视觉和音效效果
	
	// 效果4：随机能力暴走（属性增强、但会有负面效果）

}
