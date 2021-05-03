#shader vertex

#version 400 core
layout (location = 0) in vec3 POSITION;

void main()
{
    gl_Position = vec4(POSITION, 1.0);
}

#shader pixel

#version 400 core

out vec4 out_Color;

void main()
{
    out_Color = vec4(0.5, 0.2, 0.8, 1.0);
} 
