#include "core/Application.h"
#include "core/InputMap.h"
#include "core/Logger.h"
#include "ecs/World.h"
#include "systems/PlayerControlSystem.h"
#include "systems/AbilitySystem.h"
#include "systems/CorruptionSystem.h"
#include "systems/MovementSystem.h"
#include "systems/EnvironmentSystem.h"
#include "systems/AISystem.h"
#include "systems/CombatSystem.h"
#include "systems/CameraSystem.h"
#include "render/RenderSystem.h"
#include "prefabs/PlayerPrefab.h"
#include "prefabs/EnemyPrefab.h"

Application::Application(const std::string title, int width, int height)
	: m_pWindow(nullptr),
	m_glContext(nullptr),
	m_windowTitle(title),
	m_screenWidth(width),
	m_screenHeight(height),
	m_bIsRunning(true),
	m_pLogger(Logger::instance())
{
	m_pLogger->log("应用程序实例创建");
}

Application::~Application()
{
	shutdown(); // 清理资源
	m_pLogger->log("应用程序实例销毁");
	Logger::destroy();	// 销毁日志记录器
}

bool Application::initialize()
{
	m_pLogger->log("初始化应用程序...");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		m_pLogger->log("SDL初始化失败: " + std::string(SDL_GetError()), Logger::LogLevel::ERROR);
		return false;
	}

	// 设置OpenGL属性
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);	// 设置OpenGL主版本号
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);	// 设置OpenGL次版本号
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	// 设置OpenGL核心配置文件
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// 启用双缓冲
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	// 设置深度缓冲大小为24位
	SDL_SetRelativeMouseMode(SDL_TRUE);	// 锁定鼠标到窗口中心
	SDL_ShowCursor(SDL_DISABLE);	// 隐藏鼠标光标

	m_pWindow = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!m_pWindow) {
		m_pLogger->log("窗口创建失败: " + std::string(SDL_GetError()), Logger::LogLevel::ERROR);
		SDL_Quit();
		return false;
	}

	// 创建OpenGL上下文
	m_glContext = SDL_GL_CreateContext(m_pWindow);
	if (!m_glContext) {
		m_pLogger->log("OpenGL上下文创建失败: " + std::string(SDL_GetError()), Logger::LogLevel::ERROR);
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
		SDL_Quit();
		return false;
	}
	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		m_pLogger->log("GLAD初始化失败", Logger::LogLevel::ERROR);
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_pWindow);
		m_glContext = nullptr;
		m_pWindow = nullptr;
		SDL_Quit();
		return false;
	}

	m_pLogger->log("OpenGL版本: " + std::string((const char*)glGetString(GL_VERSION)));
	m_pLogger->log("显卡: " + std::string((const char*)glGetString(GL_RENDERER)));

	// 初始化OpenGL状态
	initOpenGLState();

	m_pInputMap = std::make_unique<InputMap>(); // 创建输入映射实例
	m_pInputMap.get()->addActionListener(InputMap::ExitGame, [this]() {
		m_bIsRunning = false; // 按下ESC键退出
		m_pLogger->log("按下ESC键，退出游戏");
		}, InputMap::Pressed);

	m_pLogger->log("应用程序初始化完成");
	return true;
}

