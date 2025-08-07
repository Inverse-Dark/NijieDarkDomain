#include "systems/AISystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/AI.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "components/Player.h"
#include "components/Health.h"
#include "components/Attack.h"
#include "components/CombatInput.h"
#include "components/MovementProperties.h"
#include "core/Logger.h"

void AISystem::update(World& world, float deltaTime)
{
	Entity* pPlayer = nullptr;
	for (auto& entity : world.getEntities())
	{
		if (entity->getComponent<Player>())
		{
			pPlayer = entity.get();
			break;
		}
	}

	// 更新所有AI实体
	for (auto& entity : world.getEntities()) {
		if (entity->getComponent<AI>()) {
			updateAI(entity.get(), pPlayer, deltaTime);
		}
	}
}

void AISystem::updateAI(Entity* entity, Entity* player, float deltaTime)
{
	if (!entity) return;

	auto* ai = entity->getComponent<AI>();
	auto* transform = entity->getComponent<Transform>();
	auto* velocity = entity->getComponent<Velocity>();
	Transform* playerTransform = nullptr;
	float distance = 0.0f;


	if (!ai || !transform || !velocity) return;

	// 状态机更新
	switch (ai->state) {
	case AIState::Idle:
		idleBehavior(entity, deltaTime);

		if (!player) return;
		playerTransform = player->getComponent<Transform>();
		if (!playerTransform) break;
		// 计算与玩家的距离
		distance = glm::distance(transform->position, playerTransform->position);

		// 如果看到玩家，转为追击
		if (distance <= ai->sightRange) {
			ai->state = AIState::Chase;
			ai->stateTimer.restart();
			Logger::instance()->log("敌人发现玩家，开始追击");
		}
		break;

	case AIState::Patrol:
		patrolBehavior(entity, deltaTime);

		if (!player) return;
		playerTransform = player->getComponent<Transform>();
		if (!playerTransform) break;
		// 计算与玩家的距离
		distance = glm::distance(transform->position, playerTransform->position);

		// 如果看到玩家，转为追击
		if (distance <= ai->sightRange) {
			ai->state = AIState::Chase;
			ai->stateTimer.restart();
			Logger::instance()->log("敌人发现玩家，开始追击");
		}
		break;

	case AIState::Chase:
		chaseBehavior(entity, player, deltaTime);
		break;

	case AIState::Attack:
		attackBehavior(entity, player, deltaTime);
		break;
	}
}

void AISystem::idleBehavior(Entity* entity, float deltaTime)
{
	if (!entity) return;

	auto* ai = entity->getComponent<AI>();
	auto* velocity = entity->getComponent<Velocity>();
	if (!ai || !velocity) return;

	// 停止移动
	velocity->linear = glm::vec3(0.0f);

	// 闲置时间结束后转为巡逻
	if (ai->stateTimer.elapsed() >= ai->idleDuration) {
		ai->state = AIState::Patrol;
		ai->stateTimer.restart();
		Logger::instance()->log("敌人开始巡逻");
	}
}

