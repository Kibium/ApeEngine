#version 330 

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv0;

uniform mat4 transform;
out vec2 uv0;

void main(){   

	gl_Position = transform *vec4(vertex_position, 1.0);
	uv0 = vertex_uv0;
}