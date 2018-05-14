#version 330 core

out vec4 color;

in float h;

void main()
{
    color = vec4( h*0.9f, h*0.7f, h*0.3f, 1.0f);
} 