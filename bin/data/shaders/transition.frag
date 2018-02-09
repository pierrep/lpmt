//#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

uniform float crossfade;


void main( void )

{

	vec4 col1 = texture2DRect(tex1,gl_TexCoord[0].st);
	vec4 col2 = texture2DRect(tex2,gl_TexCoord[0].st);

	gl_FragColor = mix(col1, col2, crossfade);
}
