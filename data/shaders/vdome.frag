#version 150
 
uniform float saturation;
uniform float brightness;
uniform float contrast;
													 
uniform float red;
uniform float green;
uniform float blue;

uniform sampler2DRect texsampler;

in vec2 vtexcoord;

const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

void main()
{
	vec4 t = texture2DRect(texsampler, vtexcoord);
	
	vec3 brt = t.rgb * brightness;
		
	vec3 avgluma = vec3(0.5, 0.5, 0.5);
	vec3 con = mix(avgluma, brt, contrast);
    
    vec3 intensity = vec3 (dot(con, LumCoeff));
    vec3 sat = mix(intensity, con, saturation);

	gl_FragColor = vec4(sat.rgb, t.a);
}