#version 330 core

in vec3 color;
out vec4 fColor;

uniform float time;

void main()
{
    vec3 tmpColor = color;
    tmpColor.x *= abs(sin(time));
    tmpColor.y *= abs(cos(time));

    fColor = vec4(tmpColor, 1.0f);
}