#version 400 core 

// Note: colour is smoothly interpolated (default)
out vec4 vOutputColour;

in vec4 p;
in vec3 n;
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

// Structure holding spotlight information:  its position as well as ambient, diffuse, and specular colours; also the direction, exponent and cutoff of the spotlight
struct SpotLightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
};

// Structure holding ambient light information:  its position as well as ambient, diffuse, and specular colours
struct AmbientLightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
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
// Lights and materials passed in as uniform variables from client programme
uniform AmbientLightInfo light1; 
uniform SpotLightInfo lights[3];
uniform MaterialInfo material1; 
//Skybox variables
uniform samplerCube CubeMapTex;
uniform bool renderSkybox;
in vec3 worldPosition;
//Day/night mode
uniform bool dayMode;
//Water
uniform bool renderWater;
in float d; //noise distorsion

//Fog variables
float rho;
vec3 fogColour;

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light1.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 n = eyeNorm;
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light1.Ls * material1.Ms * pow(max(dot(r, v), 0.0f), material1.shininess + eps);
	

	return ambient + diffuse + specular;

}

// This function implements the Blinn-Phong shading model
//The code is based on the OpenGL Shading Language Cookbook, p. 105.  
vec3 BlinnPhongSpotlightModel(SpotLightInfo light, vec4 p, vec3 n)
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

float random( vec3 scale, float seed ){
  return fract( sin( dot( gl_FragCoord.xyz + seed, scale ) ) * 43758.5453 + seed ) ;
}

void main()
{	

	// Vertex colour
	vec3 vColour;

	if (renderSkybox) {

		// Apply the Phong model to compute the vertex colour
		vColour = PhongModel(p, n);	
		vOutputColour = texture(CubeMapTex, worldPosition)*vec4(vColour, 1.0f);

	}else if(renderWater){
		
		// get a random offset
		float r = .01 * random( vec3( 12.9898, 78.233, 151.7182 ), 0.0 );
		// lookup vertically in the texture, using noise and offset
		// to get the right RGB colour
		float y  = abs(d) + r;
		if(abs(d)<=0){
			y = 0.1+r; //The random offset helps to diffuse the colour
		}
		vec2 tPos = vec2( 0, y);
		vec4 vTexColour = texture2D(sampler0, tPos);

		if(dayMode){
			// Apply the Phong model to compute the vertex colour
			vColour = PhongModel(p, n);		
		} else {
			// Apply the Blinn-Phong model to compute the vertex colour per spotlight, and add them to get the value of all together
			for(int i=0; i<lights.length(); i++){
				vColour += BlinnPhongSpotlightModel(lights[i], p, n);
			}
		}

		vOutputColour = vTexColour*vec4(vColour, 0.75);	// Combine object colour and texture 

	} else {

		// Get the texel colour from the texture sampler
		vec4 vTexColour = texture(sampler0, vTexCoord);

		if(dayMode){
			// Apply the Phong model to compute the vertex colour
			vColour = PhongModel(p, n);		
		} else {
			// Apply the Blinn-Phong model to compute the vertex colour per spotlight, and add them to get the value of all together
			for(int i=0; i<lights.length(); i++){
				vColour += BlinnPhongSpotlightModel(lights[i], p, n);
			}
		}

		if (bUseTexture)
			vOutputColour = vTexColour*vec4(vColour, 1.0);	// Combine object colour and texture 
		else
			vOutputColour = vec4(vColour, 1.0);	// Just use the colour instead

		// Use mix to linearly interpolate fog
		if(dayMode){
			//Fog variables
			rho = 0.004f;
			fogColour = vec3(16/256.0,49/256.0,70/256.0);//vec3(0.0f, 0.48828f, 0.734375f);
		} else {
			rho = 0.002f;
			fogColour = vec3(0.439216f, 0.858824f, 0.576471f);
		}
		float d = length(p.xyz);
		float w = exp(-rho*d);

		vOutputColour.rgb = mix(fogColour, vOutputColour.rgb, w);
	}
}