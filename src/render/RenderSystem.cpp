#include "render/RenderSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "render/Shader.h"
#include "render/Mesh.h"
#include "components/MeshRenderer.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "components/Player.h"
#include "core/Logger.h"

#include <glad/glad.h>

RenderSystem::RenderSystem() {
    // 加载核心着色器
    m_pCoreShader = new Shader("resources/shaders/core.vert", "resources/shaders/core.frag");

    // 设置相机位置
    m_viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), // 相机位置
        glm::vec3(0.0f, 0.0f, 0.0f), // 目标位置
        glm::vec3(0.0f, 1.0f, 0.0f)  // 上方向
    );

    // 设置投影矩阵
    m_projectionMatrix = glm::perspective(
        glm::radians(45.0f), // FOV
        1280.0f / 720.0f,   // 宽高比
        0.1f,               // 近平面
        100.0f              // 远平面
    );

    Logger::instance()->log("渲染系统初始化完成");
}

RenderSystem::~RenderSystem() 
{
    if (m_pCoreShader) {
        delete m_pCoreShader;
        m_pCoreShader = nullptr;
    }
    Logger::instance()->log("渲染系统已销毁");
}

void RenderSystem::update(World& world, float deltaTime) 
{
	for (auto& entity : world.getEntities()) {
		if (auto* camera = entity->getComponent<Camera>()) {
			if (auto* transform = entity->getComponent<Transform>()) {
                // 4. 计算相机视角（看向玩家前方）
                glm::vec3 target = transform->position + glm::vec3(0.0f, 1.0f, 0.0f);
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
				// 更新视图矩阵为玩家位置
				m_viewMatrix = glm::lookAt(
					camera->position, // 相机位置
                    target, // 目标位置
                    up // 上方向
				);
			}
		}
	}

    // 使用着色器
	m_pCoreShader->use();

    // 设置着色器的视图和投影矩阵
    m_pCoreShader->setMat4("view", m_viewMatrix);
    m_pCoreShader->setMat4("projection", m_projectionMatrix);

    // 设置时间uniform（用于着色器动画）
    static float totalTime = 0.0f;
    totalTime += deltaTime;
    m_pCoreShader->setFloat("time", totalTime);

    // 渲染所有实体
    for (auto& entity : world.getEntities()) {
        auto* renderer = entity->getComponent<MeshRenderer>();
        auto* transform = entity->getComponent<Transform>();

        if (renderer && renderer->mesh && transform) {
            // 设置模型矩阵
            glm::mat4 model = transform->getModelMatrix();
            m_pCoreShader->setMat4("model", model);

            // 绘制网格
            renderer->mesh->draw();
        }
    }

    // 灰色地板
    drawDebugFloor(20, 1.0f);

}

void RenderSystem::drawDebugArrow(Shader* shader, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color) {
    if (glm::length(direction) < 0.001f) return;

    // 创建箭头模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    // 计算旋转使箭头指向方向
    glm::vec3 normalizedDir = glm::normalize(direction);
    glm::vec3 up = glm::abs(normalizedDir.y) > 0.99f ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::cross(normalizedDir, up);
    glm::vec3 actualUp = glm::cross(right, normalizedDir);

    model = glm::rotate(model, glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), normalizedDir)),
        glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), normalizedDir));

    // 设置模型矩阵和颜色
    shader->setMat4("model", model);

    // 创建箭头顶点数据
    float arrowVertices[] = {
        // 主体
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,

        // 头部
        0.0f, 0.0f, 1.0f,
        0.1f, 0.0f, 0.8f,

        0.0f, 0.0f, 1.0f,
        -0.1f, 0.0f, 0.8f,

        // 十字标识
        -0.1f, 0.0f, 0.0f,
        0.1f, 0.0f, 0.0f,

        0.0f, -0.1f, 0.0f,
        0.0f, 0.1f, 0.0f
    };

    // 创建并绑定VAO/VBO
    static GLuint vao = 0, vbo = 0;
    if (vao == 0) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(arrowVertices), arrowVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    // 绘制
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 10); // 10个顶点 = 5条线
    glBindVertexArray(0);
}

void RenderSystem::drawDebugFloor(int halfSize, float step) const
{
    using Vertex = Mesh::Vertex;
    static GLuint vao = 0, vbo = 0;

    if (vao == 0)
    {
        std::vector<Vertex> vertices;
        glm::vec3 floorColor = glm::vec3(1.35f);

        for (int x = -halfSize; x <= halfSize; ++x)
        {
            vertices.emplace_back(Vertex{ glm::vec3(x * step, 0.0f, -halfSize * step), glm::vec3(0, 1, 0), glm::vec2(0), floorColor });
            vertices.emplace_back(Vertex{ glm::vec3(x * step, 0.0f, halfSize * step), glm::vec3(0, 1, 0), glm::vec2(0), floorColor });
        }
        for (int z = -halfSize; z <= halfSize; ++z)
        {
            vertices.emplace_back(Vertex{ glm::vec3(-halfSize * step, 0.0f, z * step), glm::vec3(0, 1, 0), glm::vec2(0), floorColor });
			vertices.emplace_back(Vertex{ glm::vec3(halfSize * step, 0.0f, z * step), glm::vec3(0, 1, 0), glm::vec2(0), floorColor });
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(3);
    }

    glm::mat4 model = glm::mat4(1.0f);
    m_pCoreShader->setMat4("model", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, (halfSize * 2 + 1) * 4);
    glBindVertexArray(0);
}