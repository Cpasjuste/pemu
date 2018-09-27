varying     vec4 _t1;
varying     vec2 _texCoord1;
varying     vec4 _position1;
varying     float _frame_rotation;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct out_vertex {
    vec4 _position1;
    vec2 _texCoord1;
    vec4 _t1;
};
out_vertex _ret_0;
float _TMP0;
uniform mat4 MVPMatrix;
input_dummy _IN1;
vec4 _r0005;
vec4 VertexCoord;
vec4 TexCoord;
varying vec4 TEX0;
varying vec4 TEX1;


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

    mat4 MVPMatrix_ = transpose_(MVPMatrix);
    out_vertex _OUT;
    vec2 _ps;
    _TMP0 = dot(vec4(float(MVPMatrix_[0].x), float(MVPMatrix_[0].y), float(MVPMatrix_[0].z), float(MVPMatrix_[0].w)), vec4(float(VertexCoord.x), float(VertexCoord.y), float(VertexCoord.z), float(VertexCoord.w)));
    _r0005.x = float(_TMP0);
    _TMP0 = dot(vec4(float(MVPMatrix_[1].x), float(MVPMatrix_[1].y), float(MVPMatrix_[1].z), float(MVPMatrix_[1].w)), vec4(float(VertexCoord.x), float(VertexCoord.y), float(VertexCoord.z), float(VertexCoord.w)));
    _r0005.y = float(_TMP0);
    _TMP0 = dot(vec4(float(MVPMatrix_[2].x), float(MVPMatrix_[2].y), float(MVPMatrix_[2].z), float(MVPMatrix_[2].w)), vec4(float(VertexCoord.x), float(VertexCoord.y), float(VertexCoord.z), float(VertexCoord.w)));
    _r0005.z = float(_TMP0);
    _TMP0 = dot(vec4(float(MVPMatrix_[3].x), float(MVPMatrix_[3].y), float(MVPMatrix_[3].z), float(MVPMatrix_[3].w)), vec4(float(VertexCoord.x), float(VertexCoord.y), float(VertexCoord.z), float(VertexCoord.w)));
    _r0005.w = float(_TMP0);
    _ps = vec2(float((1.00000000E+000/TextureSize.x)), float((1.00000000E+000/TextureSize.y)));
    _OUT._t1.xy = vec2(_ps.x, 0.00000000E+000);
    _OUT._t1.zw = vec2(0.00000000E+000, _ps.y);
    _ret_0._position1 = _r0005;
    _ret_0._texCoord1 = TexCoord.xy;
    _ret_0._t1 = _OUT._t1;
    gl_Position = vec4(float(_r0005.x), float(_r0005.y), float(_r0005.z), float(_r0005.w));
    TEX0.xy = TexCoord.xy;
    TEX1 = _OUT._t1;
    return;
    TEX0.xy = _ret_0._texCoord1;
    TEX1 = _ret_0._t1;
}
