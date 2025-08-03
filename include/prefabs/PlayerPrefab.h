#pragma once
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "components/Player.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "components/MovementProperties.h"
#include "components/DarkEnergy.h"
#include "components/Corruption.h"
#include "components/MeshRenderer.h"
#include "components/AbilityInput.h"
#include "components/Cooldown.h"

/// @brief 预设体：玩家实体
/// @details 创建一个玩家实体，包含必要的组件和初始值设置。
/// 
/// 设计思路：
/// 1. 封装玩家实体的创建逻辑
/// 2. 初始化玩家组件
/// 3. 提供一致的玩家配置
/// 
/// 为何这样做：
/// - 简化玩家创建过程
/// - 集中管理玩家配置
/// - 支持多种玩家类型（未来扩展）
namespace Prefab
{
    inline Entity& createPlayer(World& world) {
        auto& player = world.createEntity();

        // 添加标记组件
        player.addComponent<Player>();

        // 添加变换组件
        auto& transform = player.addComponent<Transform>();
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.updateDirectionVectors();

        // 添加速度组件
        player.addComponent<Velocity>();

        // 添加移动属性组件
		auto& movement = player.addComponent<MovementProperties>();
		movement.moveSpeed = 7.0f; // 最大速度

        // 添加技能请求组件
		player.addComponent<AbilityInput>();

        // 添加技能冷却组件
        player.addComponent<Cooldown>();

        // 添加暗能量组件
        auto& energy = player.addComponent<DarkEnergy>();
        energy.current = 100.0f;
        energy.max = 100.0f;
        energy.recoveryRate = 5.0f;

        // 添加腐蚀度组件
        auto& corruption = player.addComponent<Corruption>();
        corruption.current = 0.0f;
        corruption.lowThreshold = 30.0f;
        corruption.mediumThreshold = 70.0f;

		// 添加网格渲染组件
		auto& meshRenderer = player.addComponent<MeshRenderer>();
        // 创建玩家网格（三角形）
        std::vector<Mesh::Vertex> playerVertices = {
            {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, {0.0f, 1.0f, 1.0f}}, // 顶部
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // 左下
            {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}  // 右下
        };
        std::vector<unsigned int> playerIndices = { 0, 1, 2 };
        Mesh* playerMesh = new Mesh(playerVertices, playerIndices);

		meshRenderer.mesh = playerMesh;
		meshRenderer.color = glm::vec3(0.0f, 1.0f, 1.0f); // 玩家颜色为青色

        return player;
    }
}