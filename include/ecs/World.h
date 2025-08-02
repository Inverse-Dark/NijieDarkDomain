#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "ecs/Entity.h"
#include "ecs/System.h"

/// @brief ECS世界 - 管理所有实体和系统
/// @details 该类允许创建实体，添加系统，并在每帧更新所有系统。
/// 
/// 设计思路：
/// 1. 集中管理所有游戏对象
/// 2. 按顺序更新所有系统
/// 3. 负责实体的创建和销毁
/// 
/// 为何这样做：
/// - 统一管理游戏状态
/// - 控制系统的执行顺序
/// - 提供实体生命周期管理
class World {
public:
	World() : m_nextEntityId(0) {}
	/// @brief 创建一个新的实体
	/// @details 创建一个新的实体并将其添加到世界中。实体的ID是唯一的，自动递增。
	/// @return 返回新创建的实体的引用
	Entity& createEntity() {
		m_entities.emplace_back(std::make_unique<Entity>(m_nextEntityId++));
		return *m_entities.back();
	}
	/// @brief 添加一个系统到世界中
	/// @details 将一个系统添加到世界中，以便在每帧更新时调用该系统的update方法。
	/// @param system [IN] 要添加的系统
	void addSystem(std::unique_ptr<System> system) {
		m_systems.push_back(std::move(system));
	}
	/// @brief 更新所有系统
	/// @details 在每帧调用此方法以更新所有已添加的系统。每个系统的update方法将被调用。
	/// @param deltaTime [IN] 上一帧到当前帧的时间差，用于系统更新逻辑
	void update(float deltaTime) {
		for (auto& system : m_systems) {
			system->update(*this, deltaTime);
		}
	}

	/// @brief 获取所有实体
	/// @details 返回一个对所有实体的引用，允许访问和操作世界中的所有实体。
	/// @return 返回一个对实体向量的常量引用
	const std::vector<std::unique_ptr<Entity>>& getEntities() const {
		return m_entities;
	}

private:
	std::vector<std::unique_ptr<Entity>> m_entities;    // 存储所有实体的向量
	std::vector<std::unique_ptr<System>> m_systems; // 存储所有系统的向量
	int m_nextEntityId;	// 下一个实体的ID，用于确保实体ID的唯一性
};