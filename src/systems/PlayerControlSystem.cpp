#include "systems/PlayerControlSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Player.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "components/AbilityInput.h"
#include "components/Attack.h"
#include "components/CombatInput.h"
#include "components/MovementProperties.h"
#include "components/Camera.h"

#include "core/Logger.h"

PlayerControlSystem::PlayerControlSystem(InputMap* inputMap)
	: m_pInputMap(inputMap)
{
}

void PlayerControlSystem::update(World& world, float deltaTime)
{
	Entity* pPlayer = nullptr;
	for (auto& entity : world.getEntities())
	{
		if (entity->getComponent<Player>())
		{
			pPlayer = entity.get();
		}
	}

	if (!pPlayer || !m_pInputMap) return;

	checkAbility(pPlayer, InputMap::Ability1, AbilityType::Perception);
	checkAbility(pPlayer, InputMap::Ability2, AbilityType::Manipulation);
	checkAbility(pPlayer, InputMap::Ability3, AbilityType::Distortion);
	checkAbility(pPlayer, InputMap::Ability4, AbilityType::Assimilation);
	checkAbility(pPlayer, InputMap::Ability5, AbilityType::Purification);

	handleMovement(pPlayer, deltaTime);

	auto* attack = pPlayer->getComponent<Attack>();
	auto* transform = pPlayer->getComponent<Transform>();
	auto* combatInput = pPlayer->getComponent<CombatInput>();
	auto* camera = pPlayer->getComponent<Camera>();

	if (!attack || !transform || !combatInput || !camera) return;

	// 处理攻击输入
	if (m_pInputMap->isActionHeld(InputMap::AttackPrimary)) {
		// 玩家攻击逻辑
		if (attack) {
			if (attack->attackTimer.elapsed() >= attack->cooldown) {
				for (auto& entity : world.getEntities()) {
					if (entity.get() == pPlayer) continue; // 跳过自己

					if (auto* targetTransform = entity->getComponent<Transform>()) {
						// 计算距离和方向
						glm::vec3 toTarget = targetTransform->position - transform->position;
						float distance = glm::length(toTarget);

						if (distance <= attack->range) {
							// 计算角度
							glm::vec3 dir = glm::normalize(toTarget);
							float angle = glm::degrees(glm::acos(glm::dot(transform->forward, dir)));

							if (angle <= attack->angle) {
								// 应用伤害
								combatInput->requestCombat(entity.get());
							}
						}
					}
				}

				attack->attackTimer.restart(); // 重置攻击计时器
			}
		}
	}
}

void PlayerControlSystem::checkAbility(Entity* player, InputMap::Action action, AbilityType abilityType)
{
	if (!player) return; // 确保玩家实体已设置

	// 获取技能请求组件
	auto* abilityInput = player->getComponent<AbilityInput>();
	if (!abilityInput) return;

	if (m_pInputMap->isActionHeld(action))
	{
		// 如果技能尚未触发
		if (!abilityInput->isAbilityTriggered(abilityType))
		{
			abilityInput->requestAbility(abilityType);
			abilityInput->setAbilityTriggered(abilityType, true);
		}
	}
	else
	{
		// 按键释放时重置状态
		abilityInput->setAbilityTriggered(abilityType, false);
	}
}

void PlayerControlSystem::handleMovement(Entity* player, float deltaTime)
{
	if (!player) return; // 确保玩家实体已设置

	auto* transform = player->getComponent<Transform>();
	auto* velocity = player->getComponent<Velocity>();
	auto* movementProps = player->getComponent<MovementProperties>();
	auto* camera = player->getComponent<Camera>();

	if (!transform || !velocity || !movementProps || !camera) return;

	// 计算移动方向
	glm::vec3 moveDirection(0.0f);
	glm::vec3 moveForward = glm::vec3(
		-sin(glm::radians(-camera->yaw)),
		0,
		cos(glm::radians(-camera->yaw))
	);
	glm::vec3 moveRight = glm::normalize(glm::cross(moveForward, glm::vec3(0, 1, 0)));

	// 使用持续状态检测
	if (m_pInputMap->isActionHeld(InputMap::MoveForward))
		moveDirection += moveForward;
	if (m_pInputMap->isActionHeld(InputMap::MoveBackward))
		moveDirection -= moveForward;
	if (m_pInputMap->isActionHeld(InputMap::MoveRight))
		moveDirection += moveRight;
	if (m_pInputMap->isActionHeld(InputMap::MoveLeft))
		moveDirection -= moveRight;

	// 标准化方向并应用速度
	if (glm::length(moveDirection) > 0.01f) {
		moveDirection = glm::normalize(moveDirection);
		velocity->linear = moveDirection * movementProps->getEffectiveSpeed();

		glm::quat targetRot = glm::quatLookAt(moveDirection, glm::vec3(0, 1, 0));
		const float rotSpeed = movementProps->getEffectiveSpeed() * deltaTime;
		transform->rotation = glm::slerp(transform->rotation, targetRot, glm::clamp(rotSpeed, 0.0f, 1.0f));
	}
	else {
		velocity->linear = glm::vec3(0.0f);
	}
}
