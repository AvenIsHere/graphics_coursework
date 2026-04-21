#version 400

in  vec2 ex_TexCoord; //texture coord arriving from the vertex
in  vec3 ex_Normal;  //normal arriving from the vertex

out vec4 out_Color;   //colour for the pixel
in vec3 ex_LightDir;  //light direction arriving from the vertex

in vec3 ex_PositionEye;

uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

in vec3 ex_Color;

uniform sampler2D DiffuseMap;
uniform bool useTexture;

void main(void)
{
	//out_Color = texture(DiffuseMap, ex_TexCoord); //show texture values

	//out_Color = vec4(ex_Normal,1.0); //Show normals

	//out_Color = vec4(ex_TexCoord,0.0,1.0); //show texture coords

	//Calculate lighting
	vec3 normal, light_angle;
	vec4 color;
	float angle_from_light;

	normal = normalize(ex_Normal);
	light_angle = normalize(ex_LightDir);

	vec3 v = normalize(-ex_PositionEye);
	vec3 r = normalize(-reflect(light_angle, normal));

	float RdotV = max(0.0, dot(r, v));

	angle_from_light = max(dot(normal, light_angle), 0.0);

	color = light_ambient * material_ambient;

	vec4 updated_diffuse = material_diffuse;
	if (!useTexture) {
		updated_diffuse = material_diffuse * vec4(ex_Color, 1.0);
	}

	if(angle_from_light > 0.0)
	{
		color += (light_diffuse * updated_diffuse * angle_from_light);
	}

	color += material_specular * light_specular * pow(RdotV, material_shininess);


	if (!useTexture) {
		out_Color = color;  //show just lighting
	}
	//out_Color = texture(DiffuseMap, ex_TexCoord); //show texture only
	else {
		out_Color = color * texture(DiffuseMap, ex_TexCoord); //show texture and lighting
	}
}