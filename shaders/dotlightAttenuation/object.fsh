#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;
in vec2 texCoord;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

//  衰减参数
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

//  镜面反射需要用到摄像机的视角，所以需要传入一个摄像机的位置
uniform vec3 viewPos;

void main(){
    //  距离和衰减程度的计算
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant
    + light.linear * distance + light.quadratic * (distance * distance));

    //  环境光照：将环境光强度乘以光的颜色，就可以得到环境光的颜色
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

    //  漫反射
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);//光线的方向向量，直接得到平行光的方向向量即可
    float diff = max(dot(norm, lightDir), 0.0);//点积，获得cos值
    vec3 diffuse = (diff * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

    //  镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);//摄像机的观察方向向量，将摄像机位置向量与片元位置向量相减就可以得到
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoord)));

    //  将衰减程度作用到环境光、漫反射、镜面反射
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //  合成
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}