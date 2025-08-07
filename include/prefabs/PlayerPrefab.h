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
#include "components/CombatInput.h"
#include "components/Cooldown.h"
#include "components/Health.h"
#include "components/Attack.h"
#include "components/Camera.h"

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
	/// @brief 创建玩家实体
	/// @details 创建一个玩家实体，并添加必要的组件和初始值设置。
	/// @param world [IN] 需要创建玩家的世界实例
	/// @return 返回创建的玩家实体
	inline Entity& createPlayer(World& world)
	{
		auto& player = world.createEntity();

		// 添加标记组件
		player.addComponent<Player>();

		// 添加变换组件
		auto& transform = player.addComponent<Transform>();
		transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform.updateDirectionVectors();

		// 添加摄像机组件
		player.addComponent<Camera>();

		// 添加速度组件
		player.addComponent<Velocity>();

		// 添加移动属性组件
		auto& movement = player.addComponent<MovementProperties>();
		movement.moveSpeed = 7.0f; // 最大速度

		// 添加生命值组件
		auto& health = player.addComponent<Health>();
		health.current = 10000000000.0f;
		health.max = 100000000000.0f;
		health.baseMax = 100.0f;

		// 添加攻击组件
		auto& attack = player.addComponent<Attack>();
		attack.damage = 10.0f;

		// 添加技能请求组件
		player.addComponent<AbilityInput>();

		// 添加技能冷却组件
		player.addComponent<Cooldown>();

		// 添加攻击请求组件
		player.addComponent<CombatInput>();

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
		meshRenderer.color = glm::vec3(0.0f, 1.0f, 0.0f); // 玩家颜色为绿色
		// 创建玩家网格（金字塔形状）
		glm::vec3 frontColor = { 1.0f, 0.0f, 0.0f }; // 正面颜色 - 红色（明显区分）

		// 定义顶点
		std::vector<Mesh::Vertex> playerVertices = {
			// 底部 (保持基础颜色)
			{{-0.5f, 0.0f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, meshRenderer.color},
			{{0.5f, 0.0f, -0.5f},  {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, meshRenderer.color},
			{{0.5f, 0.0f, 0.5f},   {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, meshRenderer.color},
			{{-0.5f, 0.0f, 0.5f},  {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, meshRenderer.color},

			// 正面 (使用明显不同的颜色 - 红色)
			{{0.0f, 1.0f, 0.0f},	{0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}, frontColor},
			{{0.5f, 0.0f, -0.5f},	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, frontColor},
			{{-0.5f, 0.0f, -0.5f},	{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, frontColor},

			// 右侧 (基础颜色)
			{{0.0f, 1.0f, 0.0f},   {1.0f, 0.0f, 0.0f}, {0.5f, 1.0f}, meshRenderer.color},
			{{0.5f, 0.0f, 0.5f},   {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, meshRenderer.color},
			{{0.5f, 0.0f, -0.5f},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, meshRenderer.color},

			// 背面 (基础颜色)
			{{0.0f, 1.0f, 0.0f},  {0.0f, 0.0f, -1.0f}, {0.5f, 1.0f}, meshRenderer.color},
			{{-0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, meshRenderer.color},
			{{0.5f, 0.0f, 0.5f},  {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, meshRenderer.color},

			// 左侧 (基础颜色)
			{{0.0f, 1.0f, 0.0f},   {-1.0f, 0.0f, 0.0f}, {0.5f, 1.0f}, meshRenderer.color},
			{{-0.5f, 0.0f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, meshRenderer.color},
			{{-0.5f, 0.0f, 0.5f},  {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, meshRenderer.color}
		};

		std::vector<unsigned int> playerIndices = {
			// 底部
			0, 1, 2,
			0, 2, 3,

			// 正面 (红色面)
			4, 5, 6,

			// 右侧
			7, 8, 9,

			// 背面
			10, 11, 12,

			// 左侧
			13, 14, 15
		};
		meshRenderer.mesh = new Mesh(playerVertices, playerIndices);

		return player;
	}
}