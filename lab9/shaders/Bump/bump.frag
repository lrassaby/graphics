//fragment shader, create a file for it named fragment.frag
#version 120

uniform sampler2D "brick.ppm"
uniform sampler2D "bump.ppm"

void main()
{
	// TODO Step 1:
	// Extract normals from the normal map

	// TODO Step 2:
	// Multiply lighting times original texture color to figure out how much light we receive


	// TODO Step 3:
	// Find the final color

	gl_frag_color = vec4(1.0, 0, 0, 0);
}






