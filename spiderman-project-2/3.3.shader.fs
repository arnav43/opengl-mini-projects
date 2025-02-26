#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue; // Declare mixValue as a uniform
uniform float getTime;

void main(){
	float scaleFactor = cos(getTime)*0.5+0.5;
	
	FragColor = mix(texture(texture1, TexCoord),
					texture(texture2, vec2(scaleFactor-TexCoord.x, TexCoord.y))*vec4(ourColor,1)*scaleFactor, mixValue);


}