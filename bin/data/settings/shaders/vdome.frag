#version 150

// TEXTURE INPUT
uniform sampler2DRect texsampler;
in vec2 vtexcoord;
vec4 t;

// contrast, saturation, brightness
uniform float brightness;
uniform float contrast;
uniform float saturation;
vec3 cCSB;
const float AvgLumR = 0.5;
const float AvgLumG = 0.5;
const float AvgLumB = 0.5;
const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

/// COLOR: CURVES
uniform sampler2DRect colorlut;
uniform int interp;
uniform float mapdim;
uniform float amt;
vec4 set;
float rout;
float gout;
float bout;
vec4 mapped;
vec4 t2;

// MASK
uniform sampler2DRect maskTex;

// TEXTURE OUTPUT
out vec4 outputColor;

/*
 ** Contrast, saturation, brightness
 ** Code of this function is from TGM's shader pack
 ** http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=21057
 */

// For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con) {
	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = color * brt;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, sat);
	vec3 conColor = mix(AvgLumin, satColor, con);
	return conColor;
}

void main() {
	//input
    t = texture(texsampler, vtexcoord);

    cCSB = ContrastSaturationBrightness(t.rgb, brightness, saturation, contrast);
	
	t2 = vec4(cCSB.rgb, t.a);
    
    // grey curve
    set = mix(floor(t2*mapdim),t2*mapdim,float(interp));					//multiply color value for LUT range
    rout = float (texture(colorlut, vec2(set.r,0.)).a);						//look up red
    gout = float (texture(colorlut, vec2(set.g,0.)).a);						//look up green
    bout = float (texture(colorlut, vec2(set.b,0.)).a);						//look up blue
    mapped = vec4 (rout, gout, bout,t2.a);
    t2 = mix(t2, mapped, amt);
    
	// color curves
    set = mix(floor(t2*mapdim),t2*mapdim,float(interp));					//multiply color value for LUT range
	rout = float (texture(colorlut, vec2(set.r,0.)).r);						//look up red
	gout = float (texture(colorlut, vec2(set.g,0.)).g);						//look up green
	bout = float (texture(colorlut, vec2(set.b,0.)).b);						//look up blue
	mapped = vec4 (rout, gout, bout,t2.a);
	t2 = mix(t2, mapped, amt);
    
	// mask
    float mask = texture(maskTex, vtexcoord).a;

	// output
    outputColor = vec4(t2.rgb, 1-mask);
}

