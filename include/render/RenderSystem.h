#pragma once
#include "ecs/System.h"

#define GLM_ENABLE_EXPERIMENTAL
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
	/// @brief 绘制调试箭头
	/// @details 使用指定的着色器在给定位置绘制一个箭头，表示方向
	/// @param shader [IN] 用于渲染的着色器
	/// @param position [IN] 箭头的起始位置
	/// @param direction [IN] 箭头的方向向量
	/// @param color [IN] 箭头的颜色
	void drawDebugArrow(Shader* shader, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color);

	/// @brief 绘制调试地面网格
	/// @details 在XZ平面上绘制一个网格，作为地面参考
	/// @param halfSize [IN] 网格的半尺寸（默认20）
	/// @param step [IN] 网格线之间的间距（默认1.0f）
	void drawDebugFloor(int halfSize = 20, float step = 1.0f) const;
private:
	Shader* m_pCoreShader;	// 渲染使用的着色器程序

	glm::mat4 m_viewMatrix;		// 视图矩阵
	glm::mat4 m_projectionMatrix;	// 投影矩阵
};