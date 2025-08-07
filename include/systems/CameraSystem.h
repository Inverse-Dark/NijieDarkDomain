#pragma once
#include "ecs/System.h"

// 前向声明
class InputMap;
class Entity;

/// @brief 相机系统 - 处理相机的更新和鼠标输入
/// @details 该系统负责更新相机的位置和方向，并处理鼠标移动事件。
/// 
/// 设计思路：
/// 1. 相机系统将根据玩家的输入更新相机的位置和方向。
/// 2. 鼠标移动事件将用于调整相机的视角。
/// 
/// 为何这样做：
/// - 相机系统是游戏中不可或缺的一部分，它允许玩家查看游戏世界。
/// - 通过处理鼠标输入，可以实现更自然的视角控制。
/// - 相机系统可以与其他系统（如渲染系统）协同工作，以确保正确显示游戏世界。
class CameraSystem : public System {
public:
	/// @brief 构造函数
	/// @details 初始化移动状态和速度
	/// @param inputMap [IN] 输入映射，用于处理键盘输入
	CameraSystem(InputMap* inputMap);

	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
    virtual void update(World& world, float deltaTime) override;
private:
	/// @brief 处理鼠标移动
	/// @details 根据鼠标相对移动量更新相机的方向
	/// @param entity [IN] 携带相机的实体
	/// @param xrel [IN] 鼠标在X轴上的相对移动量
	/// @param yrel [IN] 鼠标在Y轴上的相对移动量
    void handleMouseMotion(Entity* entity, int xrel, int yrel);

private:
    InputMap* m_pInputMap;	// 输入映射，用于处理玩家输入
};