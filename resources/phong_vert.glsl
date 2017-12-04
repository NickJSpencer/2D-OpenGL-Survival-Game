#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 MV;
uniform mat4 View;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 fragNor;
out vec3 fragPos;
out vec3 dif;
out vec3 l;
out vec3 ambient;
out vec3 toCam;

void main()
{
	gl_Position = P * View * MV * vertPos;

	/* Pass the positions and normals to the fragment shader */
	fragNor = (MV * vec4(vertNor, 0.0)).xyz;
	fragPos = (MV * vertPos).xyz;

	toCam = viewPos - fragPos;

}
