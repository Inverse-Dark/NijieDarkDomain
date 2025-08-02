#pragma once
#include <string>
#include <glm/glm.hpp>

/// @brief 着色器封装类
/// @details 该类用于加载和管理着色器程序，提供基本的着色器操作接口
/// 
/// 设计思路：
/// 1. 封装OpenGL着色器程序的创建和管理
/// 2. 提供统一接口设置着色器参数
/// 3. 支持错误检查和日志
/// 
/// 为何这样做：
/// - 简化着色器使用流程
/// - 减少OpenGL API调用
/// - 提供类型安全的参数设置
class Shader
{
public:
	/// @brief 构造函数
	/// @details 加载顶点和片段着色器文件，编译并链接成着色器程序
	/// @param vertexPath [IN] 顶点着色器文件路径
	/// @param fragmentPath [IN] 片段着色器文件路径
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	/// @brief 析构函数
	/// @details 删除着色器程序，释放OpenGL资源
	~Shader();

	/// @brief 使用着色器程序
	/// @details 绑定着色器程序到OpenGL上下文，准备渲染
	void use() const;

	/// @brief 设置布尔型着色器参数
	/// @details 设置着色器中的布尔型变量
	/// @param name [IN] 着色器变量名称
	/// @param value [IN] 布尔值
	void setBool(const std::string& name, bool value) const;
	/// @brief 设置整数型着色器参数
	/// @details 设置着色器中的整数型变量
	/// @param name [IN] 着色器变量名称
	/// @param value [IN] 整数值
	void setInt(const std::string& name, int value) const;
	/// @brief 设置浮点型着色器参数
	/// @details 设置着色器中的浮点型变量
	/// @param name [IN] 着色器变量名称
	/// @param value [IN] 浮点值
	void setFloat(const std::string& name, float value) const;
	/// @brief 设置四维矩阵型着色器参数
	/// @details 设置着色器中的四维矩阵型变量
	/// @param name [IN] 着色器变量名称
	/// @param value [IN] 四维矩阵值
	void setMat4(const std::string& name, const glm::mat4& value) const;
	/// @brief 设置三维向量型着色器参数
	/// @details 设置着色器中的三维向量型变量
	/// @param name [IN] 着色器变量名称
	/// @param value [IN] 三维向量值
	void setVec3(const std::string& name, const glm::vec3& value) const;

	/// @brief 获取着色器程序ID
	/// @details 该ID用于OpenGL API调用
	/// @return 着色器程序ID
	unsigned int getID() const { return m_ID; }

private:
	/// @brief 检查着色器编译错误
	/// @details 如果编译失败，将输出错误日志
	/// @param shader [IN] 着色器ID
	/// @param type [IN] 着色器类型（顶点着色器或片段着色器）
	void checkCompileErrors(unsigned int shader, const std::string& type);

private:
	unsigned int m_ID; /// 着色器程序ID
};