uniform sampler2DRect tex;
uniform sampler2DRect mask;
uniform sampler2DRect background;
uniform vec2 bgDims;
void main()
{
	vec4 color = texture2DRect(tex,gl_TexCoord[0].st);

	//vec4 alpha = texture2DRect(mask,gl_TexCoord[0].st);


	vec4 alpha = 
	texture2DRect(mask,gl_TexCoord[0].st+vec2(-2.0,-2.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-2.0,-1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-2.0, 0.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-2.0, 1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-2.0, 2.0))	
	
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-1.0,-2.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-1.0,-1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-1.0, 0.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-1.0, 1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(-1.0, 2.0))	


	+texture2DRect(mask,gl_TexCoord[0].st+vec2(0.0,-2.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(0.0,-1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(0.0, 0.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(0.0, 1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(0.0, 2.0))	

	+texture2DRect(mask,gl_TexCoord[0].st+vec2(1.0,-2.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(1.0,-1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(1.0, 0.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(1.0, 1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(1.0, 2.0))	
	
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(2.0,-2.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(2.0,-1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(2.0, 0.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(2.0, 1.0))
	+texture2DRect(mask,gl_TexCoord[0].st+vec2(2.0, 2.0))	

	
	
	;
	
	
	float div = 25.0;
	alpha /= vec4(div,div,div,div);
	
	
	
	vec2 bgScale = bgDims/vec2(640.0, 480.0);
	vec4 bg = texture2DRect(background, gl_TexCoord[0].st*bgScale);
	
	gl_FragColor = color*alpha + bg*(vec4(1.0,1.0,1.0, 1.0) - alpha);//vec4(gl_TexCoord[0].st/vec2(640, 480), 0.0, 1.0);//color ;
}