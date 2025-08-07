#include "render/Mesh.h"
#include "core/Logger.h"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	: m_vertices(vertices), m_indices(indices)
{
	setupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);  // 删除VAO
	glDeleteBuffers(1, &VBO);   // 删除VBO
	glDeleteBuffers(1, &EBO);	// 删除EBO

    Logger::instance()->log("网格资源已释放");
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);	// 绑定VAO
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);	// 绘制网格
	glBindVertexArray(0);	// 解绑VAO
}

std::vector<Mesh::Vertex> Mesh::getVertices() const
{
	return m_vertices;
}

void Mesh::updateColor(glm::vec3 color)
{
	for (auto& vertex : m_vertices) {
		vertex.color = color;  // 更新所有顶点的颜色
	}
	// 重新上传顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Logger::instance()->log("网格颜色已更新");
}

void Mesh::setupMesh()
{
	// 创建缓冲区
	glGenVertexArrays(1, &VAO);  // 生成VAO
	glGenBuffers(1, &VBO);   // 生成VBO
	glGenBuffers(1, &EBO);   // 生成EBO

	glBindVertexArray(VAO);  // 绑定VAO

	// 填充顶点缓冲区
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	// 填充索引缓冲区
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	// 设置顶点属性指针

	// 位置属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// 法线属性
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// 纹理坐标属性
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	// 颜色属性
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindVertexArray(0);

	Logger::instance()->log("网格初始化完成，顶点数: " + std::to_string(m_vertices.size()) +
		", 索引数: " + std::to_string(m_indices.size()));

}
