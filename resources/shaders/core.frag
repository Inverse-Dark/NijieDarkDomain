#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;

uniform float time;

void main() {
    // 基础颜色
    vec3 baseColor = Color;
    
    // 简单的动态光照效果
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.7));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    
    // 环境光
    vec3 ambient = vec3(0.2);
    
    // 最终颜色
    vec3 result = (ambient + diffuse) * baseColor;
    
    // 添加脉动效果（用于测试）
    float pulse = (sin(time * 2.0) + 1.0) * 0.1 + 0.8;
    result *= pulse;
    
    FragColor = vec4(result, 1.0);
}