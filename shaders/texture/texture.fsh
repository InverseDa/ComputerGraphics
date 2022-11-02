#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(){
    FragColor = texture(ourTexture, TexCoord);    //参数一为纹理，参数二为纹理坐标
}