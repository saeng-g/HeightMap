
#version 330 core
  
layout (location = 0) in vec3 position;

uniform mat4 m_mat; //model matrix
uniform mat4 v_mat; //view matrix
uniform mat4 p_mat; //projection matrix

out float h;

void main()
{
    gl_Position = m_mat * vec4(position.x, position.y, position.z, 1.0);
    h = position.z;
}