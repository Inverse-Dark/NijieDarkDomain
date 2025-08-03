#include "systems/PlayerControlSystem.h"
#include "core/InputMap.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Player.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "components/AbilityInput.h"
#include "components/MovementProperties.h"

PlayerControlSystem::PlayerControlSystem(InputMap* inputMap)
	: m_pInputMap(inputMap),
	m_pPlayerEntity(nullptr)
{
	m_pInputMap->addActionListener(InputMap::Ability1, [this]() {
		requestAbility(AbilityType::Perception);
		}, InputMap::Pressed);

	inputMap->addActionListener(InputMap::Ability2, [this]() {
		requestAbility(AbilityType::Manipulation);
		}, InputMap::Pressed);

	inputMap->addActionListener(InputMap::Ability3, [this]() {
		requestAbility(AbilityType::Distortion);
		}, InputMap::Pressed);

	inputMap->addActionListener(InputMap::Ability4, [this]() {
		requestAbility(AbilityType::Assimilation);
		}, InputMap::Pressed);

	inputMap->addActionListener(InputMap::Ability5, [this]() {
		requestAbility(AbilityType::Purification);
		}, InputMap::Pressed);
}

void PlayerControlSystem::update(World& world, float deltaTime)
{
	if (!m_pPlayerEntity)
	{
		for (auto& entity : world.getEntities()) {
			if (entity->getComponent<Player>()) {
				m_pPlayerEntity = entity.get();
			}
		}
	}

	m_pInputMap->update();

	handleMovement(deltaTime);
}

void PlayerControlSystem::requestAbility(AbilityType type)
{
	if (m_pPlayerEntity) {
		auto* abilityInput = m_pPlayerEntity->getComponent<AbilityInput>();
		if (abilityInput) {
			abilityInput->requestAbility(type);
		}
	}
}

void PlayerControlSystem::handleMovement(float deltaTime)
{
	if (!m_pPlayerEntity) return; // 确保玩家实体已设置

	auto* transform = m_pPlayerEntity->getComponent<Transform>();
	auto* velocity = m_pPlayerEntity->getComponent<Velocity>();
	auto* movementProps = m_pPlayerEntity->getComponent<MovementProperties>();

	if (!transform || !velocity) return;

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
		velocity->linear = moveDirection * movementProps->getEffectiveSpeed() * deltaTime;
	}
	else {
		velocity->linear = glm::vec3(0.0f);
	}
}
