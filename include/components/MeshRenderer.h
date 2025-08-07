#pragma once
#include "ecs/Component.h"
#include "render/Mesh.h"

#include <glm/glm.hpp>

/// @brief 网格渲染组件 - 用于渲染3D网格
/// @details 该组件包含一个 Mesh 指针和一个可见性标志
/// 
/// 设计思路：
/// 1. 存储要渲染的网格对象指针
/// 2. 可选：添加材质属性（后续扩展）
/// 
/// 为何这样做：
/// - 将渲染数据与实体关联
/// - 分离渲染逻辑与变换逻辑
struct MeshRenderer : public Component {
	Mesh* mesh; // 要渲染的网格
	glm::vec3 color; // 网格颜色
	bool visible; // 是否可见

	/// @brief 默认构造函数
	/// @details 初始化 mesh 为 nullptr，visible 为 true
	MeshRenderer()
		: mesh(nullptr), color(glm::vec3(1.0f, 1.0f, 1.0f)), visible(true)
	{
	}
	/// @brief 析构函数
	/// @details 确保释放 mesh 资源，避免内存泄漏
	~MeshRenderer()
	{
		if (mesh)
		{
			delete mesh; // 确保释放资源
			mesh = nullptr; // 避免悬空指针
		}
	}

	/// @brief 带参数的构造函数
	/// @details 初始化 mesh 和 visible
	/// @param mesh [IN] 要渲染的网格指针
	/// @param color [IN] 网格颜色，默认为白色 (1.0f, 1.0f, 1.0f)
	/// @param visible [IN] 是否可见，默认为 true
	MeshRenderer(Mesh* mesh, glm::vec3 color, bool visible)
		: mesh(mesh), color(color), visible(visible)
	{}
};