#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec4 particlecolor;
varying float frame;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
    vec2 xy;
//     float newframe = 4;
//     //xy.x = (mod(newframe, 8.0) * 128.0 + UV.x * 128.0) / 8.0;
//     //xy.y = (newframe * 16.0 + UV.y * 128.0) / 8.0;
//      xy.x = (128 * ((mod(newframe, 8.0) / 8.0) ) / 1024.0;
//      xy.y = (128 * ((newframe / 8.0) / 8.0)) / 1024.0;
//     vec3 color = texture2D( myTextureSampler, xy ).rgb;
//     if (color.r < 0.1 && color.g < 0.1 && color.b < 0.1) {
//         gl_FragColor = vec4(color, 0.0); 
//     } else {
//         gl_FragColor = vec4(color, 1.0);   
//     }
// =
    xy.x = frame * 16 + UV.x;
    xy.y = mod(frame, 8) * 128 + UV.y;
    gl_FragColor = vec4(texture2D( myTextureSampler, xy ).rgb, 0.9);    
}
