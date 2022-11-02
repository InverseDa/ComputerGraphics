#version 460 core
//  法向量
in vec3 normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec2 staticTexCoord;

out vec4 FragColor;

//  材质结构体
struct Material{
    //  环境光与漫反射基本相似，所以合并成一个，我们将纹理的材质当作漫反射，这样就有了材质的基本颜色
    //  换言之 漫反射贴图就是纹理原有的贴图，它的颜色信息充当着漫反射信息
    sampler2D diffuse;
    //  镜面光可以用高光贴图代替，使用原理与漫反射相同
    sampler2D specular;
    float shininess;
};

//  光强结构体，控制光照的强度而已
struct Light{
    vec3 Position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 lightPos;

//  镜面反射需要用到摄像机的视角，所以需要传入一个摄像机的位置
uniform vec3 viewPos;

//  材质
uniform Material material;

//  光强
uniform Light light;

uniform sampler2D emission;
uniform float time;

void main(){
    //  环境光照：将环境光强度乘以光的颜色，就可以得到环境光的颜色
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, staticTexCoord));

    //  漫反射
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);//光线的方向向量，将光源位置向量与片元位置向量相减就可以得到
    float diff = max(dot(norm, lightDir), 0.0);//点积，获得cos值
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoord))) * light.diffuse;

    //  发光
    vec3 specular = light.specular * (4.5f * abs(sin(time * 0.5f)) * vec3(texture(material.specular, TexCoord)));

    //  合成
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}


