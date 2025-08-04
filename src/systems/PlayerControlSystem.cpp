#include "systems/PlayerControlSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Player.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "components/AbilityInput.h"
#include "components/MovementProperties.h"

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

	if (!transform || !velocity || !movementProps) return;

	// 计算移动方向
	glm::vec3 moveDirection(0.0f);

	// 使用持续状态检测
	if (m_pInputMap->isActionHeld(InputMap::MoveForward))
		moveDirection += transform->forward;
	if (m_pInputMap->isActionHeld(InputMap::MoveBackward))
		moveDirection -= transform->forward;
	if (m_pInputMap->isActionHeld(InputMap::MoveRight))
		moveDirection += transform->right;
	if (m_pInputMap->isActionHeld(InputMap::MoveLeft))
		moveDirection -= transform->right;

	// 标准化方向并应用速度
	if (glm::length(moveDirection) > 0.0f) {
		moveDirection = glm::normalize(moveDirection);
		velocity->linear = moveDirection * movementProps->getEffectiveSpeed();
	}
	else {
		velocity->linear = glm::vec3(0.0f);
	}
}
