// Fragment Shader 

#version 430

in  vec3 ex_Color;  //colour arriving from the vertex shader
out vec4 out_Color; //colour for the pixel to be sent out of the fragment shader

void main(void)
{
	//use to see the depth values in range 0.0 to 1.0 based on the z coordinate
	//objects close to the near clipping plane will have value of 0.0 and objects further away will be closer to 1.0.
	//however, the depth value is not linear so most of the scene is likely to appear white 
	//unless you move the near clipping plane away from the viewer.
	//out_Color = vec4(vec3(gl_FragCoord.z),1.0);
	
	out_Color = vec4(ex_Color,1.0);
}