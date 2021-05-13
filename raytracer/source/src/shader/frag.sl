#version 330 core

out vec3 col;
in vec2 posv;
uniform sampler2D ts;
void main()
{
    col = texture(ts,(posv+1)/2).rgb;
}