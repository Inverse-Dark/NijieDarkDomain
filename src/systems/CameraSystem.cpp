#include "systems/CameraSystem.h"
#include "core/InputMap.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Camera.h"
#include "components/Transform.h"
#include "components/Player.h"

CameraSystem::CameraSystem(InputMap* inputMap)
	: m_pInputMap(inputMap)
{
}

void CameraSystem::update(World& world, float deltaTime)
{
    // 查找玩家实体
    Entity* pPlayer = nullptr;
    for (auto& entity : world.getEntities()) {
        if (entity->getComponent<Player>()) {
            pPlayer = entity.get();
            break;
        }
    }

    if (!pPlayer) return;

    InputMap::MouseMove mouseMove = m_pInputMap->getMouseMove();
    m_pInputMap->resetMouseRelative();
	handleMouseMotion(pPlayer, mouseMove.xRel, mouseMove.yRel);

    auto* playerTransform = pPlayer->getComponent<Transform>();
    auto* camera = pPlayer->getComponent<Camera>();

    if (!playerTransform || !camera) return;

    // 计算水平旋转矩阵
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(camera->yaw), glm::vec3(0, 1, 0));
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(camera->pitch), glm::vec3(1, 0, 0));
    glm::vec3 rotatedOffset = rotY * rotX * glm::vec4(camera->offset - glm::vec3(0, 0, camera->distance), 1.0f);

    // 相机位置
    camera->position = playerTransform->position + rotatedOffset;

}

void CameraSystem::handleMouseMotion(Entity* entity, int xrel, int yrel)
{
    if (!entity) return;

	// 获取相机组件
	auto* camera = entity->getComponent<Camera>();
    
	if (!camera) return;

    // 更新相机旋转
    camera->yaw -= xrel * camera->sensitivity;
    camera->pitch += yrel * camera->sensitivity;

    // 限制俯仰角度
    camera->pitch = glm::clamp(camera->pitch,
        camera->minPitch,
        camera->maxPitch);

    // 归一化偏航角
    if (camera->yaw > 360.0f) camera->yaw -= 360.0f;
    if (camera->yaw < 0.0f) camera->yaw += 360.0f;
}