#version 330 core
//法向量
in vec3 normal;
in vec3 FragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

//  镜面反射需要用到摄像机的视角，所以需要传入一个摄像机的位置
uniform vec3 viewPos;

void main(){
    vec3 objectColor = vec3(texture(texture_diffuse1, texCoord));
    //  环境光照：将环境光强度乘以光的颜色，就可以得到环境光的颜色
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //  漫反射
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);  //光线的方向向量，将光源位置向量与片元位置向量相减就可以得到
    float diff = max(dot(norm, lightDir), 0.0);     //点积，获得cos值
    vec3 diffuse = diff * lightColor;

    //  镜面反射
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - FragPos);    //摄像机的观察方向向量，将摄像机位置向量与片元位置向量相减就可以得到
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(texture(texture_specular1, texCoord));

    //  合成
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f) ;
}


