#pragma once
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Enemy.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "components/Health.h"
#include "components/AI.h"
#include "components/Attack.h"
#include "components/CombatInput.h"
#include "components/MeshRenderer.h"
#include "components/MovementProperties.h"

/// @brief 预设体：敌人实体
/// @details 创建一个敌人实体，包含必要的组件和初始值设置。
/// 
/// 设计思路：
/// 1. 封装敌人实体的创建逻辑
/// 2. 初始化敌人组件
/// 3. 提供一致的敌人配置
/// 
/// 为何这样做：
/// - 简化敌人创建过程
/// - 集中管理敌人配置
/// - 支持多种敌人类型（未来扩展）
namespace Prefab {
    /// @brief 创建暗蚀生物实体
	/// @details 该函数创建一个暗蚀生物实体，并为其添加必要的组件和初始值设置。
	/// @param world [IN] 需要添加实体的世界对象
	/// @param position [IN] 实体的初始位置
	/// @return 返回创建的敌人实体
    inline Entity& createDarkCreature(World& world, const glm::vec3& position) {
        auto& enemy = world.createEntity();

        // 添加标记组件
        enemy.addComponent<Enemy>();

        // 添加变换组件
        auto& transform = enemy.addComponent<Transform>();
        transform.position = position;
        transform.updateDirectionVectors();

        // 添加速度组件
        enemy.addComponent<Velocity>();

        // 添加移动属性组件
        auto& movement = enemy.addComponent<MovementProperties>();
        movement.moveSpeed = 7.0f; // 最大速度

        // 添加AI组件
        auto& ai = enemy.addComponent<AI>();
        ai.sightRange = 15.0f;
        ai.chaseRange = 25.0f;
        ai.attackRange = 1.0f;
        ai.chaseSpeed = 1.0f;
        ai.patrolSpeed = 1.5f;

        // 设置巡逻路径
        ai.patrolPoints.push_back(position);
        ai.patrolPoints.push_back(position + glm::vec3(5.0f, 0.0f, 0.0f));
        ai.patrolPoints.push_back(position + glm::vec3(5.0f, 0.0f, 5.0f));

        // 添加攻击组件
        auto& attack = enemy.addComponent<Attack>();
        attack.damage = 15.0f;
        attack.range = 3.0f;
        attack.cooldown = 2.0f;

        // 添加攻击请求组件
        enemy.addComponent<CombatInput>();

        // 添加生命值组件
        auto& health = enemy.addComponent<Health>();
        health.current = 80.0f;
        health.max = 80.0f;
        health.baseMax = 80.0f;

        // 添加渲染组件（红色三角形）
        auto& renderer = enemy.addComponent<MeshRenderer>();
        renderer.color = glm::vec3(1.0f, 0.0f, 0.0f); // 红色
        // 创建敌人网格（金字塔形状）
        std::vector<Mesh::Vertex> enemyVertices = {
            // 底部
            {{-0.5f, 0.0f, -0.5f},	{0.0f, -1.0f, 0.0f},	{0.0f, 0.0f}, renderer.color},
            {{0.5f, 0.0f, -0.5f},	{0.0f, -1.0f, 0.0f},	{1.0f, 0.0f}, renderer.color},
            {{0.5f, 0.0f, 0.5f},	{0.0f, -1.0f, 0.0f},	{1.0f, 1.0f}, renderer.color},
            {{-0.5f, 0.0f, 0.5f},	{0.0f, -1.0f, 0.0f},	{0.0f, 1.0f}, renderer.color},

            // 侧面
            {{0.0f, 1.0f, 0.0f},	{0.0f, 0.0f, 1.0f},	{0.5f, 1.0f}, renderer.color},
            {{-0.5f, 0.0f, 0.5f},	{0.0f, 0.0f, 1.0f},	{0.0f, 0.0f}, renderer.color},
            {{0.5f, 0.0f, 0.5f},	{0.0f, 0.0f, 1.0f},	{1.0f, 0.0f}, renderer.color},

            {{0.0f, 1.0f, 0.0f},	{1.0f, 0.0f, 0.0f},	{0.5f, 1.0f}, renderer.color},
            {{0.5f, 0.0f, 0.5f},	{1.0f, 0.0f, 0.0f},	{0.0f, 0.0f}, renderer.color},
            {{0.5f, 0.0f, -0.5f},	{1.0f, 0.0f, 0.0f},	{1.0f, 0.0f}, renderer.color},

            {{0.0f, 1.0f, 0.0f},	{0.0f, 0.0f, -1.0f},	{0.5f, 1.0f}, renderer.color},
            {{0.5f, 0.0f, -0.5f},	{0.0f, 0.0f, -1.0f},	{0.0f, 0.0f}, renderer.color},
            {{-0.5f, 0.0f, -0.5f},	{0.0f, 0.0f, -1.0f},	{1.0f, 0.0f}, renderer.color},

            {{0.0f, 1.0f, 0.0f},	{-1.0f, 0.0f, 0.0f},	{0.5f, 1.0f}, renderer.color},
            {{-0.5f, 0.0f, -0.5f},	{-1.0f, 0.0f, 0.0f},	{0.0f, 0.0f}, renderer.color},
            {{-0.5f, 0.0f, 0.5f},	{-1.0f, 0.0f, 0.0f},	{1.0f, 0.0f}, renderer.color}
        };

        std::vector<unsigned int> enemyIndices = {
            // 底部
            0, 1, 2,
            0, 2, 3,

            // 侧面
            4, 5, 6,
            7, 8, 9,
            10, 11, 12,
            13, 14, 15
        };
        renderer.mesh = new Mesh(enemyVertices, enemyIndices);

        return enemy;
    }
}