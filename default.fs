#version 330 

out vec4 color;
uniform vec4 myColor;
in vec2 texCoord;
uniform sampler2D myTexture;

void main(){    

	color = myColor;
	
	}