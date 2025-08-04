#include "systems/CombatSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Health.h"
#include "components/Attack.h"
#include "components/CombatInput.h"
#include "components/Transform.h"
#include "components/SpatialDistortion.h"
#include "core/Logger.h"

#include <glm/glm.hpp>

void CombatSystem::update(World& world, float deltaTime) {
	// 处理所有攻击
	for (auto& attacker : world.getEntities()) {
		if (!attacker) continue; // 跳过无效实体

		auto* combatComp = attacker->getComponent<CombatInput>();
		auto* attackComp = attacker->getComponent<Attack>();
		if (!combatComp || !attackComp) continue;

		for (auto* target : combatComp->requestedCombat)
		{
			if (!target) continue;	// 如果目标是无效实体则跳过

			applyDamage(attacker.get(), target, attackComp->damage);
		}

		combatComp->clear();	// 清空攻击请求

		// 如果攻击冷却结束，检查攻击范围内的目标
		//if (attackComp->attackTimer.elapsed() >= attackComp->cooldown) {
		//	for (auto& target : world.getEntities()) {
		//		if (target.get() == attacker.get() || !target) continue; // 如果目标是自己或无效实体则跳过
		//		auto* health = target->getComponent<Health>();
		//		auto* attackerTransform = attacker->getComponent<Transform>();
		//		auto* targetTransform = target->getComponent<Transform>();
		//		if (!health || !attackerTransform || !targetTransform) continue;
		//		// 检查距离
		//		float distance = glm::distance(attackerTransform->position, targetTransform->position);
		//		if (distance <= attackComp->range) {
		//			applyDamage(attacker.get(), world, target.get(), attackComp->damage);
		//			attackComp->attackTimer.restart();
		//		}
		//	}
		//}
	}

	// 处理空间扭曲造成的伤害
	for (auto& entity : world.getEntities()) {
		if (auto* distortion = entity->getComponent<SpatialDistortion>()) {
			if (distortion->type == DistortionType::SpatialRift) {
				applyAreaDamage(entity.get(), distortion->radius, distortion->damagePerSecond * deltaTime);
			}
		}
	}
}

void CombatSystem::applyDamage(Entity* attacker, Entity* target, float damage) {
	if (!attacker || !target) return;

	auto* health = target->getComponent<Health>();
	if (!health) return;

	health->current -= damage;

	Logger::instance()->log("实体受到伤害: " + std::to_string(damage) +
		" 剩余生命: " + std::to_string(health->current));
	if (health->current <= 0.0f) {
		health->current = 0.0f;
		Logger::instance()->log("实体被击败");
		target->getWorld().markEntityForDestruction(*target);
	}
}

void CombatSystem::applyAreaDamage(Entity* source, float radius, float damage) {
	if (!source) return;

	auto* sourceTransform = source->getComponent<Transform>();
	if (!sourceTransform) return;

	// 在实际游戏中，这里会使用空间分区数据结构优化
	for (auto& entity : source->getWorld().getEntities()) {
		if (entity.get() == source) continue; // 跳过自己

		auto* health = entity->getComponent<Health>();
		auto* transform = entity->getComponent<Transform>();

		if (!health || !transform) continue;

		float distance = glm::distance(sourceTransform->position, transform->position);
		if (distance <= radius) {
			health->current -= damage;
			Logger::instance()->log("空间扭曲造成伤害: " + std::to_string(damage));
		}

		if (health->current <= 0.0f) {
			health->current = 0.0f;
			Logger::instance()->log("实体被击败");
			entity->getWorld().markEntityForDestruction(*entity);
			// 在实际游戏中，这里会触发死亡动画和实体移除
		}
	}
}