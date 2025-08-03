#pragma once

/// @brief 能力类型枚举 - 定义界痕使的能力
/// @details 每种能力对应不同的效果和用途
enum class AbilityType {
    Perception,     // 感知：显示隐藏元素
    Manipulation,   // 操控：移动物体
    Distortion,     // 扭曲：改变重力
    Assimilation,   // 同化：控制敌人
    Purification    // 净化：减少腐蚀
};