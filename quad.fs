#version 330 

out vec4 color;
uniform vec4 myColor;
in vec2 uv0;
uniform sampler2D texture0;

void main(){    

	//color = myColor;
	color = texture2D(texture0, uv0);
	}