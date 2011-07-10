uniform sampler2DRect tex;
uniform float brightness;
uniform float contrast;
uniform float saturation;


// For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)
{
	// Increase or decrease theese values to adjust r, g and b color channels seperately
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;
	
	const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
	
	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = color * brt;
	float intensityf = dot(brtColor, LumCoeff);
	vec3 intensity = vec3(intensityf, intensityf, intensityf);
	vec3 satColor = mix(intensity, brtColor, sat);
	vec3 conColor = mix(AvgLumin, satColor, con);
	return conColor;
}


void main()
{

	vec4 color = texture2DRect(tex,gl_TexCoord[0].st);

	/*
	// Apply contrast.
	color.rgb = ((color.rgb - 0.5) * max(contrast, 0.0)) + 0.5;

	// Apply brightness.
	color.rgb += brightness;
  */
  

	color.rgb = ContrastSaturationBrightness(color.rgb, brightness, saturation, contrast);
//	color.rgb = 
	gl_FragColor = color;
}
