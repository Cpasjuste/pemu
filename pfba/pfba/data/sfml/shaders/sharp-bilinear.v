#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX2;

uniform mat4 MVPMatrix;
uniform COMPAT_PRECISION vec2 TextureSize;

void main()
{
	gl_Position = MVPMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	TEX0.xy = gl_TexCoord[0].xy;
	TEX2.xy = 6.28318548E+00*TextureSize;
}

