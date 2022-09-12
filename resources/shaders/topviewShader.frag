#version 400 core 

// Note: colour is smoothly interpolated (default)
out vec4 vOutputColour;

in vec4 p;
in vec3 n;
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

uniform sampler2D sampler0;  // The texture sampler
uniform bool renderPath;
uniform bool renderSubmarine;

void main()
{	
	if(renderPath){
		vOutputColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);	
	} else if(renderSubmarine){
		vOutputColour = vec4(0.81f, 0.71f, 0.23f, 1.0f);	
	} else {
		//Background
		vOutputColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);	
	}
}