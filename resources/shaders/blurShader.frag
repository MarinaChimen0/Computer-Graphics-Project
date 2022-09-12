#version 400 core 

// Note: colour is smoothly interpolated (default)
out vec4 vOutputColour;

in vec4 p;
in vec3 n;
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

uniform sampler2D sampler0;  // The texture sampler
uniform bool renderBlur;
uniform bool renderMap;
uniform float speed;

const float sampleDist = 1.0;
const float sampleStrength = 2.0; 

void main()
{	
	if(!renderBlur){

		if(renderMap) {
			//Get the colour of the texture
			vec4 colour = texture2D(sampler0, vTexCoord);
			//if it is black, its the background, render transparent
			if(colour == vec4(0.0f,0.0f,0.0f,1.0f)){
				discard;
			} else {
				vOutputColour = colour;
			}
		}else{
			vOutputColour = texture2D(sampler0, vTexCoord);
		}

	}else{
		//code from https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene
		float samples[10];
		samples[0] = -0.08;
		samples[1] = -0.05;
		samples[2] = -0.03;
		samples[3] = -0.02;
		samples[4] = -0.01;
		samples[5] =  0.01;
		samples[6] =  0.02;
		samples[7] =  0.03;
		samples[8] =  0.05;
		samples[9] =  0.08;

		vec2 dir = 0.5 - vTexCoord; 
		float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
		dir = dir/dist; 

		vec4 color = texture2D(sampler0, vTexCoord);
		vec4 sum = color;

		for (int i = 0; i < 10; i++)
			sum += texture2D( sampler0, vTexCoord + dir * samples[i] * sampleDist );

		sum *= 1.0/11.0;
		float t = dist * sampleStrength * (speed*5);
	    t =	clamp( t ,0.0,1.0);

		vOutputColour = mix( color, sum, t );
	}
}