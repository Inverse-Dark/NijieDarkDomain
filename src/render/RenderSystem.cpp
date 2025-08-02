#include "render/RenderSystem.h"
#include "ecs/World.h"
#include "ecs/Entity.h"
#include "render/Shader.h"
#include "render/Mesh.h"
#include "core/Logger.h"

RenderSystem::RenderSystem() {
    // 加载核心着色器
    m_pCoreShader = new Shader("resources/shaders/core.vert", "resources/shaders/core.frag");

    // 创建测试网格
    createTestMesh();

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
    if(m_pTestCube) {
        delete m_pTestCube;
        m_pTestCube = nullptr;
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

    // 渲染测试立方体
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, totalTime, glm::vec3(0.5f, 1.0f, 0.0f));
    m_pCoreShader->setMat4("model", model);
    m_pTestCube->draw();
}

void RenderSystem::createTestMesh() {
    // 立方体顶点数据
    std::vector<Mesh::Vertex> vertices = {
        // 前面
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},

        // 后面
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}},
    };

    // 立方体索引数据
    std::vector<unsigned int> indices = {
        // 前面
        0, 1, 2, 2, 3, 0,
        // 右面
        1, 5, 6, 6, 2, 1,
        // 后面
        5, 4, 7, 7, 6, 5,
        // 左面
        4, 0, 3, 3, 7, 4,
        // 上面
        3, 2, 6, 6, 7, 3,
        // 下面
        4, 5, 1, 1, 0, 4
    };

    // 创建网格
    m_pTestCube = new Mesh(vertices, indices);
    Logger::instance()->log("测试立方体创建成功");
}
