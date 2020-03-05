#version 120

varying vec2 texCoord;

void main() {	
	
        //gl_TexCoord[0] = gl_MultiTexCoord0;	
        texCoord = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
        gl_Position = ftransform();	
} 
