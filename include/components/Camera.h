#pragma once
#include "ecs/Component.h"

#include <glm/glm.hpp>

/// @brief 相机组件 - 用于控制游戏中的相机行为
/// @details 包含相机的偏移、距离、灵敏度等属性，以及俯仰和偏航角度控制
/// 
/// 设计思路：
/// 1. 相机组件用于控制游戏中的相机行为，允许玩家通过鼠标输入来调整视角。
/// 2. 包含相机的偏移、距离、灵敏度等属性，以及俯仰和偏航角度控制。
/// 3. 相机的偏移量可以根据玩家的高度和位置进行调整，以确保相机始终位于玩家的后方和上方。
/// 
/// 为何这样做：
/// - 相机组件使得游戏中的视角控制更加灵活和直观，玩家可以通过鼠标移动来调整视角。
/// - 通过设置相机的偏移和距离，可以确保玩家在游戏中有一个良好的视角体验
struct Camera : public Component {
	glm::vec3 position; // 相机位置
    glm::vec3 offset; // 相机相对于玩家的偏移
	float distance;  // 相机与玩家之间的距离
	float minDistance;   // 最小距离
	float maxDistance;  // 最大距离
	float sensitivity;   // 鼠标灵敏度
	float pitch; // 俯仰角
	float yaw;   // 偏航角
	float minPitch;	// 最小俯仰角
	float maxPitch;	// 最大俯仰角

	/// @brief 默认构造函数
	/// @details 初始化相机位置、偏移、距离、灵敏度等属性
    Camera()
		: position(0.0f, 0.0f, 0.0f)
		, offset(0.0f, 1.5f, 0.0f), distance(3.0f)
        , minDistance(2.0f), maxDistance(10.0f)
        , sensitivity(0.1f), pitch(0.0f), yaw(0.0f)
        , minPitch(-30.0f), maxPitch(50.0f)
    {
	}
};