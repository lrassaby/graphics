//fragment shader, create a file for it named fragment.frag
#version 120

uniform sampler2D brick_image;  // The brick texture
uniform sampler2D bump_image; // The bump map
uniform vec3 lightVector;


void main()
{
	// Step 1:
	// Extract normals from the normal map

	// Extract the normal from the normal map
	// Need to do the 2.0*-1.0 so we can get normals in range of 0.0-2.0
	// and because normals can be back facing.
	vec3 normal = normalize(texture2D(bump_image,gl_TexCoord[0].st).rgb * 2.0 - 1.0);

	// Determine where the light is positioned
	// You get to set this
	vec3 light_pos = normalize(vec3(lightVector.x,lightVector.y,lightVector.z));

	// Calculate the lighting diffuse value
	float diffuse = max(dot(normal,light_pos),0.0);

	// Step 2:
	// Multiply lighting times original texture color to get depth
	vec3 final_color = diffuse * texture2D(brick_image, gl_TexCoord[0].st).rgb;

	// Step 3:
	// Find the final color

	// Finally set the output color of our pixel
	gl_FragColor = vec4(final_color, 1.0);
}






