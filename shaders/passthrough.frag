#version 330 core
out vec4 fragColor;

uniform vec4 ourColor; // we set this variable in the openGL code.

void main()
{
	fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	fragColor = ourColor;	
}