varying     vec3 VARmod_factor_ratio_scale;
varying     vec4 VARc22_32;
varying     vec4 VARc02_12;
varying     vec4 VARc21_31;
varying     vec4 VARc01_11;
struct tex_coords {
    vec4 VARc01_11;
    vec4 VARc21_31;
    vec4 VARc02_12;
    vec4 VARc22_32;
    vec3 VARmod_factor_ratio_scale;
};
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
};
vec4 _oPosition1;
tex_coords _coords1;
uniform mat4 MVPMatrix;
input_dummy _IN1;
vec4 _r0013;
vec4 VertexCoord;
vec4 COLOR;
varying vec4 COL0;
vec4 TexCoord;
 

         mat4 transpose_(mat4 matrix)
         {
            mat4 ret;
            for (int i = 0; i != 4; i++)
               for (int j = 0; j != 4; j++)
                  ret[i][j] = matrix[j][i];

            return ret;
         }
         
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
    VertexCoord = gl_Vertex;
    TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    COLOR = gl_Color;

    mat4 MVPMatrix_ = transpose_(MVPMatrix);
    vec4 _oColor;
    vec2 _delta;
    vec2 _TMP2;
    vec2 _TMP3;
    vec2 _TMP4;
    vec2 _TMP5;
    vec2 _TMP6;
    vec2 _TMP7;
    vec2 _TMP8;
    vec2 _TMP9;
    tex_coords _TMP11;
    _r0013.x = dot(MVPMatrix_[0], VertexCoord);
    _r0013.y = dot(MVPMatrix_[1], VertexCoord);
    _r0013.z = dot(MVPMatrix_[2], VertexCoord);
    _r0013.w = dot(MVPMatrix_[3], VertexCoord);
    _oPosition1 = _r0013;
    _oColor = COLOR;
    _delta = 1.00000000E+000/TextureSize;
    _TMP9 = TexCoord.xy + vec2(-_delta.x, 0.00000000E+000);
    _TMP11.VARc01_11 = vec4(_TMP9.x, _TMP9.y, TexCoord.x, TexCoord.y);
    _TMP7 = TexCoord.xy + vec2(_delta.x, 0.00000000E+000);
    _TMP8 = TexCoord.xy + vec2(2.00000000E+000*_delta.x, 0.00000000E+000);
    _TMP11.VARc21_31 = vec4(_TMP7.x, _TMP7.y, _TMP8.x, _TMP8.y);
    _TMP5 = TexCoord.xy + vec2(-_delta.x, _delta.y);
    _TMP6 = TexCoord.xy + vec2(0.00000000E+000, _delta.y);
    _TMP11.VARc02_12 = vec4(_TMP5.x, _TMP5.y, _TMP6.x, _TMP6.y);
    _TMP3 = TexCoord.xy + vec2(_delta.x, _delta.y);
    _TMP4 = TexCoord.xy + vec2(2.00000000E+000*_delta.x, _delta.y);
    _TMP11.VARc22_32 = vec4(_TMP3.x, _TMP3.y, _TMP4.x, _TMP4.y);
    _TMP2 = TexCoord.xy*TextureSize;
    _TMP11.VARmod_factor_ratio_scale = vec3((TexCoord.x*OutputSize.x*TextureSize.x)/InputSize.x, _TMP2.x, _TMP2.y);
    VARc01_11 = _TMP11.VARc01_11;
    VARc21_31 = _TMP11.VARc21_31;
    VARc02_12 = _TMP11.VARc02_12;
    VARc22_32 = _TMP11.VARc22_32;
    VARmod_factor_ratio_scale = _TMP11.VARmod_factor_ratio_scale;
    gl_Position = _r0013;
    COL0 = COLOR;
}
