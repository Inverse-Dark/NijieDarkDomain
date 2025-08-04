#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "ecs/Component.h"

// 前向声明
class World;

/// @brief 实体类 - 表示游戏中的对象
/// @details 实体可以拥有多个组件，每个组件提供不同的功能或数据。
/// 
/// 设计思路：
/// 1. 实体是组件的容器
/// 2. 使用唯一ID标识实体
/// 3. 通过类型索引存储和检索组件
/// 
/// 为何这样做：
/// - 灵活组合组件，实现不同行为
/// - 高效的类型安全组件访问
/// - 支持动态添加/移除组件
class Entity {
public:
	/// @brief 构造函数
	/// @details 使用整数ID来唯一标识实体，便于在系统中管理和引用实体。
	/// @param id [IN] 实体的唯一标识符
	/// @param world [IN] 实体所属世界的引用
	Entity(int id, World* world) : m_id(id), m_world(world) {}

	/// @brief 添加组件
	/// @details 使用模板函数来添加不同类型的组件，避免了类型转换的复杂性。
	/// @tparam T [IN] 组件类型
	/// @tparam Args [IN] 组件构造函数参数类型
	/// @param args [IN] 组件构造函数参数
	/// @return 返回添加的组件的引用
	template <typename T, typename... Args>
	T& addComponent(Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

		auto comp = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = comp.get();
		m_components[typeid(T)] = std::move(comp);
		return *ptr;
	}

	/// @brief 移除组件
	/// @details 使用模板函数来移除指定类型的组件，返回true表示成功移除。
	/// @tparam T [IN] 组件类型
	/// @return 返回是否成功移除组件
	template <typename T>
	bool removeComponent() {
		auto it = m_components.find(typeid(T));
		if (it != m_components.end()) {
			m_components.erase(it);
			return true;
		}
		return false;
	}

	/// @brief 获取组件
	/// @details 使用模板函数来获取指定类型的组件，返回nullptr表示组件不存在。
	/// @tparam T [IN] 组件类型
	/// @return 返回组件指针，如果不存在则返回nullptr
	template <typename T>
	T* getComponent() {
		auto it = m_components.find(typeid(T));
		if (it != m_components.end()) {
			return static_cast<T*>(it->second.get());
		}
		return nullptr;
	}

	/// @brief 检查是否拥有组件
	/// @details 使用模板函数来检查实体是否拥有指定类型的组件，返回true表示拥有。
	/// @tparam T [IN] 组件类型
	/// @return 返回是否拥有指定类型的组件
	template <typename T>
	bool hasComponent() const {
		return m_components.find(typeid(T)) != m_components.end();
	}

	/// @brief 获取实体ID
	/// @details 返回实体的唯一标识符，便于在系统中引用和管理实体。
	/// @return 返回实体的唯一ID
	int getId() const { return m_id; }

	/// @brief 获取实体所属世界的引用
	/// @details 返回实体所属世界的引用，便于在系统中引用和管理实体。
	/// @return 返回实体所属世界的引用
	World& getWorld() { return *m_world; }
private:
	int m_id;   // 实体的唯一ID
	World* m_world;	// 所属世界的引用
	std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;   // 存储组件的映射
};