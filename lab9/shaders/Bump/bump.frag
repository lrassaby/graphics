//fragment shader, create a file for it named fragment.frag
#version 120

uniform vec3 lightVector;
uniform sampler2D bump_image;
uniform sampler2D brick_image;

void main()
{
	// TODO Step 1:
	// Extract normals from the normal map
    vec3 normal = texture2D(bump_image, gl_TexCoord[0].st).rgb;
    normal = normalize(normal);
    //normal = vec3(0.0, 0.0, 1.0);
    vec3 light_normalized = normalize(lightVector);

	// TODO Step 2:
	// Multiply lighting times original texture color to figure out how much light we receive
    float f = dot(light_normalized, normal); 

	// TODO Step 3:
	// Find the final color
	gl_FragColor = vec4(f * texture2D(brick_image, gl_TexCoord[0].st).rgb, 1.0);
}






