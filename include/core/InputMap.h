#pragma once
#include <SDL.h>
#include <unordered_map>
#include <functional>

/// @brief 输入映射系统 - 管理按键绑定
/// @details 该系统允许将按键事件映射到特定的实体或操作上
/// 
/// 设计思路：
/// 1. 将物理按键映射到逻辑动作
/// 2. 支持动态重绑定
/// 
/// 为何这样做：
/// - 提高输入处理的灵活性
/// - 支持玩家自定义按键
class InputMap
{
public:
	/// @brief 逻辑动作枚举
	/// @details 定义了游戏中的各种操作
	enum Action
	{
		ExitGame,			// 退出游戏

		MoveForward,	// 向前移动
		MoveBackward,	// 向后移动
		MoveLeft,		// 向左转向
		MoveRight,		// 向右转向

		AttackPrimary,	// 主攻击
		Ability1,		// 使用技能1
		Ability2,		// 使用技能2
		Ability3,		// 使用技能3
		Ability4,		// 使用技能4
		Ability5		// 使用技能5
	};

	/// @brief 状态类型枚举
	/// @details 定义了按键的状态类型
	enum StateType
	{
		Pressed,	// 按键按下
		Released,	// 按键释放
		Held	// 按键按住
	};

	/// @brief 鼠标移动绑定结构体
	/// @details 包含鼠标移动的坐标和相对坐标
	struct MouseMove {
		int x, y; // 鼠标移动的坐标
		int xRel, yRel; // 鼠标相对移动的坐标
	};
public:
	/// @brief 构造函数
	/// @details 初始化输入映射系统，设置默认按键绑定
	InputMap();

	/// @brief 绑定按键到逻辑动作
	/// @details 将指定的按键绑定到逻辑动作上
	/// @param key [IN] SDL_Keycode 键盘按键代码
	/// @param action [IN] 逻辑动作枚举
	/// @param state [IN] 状态类型，默认为Pressed（按下）
	void bindKey(SDL_Keycode key, Action action, StateType state = Pressed);
	/// @brief 绑定按键到逻辑动作
	/// @details 将指定的按键绑定到逻辑动作上
	/// @param button [IN] 鼠标按键代码
	/// @param action [IN] 逻辑动作枚举
	/// @param state [IN] 状态类型，默认为Pressed（按下）
	void bindMouse(int button, Action action, StateType state = Pressed);

	/// @brief 处理输入事件
	/// @details 根据SDL事件更新逻辑动作状态
	/// @param event [IN] SDL_Event 输入事件
	void processEvent(const SDL_Event& event);

	/// @brief 更新输入状态
	/// @details 更新所有逻辑动作的状态，检查按键是否被按下或释放
	void update();

	/// @brief 获取按键持续状态
	/// @details 检查指定的逻辑动作是否被按住
	/// @param action [IN] 逻辑动作枚举
	/// @return 如果动作被按住返回true，否则返回false
	bool isActionHeld(Action action) const;
	
	/// @brief 添加动作监听器
	/// @details 当逻辑动作状态变化时调用回调函数
	/// @param action [IN] 逻辑动作枚举
	/// @param callback [IN] 状态变化回调函数
	/// @param state [IN] 状态类型，默认为Pressed（按下）
	void addActionListener(Action action, std::function<void()> callback, StateType state = Pressed);

	/// @brief 获取鼠标移动
	/// @details 获取鼠标移动的坐标和相对坐标
	/// @return 鼠标移动绑定结构体，包含坐标和相对坐标
	MouseMove getMouseMove() const;
	/// @brief 重置鼠标相对坐标
	/// @details 重置鼠标相对坐标为0
	void resetMouseRelative();
private:
	/// @brief 按键绑定结构体
	/// @details 包含逻辑动作和状态类型
	struct KeyBinding {
		Action action;
		StateType state;
	};
	/// @brief 活动事件结构体
	/// @details 包含逻辑动作和状态类型，用于处理按键事件
	struct ActiveEvent {
		Action action;
		StateType state;
	};
	/// @brief 按键绑定结构体
	/// @details 包含逻辑动作和状态类型
	struct MouseBinding {
		Action action;
		StateType state;
	};
private:
	std::unordered_map<SDL_Keycode, std::vector<KeyBinding>> m_keyBindings;	// 键盘按键到逻辑动作的映射
	std::unordered_map<int, std::vector<MouseBinding>> m_mouseBindings;	// 鼠标按键到逻辑动作的映射
	std::unordered_map<SDL_Keycode, bool> m_keyStates;	// 逻辑动作的当前状态（按下或未按下）
	std::unordered_map<int, bool> m_mouseStates;	// 逻辑动作的当前状态（按下或未按下）
	std::unordered_map<Action, std::vector<std::function<void()>>> m_pressedListeners;	// 逻辑动作按下的监听器
	std::unordered_map<Action, std::vector<std::function<void()>>> m_releasedListeners;	// 逻辑动作释放的监听器
	std::vector<ActiveEvent> m_activeEvents;	// 当前活动的按键事件列表，用于处理按键状态变化

	MouseMove m_mouseMove;	// 鼠标移动绑定，用于处理鼠标移动事件
};
