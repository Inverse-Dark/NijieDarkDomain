#pragma once

/// @brief 组件基类
/// @details 所有组件都应该继承自此类
/// 
/// 设计思路：
/// 1. 所有组件继承自这个基类
/// 2. 组件是纯数据容器，不包含逻辑
/// 3. 使用空基类优化内存布局
/// 
/// 为何这样做：
/// - 统一组件类型，便于ECS管理
/// - 支持运行时类型识别
/// - 为后续序列化/反序列化提供基础
class Component
{
public:
    virtual ~Component() = default;
};