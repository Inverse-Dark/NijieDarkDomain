#include "systems/MovementSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Transform.h"
#include "components/Velocity.h"

void MovementSystem::update(World& world, float deltaTime) {
	for (auto& entity : world.getEntities()) {
		auto* transform = entity->getComponent<Transform>();
		auto* velocity = entity->getComponent<Velocity>();

		if (transform && velocity) {
			// 更新位置
			transform->position += velocity->linear * deltaTime;

			// 更新旋转（四元数旋转）
			if (glm::length(velocity->angular) > 0.0f) {
				float angle = glm::length(velocity->angular) * deltaTime;
				glm::quat rot = glm::angleAxis(angle, glm::normalize(velocity->angular));
				transform->rotation = rot * transform->rotation;
			}
		}
	}
}