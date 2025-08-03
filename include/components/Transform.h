#pragma once
#include "ecs/Component.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/// @brief 变换矩阵 - 表示实体的位置、旋转和缩放
/// @details 该组件包含位置、旋转和缩放信息，通常用于3D图形中的实体变换。
/// 
/// 设计思路：
/// 1. 存储位置、旋转和缩放信息
/// 2. 提供方向向量（前、右、上）
/// 3. 计算模型矩阵
/// 
/// 为何这样做：
/// - 统一管理空间变换
/// - 为渲染和物理提供基础数据
/// - 支持层级变换（未来扩展）
struct Transform : public Component
{
    glm::vec3 position;   // 位置向量
    glm::quat rotation; // 旋转四元数
    glm::vec3 scale;  // 缩放向量

    // 方向向量（计算得出）
    glm::vec3 forward;   // 前向量
    glm::vec3 right;  // 右向量
    glm::vec3 up; // 上向量

    // 重力方向（用于扭曲能力）
    glm::vec3 gravityDirection;  // 重力方向向量

	/// @brief 默认构造函数
	/// @details 初始化位置为原点，旋转为单位四元数，缩放为1.0，方向向量为默认值
    Transform() 
        : position(0.0f),
		rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		scale(1.0f),
		forward(0.0f, 0.0f, -1.0f),
		right(1.0f, 0.0f, 0.0f),
		up(0.0f, 1.0f, 0.0f),
		gravityDirection(0.0f, -1.0f, 0.0f)
    {}
	/// @brief 带参数的构造函数
	/// @details 初始化位置、旋转和缩放，并计算方向向量
	/// @param pos [IN] 位置向量
	/// @param rot [IN] 旋转四元数
	/// @param scl [IN] 缩放向量
	Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl)
		: position(pos), rotation(rot), scale(scl) {
		updateDirectionVectors();  // 初始化方向向量
	}

	/// @brief 获取模型矩阵
	/// @details 模型矩阵是通过位置、旋转和缩放计算得出的，通常用于渲染实体在世界空间中的变换。
	/// @return 返回一个4x4的模型矩阵，用于渲染和物理计算
    glm::mat4 getModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);  // 初始化为单位矩阵
		model = glm::translate(model, position);    // 应用位置变换
		model = model * glm::toMat4(rotation);  // 应用旋转变换
		model = glm::scale(model, scale);   // 应用缩放变换
        return model;
    }

	/// @brief 更新方向向量
	/// @details 根据当前的旋转四元数更新前、右、上方向向量
    void updateDirectionVectors() {
		forward = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, -1.0f));  // 前向量
		right = glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 0.0f)); // 右向量
		up = glm::rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));    // 上向量
    }
};