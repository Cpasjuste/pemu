#ifdef GL_ES
precision mediump float;
#endif
varying     float _frame_rotation;

varying     vec2 _one;

varying     float _mod_factor;

varying     vec2 _texCoord;





struct tex_coords {

    vec2 _texCoord;

    float _mod_factor;

    vec2 _one;

};



struct input_dummy {

    vec2 _video_size;

    vec2 _texture_size;

    vec2 _output_dummy_size;

    float _frame_count;

    float _frame_direction;

    float _frame_rotation;

};



vec4 _ret_0;

float _TMP45;

float _TMP44;

float _TMP43;

float _TMP24;

float _TMP42;

float _TMP41;

float _TMP40;

float _TMP39;

vec4 _TMP37;

float _TMP33;

float _TMP32;

float _TMP31;

float _TMP30;

vec4 _TMP36;

float _TMP49;

float _TMP48;

float _TMP47;

float _TMP46;

vec4 _TMP35;

vec4 _TMP34;

vec4 _TMP21;

vec4 _TMP19;

vec4 _TMP17;

vec4 _TMP15;

vec4 _TMP20;

vec4 _TMP18;

vec4 _TMP16;

vec4 _TMP14;

vec4 _TMP12;

vec4 _TMP10;

vec4 _TMP8;

vec4 _TMP6;

vec4 _TMP11;

vec4 _TMP9;

vec4 _TMP7;

vec4 _TMP5;

float _TMP4;

vec4 _TMP3;

vec4 _TMP2;

float _TMP29;

float _TMP28;

float _TMP27;

float _TMP26;

vec4 _TMP1;

vec2 _TMP0;

input_dummy _IN1;

uniform sampler2D Texture;

vec2 _coord0057;

vec4 _TMP66;

vec4 _x0081;

vec2 _c0093;

vec2 _c0117;

vec2 _c0129;

vec4 _r0141;

vec4 _TMP142;

vec2 _c0149;

vec2 _c0161;

vec2 _c0173;

vec2 _c0185;

vec4 _r0197;

vec4 _TMP198;

vec4 _wid0205;

vec4 _weights0205;

vec4 _x0217;

vec4 _a0227;

vec4 _TMP236;

vec4 _x0237;

vec4 _wid0249;

vec4 _weights0249;

float _distance0249;

vec4 _x0261;

vec4 _a0271;

vec4 _TMP280;

vec4 _x0281;

float _c0293;

float _a0295;

varying vec4 TEX0;

varying vec4 TEX1;

varying vec4 TEX2;




uniform int FrameDirection;
uniform int FrameCount;
#ifdef GL_ES
uniform mediump vec2 OutputSize;
uniform mediump vec2 TextureSize;
uniform mediump vec2 InputSize;
#else
uniform vec2 OutputSize;
uniform vec2 TextureSize;
uniform vec2 InputSize;
#endif
void main()

