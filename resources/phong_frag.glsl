#version 330 core 
in vec3 fragNor;
in vec3 fragPos;
in vec3 toCam;

out vec4 color;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	/* Normalize the fragment's normal */
	vec3 normal = normalize(fragNor);

	/* Get the normal vector from the fragment to the light */
	vec3 toLight = normalize(lightPos - fragPos);

	vec3 toCam = normalize(toCam);

	/* Get the diffuse component using the material's diffuse properties, the fragment's normal, 
	 * and the vector from the fragment to the light */
	vec3 diffuse = MatDif * max(dot(normal, toLight), 0.0);

	/* Calculate R, which is the reflectance vector, using the vector from the fragment to the light,
	 * and the fragment's normal; this is used to find the specular value */
	vec3 R = normalize((-toLight) + 2 * max(dot(toLight, normal), 0.0) * normal);

	/* Get the specular component using the material's specular properties, the fragment's position,
	 * the reflectance vector, and the material's shiny property */
	vec3 spec = MatSpec * pow(max(dot(toCam, R), 0.0), shine);

	/* Set the color of the fragment to the sum of the diffuse, specular, and ambiant lightings */
	color = vec4(diffuse + MatAmb, 1.0);
}
