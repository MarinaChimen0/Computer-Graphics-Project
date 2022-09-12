#version 400 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

// Vertex colour output to fragment shader for Gouraud shading
out vec4 p;
out vec3 n;
// Vertex colour output to fragment shader
out vec2 vTexCoord;	// Texture coordinate
out vec3 worldPosition;	// used for skybox

//Water variables
uniform bool renderWater;
uniform float time;
out float d; //noise distorsion

//Walls variables
uniform bool renderWalls;

//Sand variables
uniform bool renderSand;

//Rock variables
uniform float rockScale;
uniform bool renderRockA;

//uniform bool renderRockB;
//vec3 posB[10] = vec3[](vec3(1227.648071f, 3.255793f, 95.899269f), vec3(1025.936890f, 7.039076f, 1003.559265f),
//				vec3(1302.836060f, 5.000000f, 1556.410522f), vec3(1500.870483f, 5.000000f, 2677.173096f), 
//				vec3(550.165771f, 5.325739f, 2147.090820f), vec3(-498.718628f, -63.083366f, 1785.200195f),
//				vec3(-221.790237f, -36.552441f, 2002.956787f), vec3(-1004.217529f, 25.180920f, 925.917542f),
//				vec3(-404.987152f, 17.990131f, 288.178833f), vec3(246.254883f, 3.571141f, -45.833736f));
uniform bool renderRockC;

uniform bool renderRockD;

uniform bool renderRockE;


// CLASSIC 3D PERLIN NOISE FROM https://github.com/ashima/webgl-noise/blob/master/src/classicnoise3D.glsl//
//
// GLSL textureless classic 3D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@liu.se)
// Version: 2011-10-11
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/stegu/webgl-noise
//

vec3 mod289(vec3 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec3 P)
{
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}

// Classic Perlin noise, periodic variant
float pnoise(vec3 P, vec3 rep)
{
  vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
  vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}


// END //



//Displace each vertex a distance d along its normal using Perlin noise
float perlin(vec3 vertex){
  float w = 100.0;
  float t = -.5;

  for (float f = 1.0 ; f <= 10.0 ; f++ ){
    float power = pow( 2.0, f );
    t += abs( pnoise( vec3( power * vertex), vec3( 10.0, 10.0, 10.0) ) / power );
  }

  return t;
}

void main()
{	
	// Save the world position for rendering the skybox
	worldPosition = inPosition;

	vec3 pos = inPosition;

	if(renderWater){

		//Displace each vertex a distance d along its normal using Perlin noise
		//d =    perlin(0.5*pos+time*0.025);
		//d = 5.0 * pnoise( 0.05 * pos.xy + vec2( 2.0 * time*0.025 ), vec2( 100.0 ) ) - 10.0 *d;
		d = cnoise(0.1 * pos+time*0.1); // 0.5 * cnoise(0.75 * pos+time*0.25); + 0.5 * cnoise(vec2(4*pos.x,2*pos.y,pos.z)+time*0.25);
		pos = pos + inNormal * d;

	} else if(renderWalls){

		//Displace each vertex a distance d along its normal using Perlin noise
		//d =  5.0*  perlin(0.5*pos+time*0.025);
		//d = 5.0 * pnoise( 0.05 * pos.xy + vec2( 2.0 * time*0.025 ), vec2( 100.0 ) ) - 10.0 *d;
		d =    cnoise(2 * pos) + 2*cnoise(vec3(4*pos.x,2*pos.y, pos.z));
		pos = pos + inNormal * abs(d);

	} else if(renderSand){

		d =  3*cnoise(0.5 * pos);
		pos = pos + inNormal * abs(d);

	} else if(renderRockA){

		vec3 posA[10] = vec3[](vec3(873.624512f, 4.810508f, -26.400465f), vec3(1531.706421f, 17.560579f, 606.644714f),
				vec3(438.523590f, -14.528829f, 1204.209595f), vec3(555.962891f, 6.798330f, 1565.175903f), 
				vec3(1728.013306f, 5.000000f, 2226.925049f), vec3(700.572266f, 5.737399f, 2330.748291f),
				vec3(73.245956f, -1.213879f, 1738.892456f), vec3(-184.137146f, -2.358431f, 1756.539307f),
				vec3(-431.467255f, 30.414373f, 1356.399170f), vec3(-665.115173f, 25.934746f, 560.476746f));
		pos = pos + posA[gl_InstanceID]/rockScale;

	} else if(renderRockC){

		vec3 posC[10] = vec3[](vec3(623.466553f, 4.060357f, -75.448341f), vec3(1500.844116f, 49.944080f, 1097.953247),
				vec3(650.873474, -18.815233, 958.710205), vec3(1054.558472f, 4.995919f, 1600.697876), 
				vec3(1639.191772f, 5.000000f, 1856.910767f), vec3(1170.262329f, 5.065478f, 2768.829834),
				vec3(299.571106f, 3.327008f, 1892.371826f), vec3(-590.278748f, -83.529861f, 2137.438232f),
				vec3(-879.161499f, 36.702816f, 1120.569824f), vec3(-115.698029f, 7.162012f, 49.071007f));
		pos = pos + posC[gl_InstanceID]/rockScale;

	} else if(renderRockD){

		vec3 posD[10] = vec3[](vec3(1115.389038, 0.502003, 65.936493), vec3(1483.617432, 0.790963, 366.540161),
				vec3(1263.640625, 25.391977, 1067.369141), vec3(803.609314f, 1.223301f, 1588.468018f), 
				vec3(1640.337402, 0.000000, 2466.988037), vec3(910.072693, 0.607118, 2518.150391),
				vec3(-287.545197, -40.165909, 1683.459717), vec3(-351.697754, -70.539474, 2224.338379),
				vec3(-220.977585, 18.909378, 1514.444824), vec3(-857.451843, 20.356800, 744.549500));
		pos = pos + posD[gl_InstanceID]/rockScale;

	} else if(renderRockE){

		vec3 posE[10] = vec3[](vec3(497.746826, 0.730947, -56.333618), vec3(1580.294800, 30.003113, 858.630127),
				vec3(478.687744, -20.206961, 1077.801880), vec3(929.448914, 0.237438, 1574.761475), 
				vec3(1508.359497, 0.000000, 1631.927124), vec3(1405.232422, 0.000000, 2740.204834),
				vec3(363.576294, 0.090534, 1977.103271), vec3(-544.021667, -80.858459, 2278.773926),
				vec3(-655.671631, 25.626198, 1261.074219), vec3(-509.438385, 26.582850, 381.154083));
		pos = pos + posE[gl_InstanceID]/rockScale;

	}

	// Transform the vertex spatial position using the projection and modelview matrices
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(pos, 1.0);

	// Get the vertex normal and vertex position in eye coordinates
	n = normalize(matrices.normalMatrix * inNormal);
	p = matrices.modelViewMatrix * vec4(pos, 1.0f);

	// Pass through the texture coordinate
	vTexCoord = inCoord;

} 