{



    vec2 _xy;

    vec2 _ratio_scale;

    vec2 _uv_ratio;

    vec4 _coeffs;

    vec4 _weights1;

    vec4 _weights2;

    vec3 _mul_res;

    vec3 _dotMaskWeights;



    _coord0057 = TEX0.xy*(TextureSize.x/InputSize.x);

    _xy = (_coord0057*InputSize.x)/TextureSize.x;

    _ratio_scale = _xy*TextureSize - vec2( 5.00000000E-001, 5.00000000E-001);

    _uv_ratio = fract(_ratio_scale);

    _TMP0 = floor(_ratio_scale);

    _xy = (_TMP0 + vec2( 5.00000000E-001, 5.00000000E-001))/TextureSize;

    _coeffs = 3.14159274E+000*vec4(1.00000000E+000 + _uv_ratio.x, _uv_ratio.x, 1.00000000E+000 - _uv_ratio.x, 2.00000000E+000 - _uv_ratio.x);

    _TMP1 = abs(_coeffs);

    _TMP66 = max(_TMP1, vec4( 9.99999975E-006, 9.99999975E-006, 9.99999975E-006, 9.99999975E-006));

    _TMP26 = sin(_TMP66.x);

    _TMP27 = sin(_TMP66.y);

    _TMP28 = sin(_TMP66.z);

    _TMP29 = sin(_TMP66.w);

    _TMP2 = vec4(_TMP26, _TMP27, _TMP28, _TMP29);

    _x0081 = _TMP66/2.00000000E+000;

    _TMP26 = sin(_x0081.x);

    _TMP27 = sin(_x0081.y);

    _TMP28 = sin(_x0081.z);

    _TMP29 = sin(_x0081.w);

    _TMP3 = vec4(_TMP26, _TMP27, _TMP28, _TMP29);

    _coeffs = ((2.00000000E+000*_TMP2)*_TMP3)/(_TMP66*_TMP66);

    _TMP4 = dot(_coeffs, vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000));

    _coeffs = _coeffs/_TMP4;

    _c0093 = _xy + vec2(-TEX2.x, 0.00000000E+000);

    _TMP5 = texture2D(Texture, _c0093);

    _TMP30 = pow(_TMP5.x, 2.40000010E+000);

    _TMP31 = pow(_TMP5.y, 2.40000010E+000);

    _TMP32 = pow(_TMP5.z, 2.40000010E+000);

    _TMP33 = pow(_TMP5.w, 2.40000010E+000);

    _TMP6 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _TMP7 = texture2D(Texture, _xy);

    _TMP30 = pow(_TMP7.x, 2.40000010E+000);

    _TMP31 = pow(_TMP7.y, 2.40000010E+000);

    _TMP32 = pow(_TMP7.z, 2.40000010E+000);

    _TMP33 = pow(_TMP7.w, 2.40000010E+000);

    _TMP8 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _c0117 = _xy + vec2(TEX2.x, 0.00000000E+000);

    _TMP9 = texture2D(Texture, _c0117);

    _TMP30 = pow(_TMP9.x, 2.40000010E+000);

    _TMP31 = pow(_TMP9.y, 2.40000010E+000);

    _TMP32 = pow(_TMP9.z, 2.40000010E+000);

    _TMP33 = pow(_TMP9.w, 2.40000010E+000);

    _TMP10 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _c0129 = _xy + vec2(2.00000000E+000*TEX2.x, 0.00000000E+000);

    _TMP11 = texture2D(Texture, _c0129);

    _TMP30 = pow(_TMP11.x, 2.40000010E+000);

    _TMP31 = pow(_TMP11.y, 2.40000010E+000);

    _TMP32 = pow(_TMP11.z, 2.40000010E+000);

    _TMP33 = pow(_TMP11.w, 2.40000010E+000);

    _TMP12 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _r0141 = _coeffs.x*_TMP6;

    _r0141 = _r0141 + _coeffs.y*_TMP8;

    _r0141 = _r0141 + _coeffs.z*_TMP10;

    _r0141 = _r0141 + _coeffs.w*_TMP12;

    _TMP34 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _r0141);

    _TMP142 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP34);

    _c0149 = _xy + vec2(-TEX2.x, TEX2.y);

    _TMP14 = texture2D(Texture, _c0149);

    _TMP30 = pow(_TMP14.x, 2.40000010E+000);

    _TMP31 = pow(_TMP14.y, 2.40000010E+000);

    _TMP32 = pow(_TMP14.z, 2.40000010E+000);

    _TMP33 = pow(_TMP14.w, 2.40000010E+000);

    _TMP15 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _c0161 = _xy + vec2(0.00000000E+000, TEX2.y);

    _TMP16 = texture2D(Texture, _c0161);

    _TMP30 = pow(_TMP16.x, 2.40000010E+000);

    _TMP31 = pow(_TMP16.y, 2.40000010E+000);

    _TMP32 = pow(_TMP16.z, 2.40000010E+000);

    _TMP33 = pow(_TMP16.w, 2.40000010E+000);

    _TMP17 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _c0173 = _xy + TEX2.xy;

    _TMP18 = texture2D(Texture, _c0173);

    _TMP30 = pow(_TMP18.x, 2.40000010E+000);

    _TMP31 = pow(_TMP18.y, 2.40000010E+000);

    _TMP32 = pow(_TMP18.z, 2.40000010E+000);

    _TMP33 = pow(_TMP18.w, 2.40000010E+000);

    _TMP19 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _c0185 = _xy + vec2(2.00000000E+000*TEX2.x, TEX2.y);

    _TMP20 = texture2D(Texture, _c0185);

    _TMP30 = pow(_TMP20.x, 2.40000010E+000);

    _TMP31 = pow(_TMP20.y, 2.40000010E+000);

    _TMP32 = pow(_TMP20.z, 2.40000010E+000);

    _TMP33 = pow(_TMP20.w, 2.40000010E+000);

    _TMP21 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _r0197 = _coeffs.x*_TMP15;

    _r0197 = _r0197 + _coeffs.y*_TMP17;

    _r0197 = _r0197 + _coeffs.z*_TMP19;

    _r0197 = _r0197 + _coeffs.w*_TMP21;

    _TMP34 = min(vec4( 1.00000000E+000, 1.00000000E+000, 1.00000000E+000, 1.00000000E+000), _r0197);

    _TMP198 = max(vec4( 0.00000000E+000, 0.00000000E+000, 0.00000000E+000, 0.00000000E+000), _TMP34);

    _TMP30 = pow(_TMP142.x, 4.00000000E+000);

    _TMP31 = pow(_TMP142.y, 4.00000000E+000);

    _TMP32 = pow(_TMP142.z, 4.00000000E+000);

    _TMP33 = pow(_TMP142.w, 4.00000000E+000);

    _TMP35 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _wid0205 = 2.00000000E+000 + 2.00000000E+000*_TMP35;

    _weights0205 = vec4(_uv_ratio.y/3.00000012E-001, _uv_ratio.y/3.00000012E-001, _uv_ratio.y/3.00000012E-001, _uv_ratio.y/3.00000012E-001);

    _x0217 = 5.00000000E-001*_wid0205;

    _TMP46 = inversesqrt(_x0217.x);

    _TMP47 = inversesqrt(_x0217.y);

    _TMP48 = inversesqrt(_x0217.z);

    _TMP49 = inversesqrt(_x0217.w);

    _TMP36 = vec4(_TMP46, _TMP47, _TMP48, _TMP49);

    _a0227 = _weights0205*_TMP36;

    _TMP30 = pow(_a0227.x, _wid0205.x);

    _TMP31 = pow(_a0227.y, _wid0205.y);

    _TMP32 = pow(_a0227.z, _wid0205.z);

    _TMP33 = pow(_a0227.w, _wid0205.w);

    _TMP37 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _x0237 = -_TMP37;

    _TMP30 = pow(2.71828198E+000, _x0237.x);

    _TMP31 = pow(2.71828198E+000, _x0237.y);

    _TMP32 = pow(2.71828198E+000, _x0237.z);

    _TMP33 = pow(2.71828198E+000, _x0237.w);

    _TMP236 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _weights1 = (1.70000005E+000*_TMP236)/(6.00000024E-001 + 2.00000003E-001*_wid0205);

    _distance0249 = 1.00000000E+000 - _uv_ratio.y;

    _TMP30 = pow(_TMP198.x, 4.00000000E+000);

    _TMP31 = pow(_TMP198.y, 4.00000000E+000);

    _TMP32 = pow(_TMP198.z, 4.00000000E+000);

    _TMP33 = pow(_TMP198.w, 4.00000000E+000);

    _TMP35 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _wid0249 = 2.00000000E+000 + 2.00000000E+000*_TMP35;

    _weights0249 = vec4(_distance0249/3.00000012E-001, _distance0249/3.00000012E-001, _distance0249/3.00000012E-001, _distance0249/3.00000012E-001);

    _x0261 = 5.00000000E-001*_wid0249;

    _TMP46 = inversesqrt(_x0261.x);

    _TMP47 = inversesqrt(_x0261.y);

    _TMP48 = inversesqrt(_x0261.z);

    _TMP49 = inversesqrt(_x0261.w);

    _TMP36 = vec4(_TMP46, _TMP47, _TMP48, _TMP49);

    _a0271 = _weights0249*_TMP36;

    _TMP30 = pow(_a0271.x, _wid0249.x);

    _TMP31 = pow(_a0271.y, _wid0249.y);

    _TMP32 = pow(_a0271.z, _wid0249.z);

    _TMP33 = pow(_a0271.w, _wid0249.w);

    _TMP37 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _x0281 = -_TMP37;

    _TMP30 = pow(2.71828198E+000, _x0281.x);

    _TMP31 = pow(2.71828198E+000, _x0281.y);

    _TMP32 = pow(2.71828198E+000, _x0281.z);

    _TMP33 = pow(2.71828198E+000, _x0281.w);

    _TMP280 = vec4(_TMP30, _TMP31, _TMP32, _TMP33);

    _weights2 = (1.70000005E+000*_TMP280)/(6.00000024E-001 + 2.00000003E-001*_wid0249);

    _mul_res = (_TMP142*_weights1 + _TMP198*_weights2).xyz;

    _a0295 = TEX1.x/2.00000000E+000;

    _TMP39 = abs(_a0295);

    _TMP40 = fract(_TMP39);

    _TMP41 = abs(2.00000000E+000);

    _c0293 = _TMP40*_TMP41;

    if (TEX1.x < 0.00000000E+000) {
        _TMP42 = -_c0293;

    } else {

        _TMP42 = _c0293;

    }
    _TMP24 = floor(_TMP42);

    _dotMaskWeights = vec3( 1.00000000E+000, 6.99999988E-001, 1.00000000E+000) + _TMP24*vec3( -3.00000012E-001, 3.00000012E-001, -3.00000012E-001);

    _mul_res = _mul_res*_dotMaskWeights;

    _TMP43 = pow(_mul_res.x, 4.54545438E-001);

    _TMP44 = pow(_mul_res.y, 4.54545438E-001);

    _TMP45 = pow(_mul_res.z, 4.54545438E-001);

    _mul_res = vec3(_TMP43, _TMP44, _TMP45);

    _ret_0 = vec4(_mul_res.x, _mul_res.y, _mul_res.z, 1.00000000E+000);

    gl_FragColor = _ret_0;

    return;

}