void AISystem::patrolBehavior(Entity* entity, float deltaTime)
{
	if (!entity) return;

	auto* ai = entity->getComponent<AI>();
	auto* transform = entity->getComponent<Transform>();
	auto* velocity = entity->getComponent<Velocity>();
	auto* movementProperties = entity->getComponent<MovementProperties>();
	if (!ai || !transform || !velocity || !movementProperties || ai->patrolPoints.empty()) return;

	// 获取当前目标点
	glm::vec3 target = ai->patrolPoints[ai->currentPatrolIndex];
	float distance = glm::distance(transform->position, target);

	if (distance < 0.5f) {
		// 到达目标点，选择下一个
		ai->currentPatrolIndex = (ai->currentPatrolIndex + 1) % ai->patrolPoints.size();
		target = ai->patrolPoints[ai->currentPatrolIndex];
	}

	// 计算移动方向
	glm::vec3 direction = glm::normalize(target - transform->position);
	movementProperties->moveSpeed = ai->patrolSpeed; // 使用巡逻速度
	velocity->linear = direction * movementProperties->getEffectiveSpeed();

	// 更新朝向（仅Y轴旋转）
	transform->rotation = glm::quatLookAt(
		glm::normalize(glm::vec3(direction.x, 0.0f, direction.z)),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	// 巡逻时间结束转空闲
	if (ai->stateTimer.elapsed() >= ai->patrolDuration) {
		ai->state = AIState::Idle;
		ai->stateTimer.restart();
	}
}

void AISystem::chaseBehavior(Entity* entity, Entity* player, float deltaTime)
{
	if (!entity) return;

	auto* ai = entity->getComponent<AI>();

	if (!player)
	{
		if(ai)	// 玩家不存在，转回空闲状态
		{
			ai->state = AIState::Idle;
			ai->stateTimer.restart();
		}
		return;
	}

	auto* transform = entity->getComponent<Transform>();
	auto* velocity = entity->getComponent<Velocity>();
	auto* movementProperties = entity->getComponent<MovementProperties>();
	auto* playerTransform = player->getComponent<Transform>();
	if (!ai || !transform || !velocity || !movementProperties || !playerTransform) return;

	// 计算到玩家的方向
	glm::vec3 direction = playerTransform->position - transform->position;
	float distance = glm::length(direction);

	if (distance > ai->attackRange) {
		direction = glm::normalize(direction);

		// 应用移动速度
		movementProperties->moveSpeed = ai->chaseSpeed; // 使用追击速度
		velocity->linear = direction * movementProperties->getEffectiveSpeed();

		// 更新朝向
		glm::quat targetRot = glm::quatLookAt(direction, glm::vec3(0, 1, 0));
		const float rotSpeed = movementProperties->getEffectiveSpeed() * deltaTime;
		transform->rotation = glm::slerp(transform->rotation, targetRot, glm::clamp(rotSpeed, 0.0f, 1.0f));
	}

	// 如果接近玩家，切换到攻击状态
	if (distance < ai->attackRange) {
		ai->state = AIState::Attack;
		ai->stateTimer.restart();
		Logger::instance()->log("敌人开始攻击");
	}

	// 如果玩家太远，返回巡逻状态
	if (distance > ai->chaseRange) {
		ai->state = AIState::Patrol;
		ai->stateTimer.restart();
		Logger::instance()->log("玩家超出追击范围，敌人放弃");
	}
}

void AISystem::attackBehavior(Entity* entity, Entity* player, float deltaTime)
{
	if (!entity) return;

	auto* ai = entity->getComponent<AI>();

	if (!player)
	{
		if (ai)	// 玩家不存在，转回空闲状态
		{
			ai->state = AIState::Idle;
			ai->stateTimer.restart();
		}
		return;
	}

	auto* transform = entity->getComponent<Transform>();
	auto* velocity = entity->getComponent<Velocity>();
	auto* attack = entity->getComponent<Attack>();
	auto* combat = entity->getComponent<CombatInput>();
	auto* playerTransform = player->getComponent<Transform>();
	auto* playerHealth = player->getComponent<Health>();
	auto* movementProperties = entity->getComponent<MovementProperties>();

	if (!ai || !transform || !velocity || !attack || !combat || !playerTransform || !playerHealth || !movementProperties) return;

	// 停止移动
	velocity->linear = glm::vec3(0.0f);

	// 超出攻击范围转追逐
	float distance = glm::distance(transform->position, playerTransform->position);
	if (distance > attack->range) {
		ai->state = AIState::Chase;
		return;
	}

	// 攻击冷却结束，执行攻击
	if (attack->attackTimer.elapsed() >= attack->cooldown) {
		// 计算距离和方向
		glm::vec3 toTarget = playerTransform->position - transform->position;
		float distance = glm::length(toTarget);
		if (distance <= attack->range)
		{
			// 计算角度
			glm::vec3 dir = glm::normalize(toTarget);
			float angle = glm::degrees(glm::acos(glm::dot(transform->forward, dir)));

			if (angle <= attack->angle)
			{
				// 应用伤害
				combat->requestCombat(player);
				attack->attackTimer.restart();
			}
			else
			{
				glm::quat targetRot = glm::quatLookAt(dir, glm::vec3(0, 1, 0));
				const float rotSpeed = movementProperties->getEffectiveSpeed() * deltaTime;
				transform->rotation = glm::slerp(transform->rotation, targetRot, glm::clamp(rotSpeed, 0.0f, 1.0f));
			}
		}
	}

}