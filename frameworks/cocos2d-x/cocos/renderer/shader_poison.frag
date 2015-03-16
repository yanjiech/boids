const char* shader_poison_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision lowp float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
    vec4 color = texture2D( CC_Texture0, v_texCoord ) * v_fragmentColor;
	float gray = ( color.r + color.g + color.b ) / 2.0;
    gl_FragColor = vec4( gray, gray, gray, color.a ) * vec4( 0.6, 1.2, 0.3, 1 );
}
                                              
);
