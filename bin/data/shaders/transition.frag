//#extension GL_ARB_texture_rectangle : enable
#version 120

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
varying vec2 texCoord;
uniform vec2 ratio;
uniform float crossfade;


void main( void )

{
	vec4 col1 = texture2DRect(tex1,texCoord);
	vec4 col2 = texture2DRect(tex2,texCoord*ratio);

	gl_FragColor = mix(col1, col2, crossfade);
}
