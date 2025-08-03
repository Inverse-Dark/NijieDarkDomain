#include "render/RenderSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "render/Shader.h"
#include "render/Mesh.h"
#include "components/MeshRenderer.h"
#include "components/Transform.h"
#include "core/Logger.h"

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

            // 设置颜色
            m_pCoreShader->setVec3("color", renderer->color);

            // 绘制网格
            renderer->mesh->draw();
        }
    }

}
