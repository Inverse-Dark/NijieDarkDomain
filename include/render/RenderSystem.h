#pragma once
#include "ecs/System.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 前向声明
class Shader;	// 着色器类
class Mesh;	// 网格类

/// @brief 渲染系统 - 负责所有渲染逻辑
/// @details 该系统处理所有渲染相关的操作，包括加载着色器、网格和渲染实体。
/// 
/// 设计思路：
/// 1. 管理着色器和渲染状态
/// 2. 处理所有需要渲染的实体
/// 3. 实现相机和投影矩阵
/// 
/// 为何这样做：
/// - 集中管理渲染资源
/// - 实现渲染优化（如批处理）
/// - 分离渲染逻辑与游戏逻辑
class RenderSystem : public System
{
public:
	/// @brief 构造函数
	/// @details 初始化渲染系统，加载必要的着色器和网格数据。
	RenderSystem();
	/// @brief 析构函数
	/// @details 清理渲染系统，释放资源。
	~RenderSystem() override;
	
	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
	void update(World& world, float deltaTime) override;
	
private:
	/// @brief 创建测试网格（临时）
	/// @details 该方法用于创建一个简单的测试网格，便于验证渲染系统是否正常工作。
	void createTestMesh();

private:
	Shader* m_pCoreShader;	// 渲染使用的着色器程序
	Mesh* m_pTestCube;		// 渲染使用的网格数据(临时)

	glm::mat4 m_viewMatrix;		// 视图矩阵
	glm::mat4 m_projectionMatrix;	// 投影矩阵
};