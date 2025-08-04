#include "systems/EnvironmentSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Corruption.h"
#include "components/CorruptionSource.h"
#include "components/DarkTide.h"
#include "components/Transform.h"
#include "prefabs/EnemyPrefab.h"
#include "core/Logger.h"

EnvironmentSystem::EnvironmentSystem()
	: m_darkTideTimer(0.0f), m_darkTideInterval(120.0f)
{ }

void EnvironmentSystem::update(World& world, float deltaTime)
{
    // 更新暗蚀潮汐计时器
    m_darkTideTimer += deltaTime;
    if (m_darkTideTimer >= m_darkTideInterval) {
        spawnDarkTide(world);
        m_darkTideTimer = 0.0f;
        Logger::instance()->log("暗蚀潮汐爆发！腐蚀强度增加，敌人生成率提升");
    }

    for (auto& entity : world.getEntities()) {
		// 更新所有腐蚀源
        if (auto* source = entity->getComponent<CorruptionSource>()) {
            auto* transform = entity->getComponent<Transform>();
            if (!transform) continue;

            // 腐蚀源随时间增强
            source->power += deltaTime * 0.1f;

            // 影响范围内的实体（简化实现，实际应使用空间划分）
            for (auto& other : world.getEntities()) {
                if (other->getId() == entity->getId()) continue;

                auto* otherTransform = other->getComponent<Transform>();
                auto* corruption = other->getComponent<Corruption>();
                if (!otherTransform || !corruption) continue;

                float distance = glm::distance(transform->position, otherTransform->position);
                if (distance <= source->radius) {
                    // 距离越近影响越大（线性衰减）
                    float effect = source->power * (1.0f - distance / source->radius);
                    corruption->current += effect * deltaTime;
                }
            }
        }

        // 更新所有空间扭曲效果
        for (auto& entity : world.getEntities()) {
            if (auto* distortion = entity->getComponent<SpatialDistortion>()) {
                distortion->duration -= deltaTime;

                if (distortion->duration <= 0.0f) {
                    // 扭曲效果结束
                    entity->getWorld().markEntityForDestruction(*entity);
                    Logger::instance()->log("空间扭曲效果结束");
                }
            }
        }
    }
}

void EnvironmentSystem::spawnCorruptionSource(World& world, const glm::vec3& position, float power, float range) {
    auto& source = world.createEntity();
	source.addComponent<Transform>(position, glm::quat(1, 0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
    source.addComponent<CorruptionSource>(power, range);

    Logger::instance()->log("生成腐蚀源于位置: (" +
        std::to_string(position.x) + ", " +
        std::to_string(position.y) + ", " +
        std::to_string(position.z) + ")");
}

void EnvironmentSystem::spawnDarkTide(World& world, float duration, float powerMultiplier, float spawnRate) {
    auto& tide = world.createEntity();
    tide.addComponent<DarkTide>(duration, powerMultiplier, spawnRate);

    // 增强所有腐蚀源
    for (auto& entity : world.getEntities()) {
        if (auto* source = entity->getComponent<CorruptionSource>()) {
            source->power *= powerMultiplier;
        }
    }

    // 生成更多暗蚀生物
    for (int i = 0; i < 5; i++) {
        glm::vec3 position(rand() % 100 - 50, 0.0f, rand() % 100 - 50);
        Prefab::createDarkCreature(world, position);
    }

    Logger::instance()->log("暗蚀潮汐开始，持续: " + std::to_string(duration) + "秒");
}

void EnvironmentSystem::spawnSpatialDistortion(World& world, DistortionType type, const glm::vec3& position, float radius, float strength, float duration)
{
    auto& distortion = world.createEntity();
    auto& transform = distortion.addComponent<Transform>(position, glm::quat(1, 0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
    auto& distortionComp = distortion.addComponent<SpatialDistortion>();

    distortionComp.type = type;
    distortionComp.center = position;
    distortionComp.radius = radius;
    distortionComp.strength = strength;
    distortionComp.duration = duration;

    switch (type) {
    case DistortionType::GravityShift:
        distortionComp.gravityDirection = glm::vec3(0.0f, -1.0f, 0.0f); // 默认向下
        break;
    case DistortionType::TimeDilation:
        distortionComp.timeScale = 0.5f; // 时间减慢
        break;
    case DistortionType::SpatialRift:
        distortionComp.damagePerSecond = 10.0f;
        break;
    }

    Logger::instance()->log("生成空间扭曲: " + std::to_string(static_cast<int>(type)) +
        " 在位置: (" + std::to_string(position.x) + ", " +
        std::to_string(position.y) + ", " +
        std::to_string(position.z) + ")");
}