cmake_minimum_required(VERSION 3.20)
project(NijieDarkDomain VERSION 0.0.1 LANGUAGES C CXX)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)   # VS兼容
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin) # VS兼容

# 其他设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
set(CMAKE_TOOLCHAIN_FILE "D:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")

# 依赖查找
find_package(SDL2 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

# 可执行文件
add_executable(${PROJECT_NAME}
    "src/core/Application.cpp"
    "src/main.cpp"
    "vendor/glad/src/glad.c"
    "src/core/Timer.cpp" 
    "src/core/Logger.cpp" 
    "src/render/Shader.cpp" 
    "src/render/Mesh.cpp" 
    "src/render/RenderSystem.cpp"
    "src/systems/PlayerControlSystem.cpp"
    "src/systems/AbilitySystem.cpp"
    "src/systems/CorruptionSystem.cpp"
    "src/systems/MovementSystem.cpp" 
    "src/core/InputMap.cpp" 
    "src/systems/EnvironmentSystem.cpp" 
    "src/systems/AISystem.cpp" 
    "src/systems/CombatSystem.cpp"
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2 
    SDL2::SDL2main
    OpenGL::GL
)

# 创建 bin/resources/shaders 目录并复制着色器
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders)
file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders
)