void Application::initOpenGLState()
{
	// 设置OpenGL基本参数
	glViewport(0, 0, m_screenWidth, m_screenHeight);	// 设置视口
	glEnable(GL_DEPTH_TEST); // 启用深度测试
	glClearColor(0.05f, 0.02f, 0.08f, 1.0f); // 设置清除颜色（暗域主题的深紫色）

	// 启用混合（用于透明效果）
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 启用面剔除（优化）
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Application::run()
{
	World world;	// 创建ECS世界
	float deltaTime = 0.0f; // 帧时间初始化为0

	m_pLogger->log("开始游戏主循环");

	// 创建玩家实体并添加到ECS世界
	Prefab::createPlayer(world);

	// 创建初始环境
	auto& envSystem = std::make_unique<EnvironmentSystem>();
	envSystem->spawnCorruptionSource(world, glm::vec3(10.0f, 0.0f, 10.0f), 15.0f, 8.0f);
	envSystem->spawnCorruptionSource(world, glm::vec3(-10.0f, 0.0f, -10.0f), 12.0f, 6.0f);

	world.addSystem(std::make_unique<MovementSystem>()); // 添加移动系统到ECS世界
	world.addSystem(std::make_unique<CameraSystem>(m_pInputMap.get())); // 添加相机系统到ECS世界
	world.addSystem(std::make_unique<PlayerControlSystem>(m_pInputMap.get())); // 添加玩家控制系统到ECS世界
	world.addSystem(std::make_unique<AbilitySystem>()); // 添加能力系统到ECS世界
	world.addSystem(std::make_unique<CorruptionSystem>()); // 添加腐化系统到ECS世界
	world.addSystem(std::make_unique<CombatSystem>()); // 添加战斗系统到ECS世界
	world.addSystem(std::make_unique<AISystem>()); // 添加AI系统到ECS世界
	world.addSystem(std::move(envSystem)); // 添加环境系统到ECS世界
	world.addSystem(std::make_unique<RenderSystem>()); // 添加渲染系统到ECS世界

	// 创建测试敌人
	Prefab::createDarkCreature(world, glm::vec3(10.0f, 0.0f, 0.0f));
	auto& entity1 = Prefab::createDarkCreature(world, glm::vec3(-10.0f, 0.0f, 0.0f));
	if(auto* meshRenderer = entity1.getComponent<MeshRenderer>())
		meshRenderer->mesh->updateColor(glm::vec3(0.5f, 0.2f, 0.8f)); // 设置敌人颜色为暗紫色
	auto& entity2 = Prefab::createDarkCreature(world, glm::vec3(0.0f, 0.0f, 10.0f));
	if(auto* meshRenderer = entity2.getComponent<MeshRenderer>())
		meshRenderer->mesh->updateColor(glm::vec3(0.2f, 0.5f, 0.8f)); // 设置敌人颜色为暗蓝色
	auto& entity3 = Prefab::createDarkCreature(world, glm::vec3(0.0f, 0.0f, -10.0f));
	if(auto* meshRenderer = entity3.getComponent<MeshRenderer>())
		meshRenderer->mesh->updateColor(glm::vec3(0.8f, 0.2f, 0.5f)); // 设置敌人颜色为暗粉色

	m_frameTimer.start(); // 启动帧计时器

	while (m_bIsRunning) {
		// 计算帧时间
		deltaTime = m_frameTimer.restart();

		// 处理事件
		processEvents();

		// 处理输入映射
		m_pInputMap->update();

		// 渲染场景
		render();

		// 更新世界状态
		update(deltaTime);

		// 更新ECS世界
		world.update(deltaTime);
		world.processDestruction();

		// 交换缓冲区
		SDL_GL_SwapWindow(m_pWindow);
	}
	m_pLogger->log("游戏主循环结束");
}

void Application::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_bIsRunning = false;
			m_pLogger->log("接收到退出事件");
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				// 窗口大小变化时更新视口
				m_screenWidth = event.window.data1;
				m_screenHeight = event.window.data2;
				glViewport(0, 0, m_screenWidth, m_screenHeight);
				m_pLogger->log("窗口大小改变: " +
					std::to_string(m_screenWidth) + "x" +
					std::to_string(m_screenHeight));
			}
			break;
		}
		m_pInputMap.get()->processEvent(event); // 处理输入事件
	}
}

void Application::update(float deltaTime)
{

}

void Application::render()
{
	// 清除颜色缓冲和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Application::shutdown() {
	m_pLogger->log("清理应用程序资源...");

	if (m_glContext) {
		SDL_GL_DeleteContext(m_glContext);
		m_glContext = nullptr;
	}

	if (m_pWindow) {
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}

	SDL_Quit();
	m_pLogger->log("资源清理完成");
}