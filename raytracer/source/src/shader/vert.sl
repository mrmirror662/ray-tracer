#version 330 core

layout(location = 0) in vec2 pos;
out vec2 posv;
void main()
{
posv = pos;
gl_Position.xy = pos;
gl_Position.z = 0;
}