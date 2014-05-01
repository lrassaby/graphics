#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying float frame;

uniform sampler2D myTextureSampler;

void main() {
    vec2 xy;
    /*
    for debugging 
	if (frame < 0.33) {
		gl_FragColor = vec4(255, 0, 0, 1.0);
	} else if (frame < 0.66) {
		gl_FragColor = vec4(0, 255, 0, 1.0);
	} else if (frame < 1.0 ){
		gl_FragColor = vec4(0, 0, 255, 1.0);
	} else {
		gl_FragColor = vec4(255, 255, 255, 1.0);
	}
	*/
    float i = floor(frame * 64.0);

    float row = floor(i / 8.0);
    float col = floor(mod(i, 8.0));

    xy.x = (UV.x / 8.0) +  row / 8.0;
    xy.y = (UV.y / 8.0) +  col / 8.0;
    vec3 color = texture2D( myTextureSampler, xy ).rgb;

    if (color.r < 0.1 && color.g < 0.1 && color.b < 0.1) {
        gl_FragColor = vec4(color, 0.0); 
    } else {
    	gl_FragColor = vec4(color, 0.8);
    }
}
