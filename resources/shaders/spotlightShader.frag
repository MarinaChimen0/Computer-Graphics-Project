#version 400 core 

// Note: colour is smoothly interpolated (default)
out vec4 vOutputColour;

in vec4 p;
in vec3 n;
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
};

struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

uniform sampler2D sampler0;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform LightInfo light1; 
uniform LightInfo light2;
uniform MaterialInfo material1; 

//Fog variables
float rho = 0.002f;
vec3 fogColour = vec3(0.439216f, 0.858824f, 0.576471f);

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, pp. 67 - 68, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light1.position - p));
	vec3 v = normalize(-p.xyz);
	vec3 r = reflect(-s, n);
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0);
	vec3 h = normalize(v + s);
	if (sDotN > 0.0)
		specular = light1.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
	
	return ambient + diffuse + specular;

}

vec3 BlinnPhongSpotlightModel(LightInfo light, vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light.position - p));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.Ma;
	if (angle < cutoff) {
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(-p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = light.Ld * material1.Md * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
			specular = light.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
		return ambient + spotFactor * (diffuse + specular);
	} else
		return ambient;
}


void main()
{	
	// Get the texel colour from the texture sampler
	vec4 vTexColour = texture(sampler0, vTexCoord);	
	vec3 vColour1 = BlinnPhongSpotlightModel(light1, p, normalize(n));
	vec3 vColour2 = BlinnPhongSpotlightModel(light2, p, normalize(n));
	
	// Use mix to linearly interpolate fog
	float d = length(p.xyz);
	float w = exp(-rho*d);

	if (bUseTexture)
			vOutputColour = vTexColour*vec4(vColour1 + vColour2, 1.0);	// Combine object colour and texture 
		else
			vOutputColour = vec4(vColour1 + vColour2, 1.0);	// Just use the colour instead

	vOutputColour.rgb = mix(fogColour, vOutputColour.rgb, w);
}