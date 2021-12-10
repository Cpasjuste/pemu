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

COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX2;

uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;

vec4 _oPosition1;
vec4 _r0009;
vec4 VertexCoord;
vec4 TexCoord;
vec4 COLOR;

void main()
{
	VertexCoord = gl_Vertex;
	TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	COLOR = gl_Color;

	vec4 _oColor;
	vec2 _oTex;
	vec2 _omega;
	_r0009 = VertexCoord.x*MVPMatrix[0];
	_r0009 = _r0009 + VertexCoord.y*MVPMatrix[1];
	_r0009 = _r0009 + VertexCoord.z*MVPMatrix[2];
	_r0009 = _r0009 + VertexCoord.w*MVPMatrix[3];
	_oPosition1 = _r0009;
	_oColor = COLOR;
	_oTex = TexCoord.xy;
	_omega = 6.28318548E+00*TextureSize;
	gl_Position = _r0009;
	COL0 = COLOR;
	TEX0.xy = TexCoord.xy;
	TEX2.xy = _omega;
}
