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
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
vec4 _ret_0;
vec2 _TMP1;
uniform sampler2D Texture;
input_dummy _IN1;
vec2 _TMP6;
vec2 _TMP10;
vec2 _c0017;
COMPAT_VARYING vec4 TEX0;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec2 _texel;
    vec2 _texel_floored;
    vec2 _center_dist;
    vec2 _f;
    vec2 _mod_texel;
    _texel = TEX0.xy*TextureSize;
    _texel_floored = floor(_texel);
    _TMP6 = fract(_texel);
    _center_dist = _TMP6 - 5.00000000E-01;
    _TMP1 = min(vec2( 3.75000000E-01, 3.75000000E-01), _center_dist);
    _TMP10 = max(vec2( -3.75000000E-01, -3.75000000E-01), _TMP1);
    _f = (_center_dist - _TMP10)*4.00000000E+00 + 5.00000000E-01;
    _mod_texel = _texel_floored + _f;
    _c0017 = _mod_texel/TextureSize;
    _ret_0 = COMPAT_TEXTURE(Texture, _c0017);
    FragColor = _ret_0;
    return;
} 
