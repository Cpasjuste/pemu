#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out vec4 FragColor;
#else
#define COMPAT_VARYING varying
#define FragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif
COMPAT_VARYING     vec4 _t4;
COMPAT_VARYING     vec4 _t3;
COMPAT_VARYING     vec4 _t21;
COMPAT_VARYING     vec4 _t11;
COMPAT_VARYING     vec2 _CT;
COMPAT_VARYING     vec4 _color1;
COMPAT_VARYING     vec4 _t41;
COMPAT_VARYING     vec4 _t31;
COMPAT_VARYING     vec4 _t22;
COMPAT_VARYING     vec4 _t12;
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     vec4 _color;
COMPAT_VARYING     float _frame_rotation;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct VERTEX_OUTPUT {
    vec4 _color;
    vec2 _texCoord;
    vec4 _t12;
    vec4 _t22;
    vec4 _t31;
    vec4 _t41;
};
struct output_dummy {
    vec4 _color1;
};
struct VERTEX_INPUT {
    vec2 _CT;
    vec4 _t11;
    vec4 _t21;
    vec4 _t3;
    vec4 _t4;
};
float _TMP20;
vec3 _TMP19;
float _TMP18;
vec3 _TMP17;
float _TMP16;
vec3 _TMP15;
float _TMP14;
vec3 _TMP13;
float _TMP12;
vec3 _TMP11;
float _TMP10;
vec3 _TMP9;
vec4 _TMP8;
vec4 _TMP7;
vec4 _TMP6;
vec4 _TMP5;
vec4 _TMP4;
vec4 _TMP3;
vec4 _TMP2;
vec4 _TMP1;
vec4 _TMP0;
uniform sampler2D Texture;
vec3 _a0045;
vec3 _a0049;
vec3 _a0053;
vec3 _a0057;
vec3 _a0061;
vec3 _a0065;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
COMPAT_VARYING vec4 TEX2;
COMPAT_VARYING vec4 TEX3;
COMPAT_VARYING vec4 TEX4;
uniform vec3 _dt;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    output_dummy _OUT;
    float _d1;
    float _d2;
    float _hl;
    float _vl;
    float _k1;
    float _k2;
    vec3 _t1;
    vec3 _t2;
    vec3 _TMP25;
    _TMP0 = COMPAT_TEXTURE(Texture, TEX1.zw);
    _TMP1 = COMPAT_TEXTURE(Texture, TEX3.xy);
    _TMP2 = COMPAT_TEXTURE(Texture, TEX3.zw);
    _TMP3 = COMPAT_TEXTURE(Texture, TEX1.xy);
    _TMP4 = COMPAT_TEXTURE(Texture, TEX0.xy);
    _TMP5 = COMPAT_TEXTURE(Texture, TEX2.xy);
    _TMP6 = COMPAT_TEXTURE(Texture, TEX2.zw);
    _TMP7 = COMPAT_TEXTURE(Texture, TEX4.xy);
    _TMP8 = COMPAT_TEXTURE(Texture, TEX4.zw);
    _a0045 = _TMP0.xyz - _TMP8.xyz;
    _TMP9 = abs(_a0045);
    _TMP10 = dot(_TMP9, _dt);
    _d1 = _TMP10 + 9.99999975E-05;
    _a0049 = _TMP2.xyz - _TMP6.xyz;
    _TMP11 = abs(_a0049);
    _TMP12 = dot(_TMP11, _dt);
    _d2 = _TMP12 + 9.99999975E-05;
    _a0053 = _TMP3.xyz - _TMP5.xyz;
    _TMP13 = abs(_a0053);
    _TMP14 = dot(_TMP13, _dt);
    _hl = _TMP14 + 9.99999975E-05;
    _a0057 = _TMP1.xyz - _TMP7.xyz;
    _TMP15 = abs(_a0057);
    _TMP16 = dot(_TMP15, _dt);
    _vl = _TMP16 + 9.99999975E-05;
    _k1 = 5.00000000E-01*(_hl + _vl);
    _k2 = 5.00000000E-01*(_d1 + _d2);
    _t1 = (_hl*(_TMP1.xyz + _TMP7.xyz) + _vl*(_TMP3.xyz + _TMP5.xyz) + _k1*_TMP4.xyz)/(2.50000000E+00*(_hl + _vl));
    _t2 = (_d1*(_TMP2.xyz + _TMP6.xyz) + _d2*(_TMP0.xyz + _TMP8.xyz) + _k2*_TMP4.xyz)/(2.50000000E+00*(_d1 + _d2));
    _a0061 = _t1 - _TMP4.xyz;
    _TMP17 = abs(_a0061);
    _TMP18 = dot(_TMP17, _dt);
    _k1 = _TMP18 + 9.99999975E-05;
    _a0065 = _t2 - _TMP4.xyz;
    _TMP19 = abs(_a0065);
    _TMP20 = dot(_TMP19, _dt);
    _k2 = _TMP20 + 9.99999975E-05;
    _TMP25 = (_k1*_t2 + _k2*_t1)/(_k1 + _k2);
    _OUT._color1 = vec4(_TMP25.x, _TMP25.y, _TMP25.z, 1.00000000E+00);
    FragColor = _OUT._color1;
    return;
} 
