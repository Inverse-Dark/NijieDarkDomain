#pragma once
#include <vector>
#include <glm/glm.hpp>

/// @brief 网格类 - 表示3D对象的几何数据
/// @details 包含顶点、法线、纹理坐标和索引数据，用于渲染3D模型
/// 
/// 设计思路：
/// 1. 封装顶点数据和渲染状态
/// 2. 提供简单的渲染接口
/// 3. 支持基本图元创建
/// 
/// 为何这样做：
/// - 抽象OpenGL缓冲区管理
/// - 简化网格渲染流程
/// - 为后续资源加载系统做准备
class Mesh
{
public:
	/// @brief 顶点结构体 - 包含位置、法线、纹理坐标和颜色
	/// @details 用于描述单个顶点的属性
	struct Vertex
	{
		glm::vec3 position;  // 顶点位置
		glm::vec3 normal;    // 法线向量
		glm::vec2 texCoords; // 纹理坐标
		glm::vec3 color;   // 顶点颜色
	};
public:
	/// @brief 构造函数 - 初始化网格数据
	/// @details 创建顶点数组对象和缓冲区对象
	/// @param vertices [IN] 顶点数据
	/// @param indices [IN] 索引数据
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	/// @brief 析构函数 - 清理资源
	/// @details 删除顶点数组对象
	~Mesh();

	/// @brief 渲染网格
	/// @details 绑定VAO并绘制元素
	void draw() const;

	/// @brief 获取顶点数据
	/// @details 返回存储的顶点数组
	std::vector<Vertex> getVertices() const;

	/// @brief 更新顶点颜色
	/// @details 更新所有顶点的颜色属性
	void updateColor(glm::vec3 color);

private:
	/// @brief 设置网格的OpenGL缓冲区对象
	/// @details 创建并绑定VAO、VBO和EBO，上传顶点和索引数据
	void setupMesh();

private:
	unsigned int VAO; // 顶点数组对象
	unsigned int VBO; // 顶点缓冲对象
	unsigned int EBO; // 索引缓冲对象

	std::vector<Vertex> m_vertices; // 顶点数据
	std::vector<unsigned int> m_indices; // 索引数据
};