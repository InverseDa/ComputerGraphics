#version 330 core
in vec3 aPos;
in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    texCoord = aTexCoord;
}