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
//聚光灯的数据结构
struct Light{
//位置
    vec3 position;
//方向
    vec3 direction;
//切光角
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

//  镜面反射需要用到摄像机的视角，所以需要传入一个摄像机的位置
uniform vec3 viewPos;

void main(){
    vec3 lightDir = normalize(light.position - FragPos);
    //  环境光照：将环境光强度乘以光的颜色，就可以得到环境光的颜色
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

    //  漫反射
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);//点积，获得cos值
    vec3 diffuse = (diff * vec3(texture(material.diffuse, texCoord))) * light.diffuse;

    //  镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);//摄像机的观察方向向量，将摄像机位置向量与片元位置向量相减就可以得到
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoord)));

    //  计算角度的余弦值，与切光角比较，如果小于切光角那么就渲染光照、否则不渲染
    float theta = dot(lightDir, normalize(-light.direction));
    //  平滑边缘
    //  I = (theta - gamma) / epsilion，其中epsilion = fiv - gamma
    float epsilion = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilion, 0.0f, 1.0f);

    diffuse *= intensity;
    specular *= intensity;
    //  合成
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);

}