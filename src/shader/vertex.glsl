#version 440 core

layout (location = 0) in vec3 pos;

uniform mat4 projection_view_model;

void main()
{
    gl_PointSize = 3.0;
    gl_Position = projection_view_model * vec4(pos, 1.0);
}

