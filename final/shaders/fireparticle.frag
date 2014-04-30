#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec4 particlecolor;
varying float frame;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
    vec2 xy;
    xy.x = frame * 16 + UV.x;
    xy.y = mod(frame, 8) * 128 + UV.y;
    gl_FragColor = vec4(texture2D( myTextureSampler, xy ).rgb, 0.9);    
}
