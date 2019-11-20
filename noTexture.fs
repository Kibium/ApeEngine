#version 330 

out vec4 color;
in vec3 myColor;

void main(){    

	color = vec4(myColor, 1);
	//color = texture2D(texture0, uv0);
	}