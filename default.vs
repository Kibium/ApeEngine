#version 330 

layout(location = 0) in vec3 vertex_position;
uniform mat4 transform;

void main(){   

	gl_Position = transform * vec4(vertex_position, 1.0);

}