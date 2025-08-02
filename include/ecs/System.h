#pragma once
class World;

/// @brief 系统基类 - 处理特定组件组合的逻辑
/// @details 系统负责更新和处理游戏世界中的实体和组件。每个系统通常关注特定类型的组件组合，并在每帧更新时执行逻辑。
/// 
/// 设计思路：
/// 1. 每个系统负责处理一种特定的功能
/// 2. 系统在update方法中处理符合条件的实体
/// 3. 系统不直接存储实体，而是通过世界查询
/// 
/// 为何这样做：
/// - 分离关注点，提高代码可维护性
/// - 便于性能优化（如批处理）
/// - 支持系统执行顺序控制
class System {
public:
	/// @brief 析构函数
	/// @details 系统在销毁时不需要清理实体或组件，因为它们由World管理
    virtual ~System() = default;
	/// @brief 更新系统状态
	/// @details 每帧调用一次，处理符合条件的实体和组件
	/// @param world [IN] 当前游戏世界
	/// @param deltaTime [IN] 时间增量
    virtual void update(World& world, float deltaTime) = 0;
};