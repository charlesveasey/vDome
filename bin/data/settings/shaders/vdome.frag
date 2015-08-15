#version 150

// TEXTURE INPUT
uniform sampler2DRect texsampler;
in vec2 vtexcoord;
vec4 t;

// INTENSITY: BC
uniform float brightness;
uniform float contrast;
vec3 cBC;
const float AvgLumR = 0.5;
const float AvgLumG = 0.5;
const float AvgLumB = 0.5;
const vec3 LumCoeff = vec3 (0.2125, 0.7154, 0.0721);

// INTENSITY: LEVELS
uniform float blackLevel;
uniform float whiteLevel;

// COLOR: HSL
uniform float hue;
uniform float saturation;
uniform float lightness;
vec3 cHSL;
vec3 cRGB;

// COLOR: GAMMA
uniform float gamma;
uniform float gammaR;
uniform float gammaG;
uniform float gammaB;

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
 ** Hue, saturation, luminance
 */

vec3 RGBToHSL(vec3 color) {
	vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
	
	float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
	float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
	float delta = fmax - fmin;             //Delta RGB value
    
	hsl.z = (fmax + fmin) / 2.0; // Luminance
    
	if (delta == 0.0)		//This is a gray, no chroma...
	{
		hsl.x = 0.0;	// Hue
		hsl.y = 0.0;	// Saturation
	}
	else                                    //Chromatic data...
	{
		if (hsl.z < 0.5)
			hsl.y = delta / (fmax + fmin); // Saturation
		else
			hsl.y = delta / (2.0 - fmax - fmin); // Saturation
		
		float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
		float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
		float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;
        
		if (color.r == fmax )
			hsl.x = deltaB - deltaG; // Hue
		else if (color.g == fmax)
			hsl.x = (1.0 / 3.0) + deltaR - deltaB; // Hue
		else if (color.b == fmax)
			hsl.x = (2.0 / 3.0) + deltaG - deltaR; // Hue
        
		if (hsl.x < 0.0)
			hsl.x += 1.0; // Hue
		else if (hsl.x > 1.0)
			hsl.x -= 1.0; // Hue
	}
    
	return hsl;
}


float HueToRGB(float f1, float f2, float hue) {
	if (hue < 0.0)
		hue += 1.0;
	else if (hue > 1.0)
		hue -= 1.0;
	float res;
	if ((6.0 * hue) < 1.0)
		res = f1 + (f2 - f1) * 6.0 * hue;
	else if ((2.0 * hue) < 1.0)
		res = f2;
	else if ((3.0 * hue) < 2.0)
		res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
	else
		res = f1;
	return res;
}

vec3 HSLToRGB(vec3 hsl) {
	vec3 rgb;
	
	if (hsl.y == 0.0)
		rgb = vec3(hsl.z); // Luminance
	else
	{
		float f2;
		
		if (hsl.z < 0.5)
			f2 = hsl.z * (1.0 + hsl.y);
		else
			f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
        
		float f1 = 2.0 * hsl.z - f2;
		
		rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
		rgb.g = HueToRGB(f1, f2, hsl.x);
		rgb.b = HueToRGB(f1, f2, hsl.x - (1.0/3.0));
	}
	
	return rgb;
}


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

float GammaCorrection(float color, float gamma) {
    return pow(color, 1.0 / gamma);
}

vec3 LevelsControlOutputRange(vec3 color, vec3 minOutput, vec3 maxOutput) {
    return mix(vec3(minOutput), vec3(maxOutput), color);
}

void main() {
	//input
    t = texture(texsampler, vtexcoord);

	//hsl, contrast, brightness
    cBC = ContrastSaturationBrightness(t.rgb, brightness, contrast, 1);
    
    cHSL = RGBToHSL(cBC.rgb);
    cHSL.r *= hue;
    cHSL.g *= saturation;
    cHSL.b *= lightness;
    cRGB = HSLToRGB(cHSL.rgb);
	
	// gamma
    cRGB.r = GammaCorrection(cRGB.r, gammaR * gamma);
    cRGB.g = GammaCorrection(cRGB.g, gammaG * gamma);
    cRGB.b = GammaCorrection(cRGB.b, gammaB * gamma);

	// levels    
    cRGB = LevelsControlOutputRange(cRGB, vec3(blackLevel/255, blackLevel/255, blackLevel/255),
                                    vec3(whiteLevel/255, whiteLevel/255, whiteLevel/255));
    
	t2 = vec4(cRGB.rgb, t.a);

	// color curves
    set = mix(floor(t2*mapdim),t2*mapdim,float(interp));					//multiply color value for LUT range
	rout = float (texture(colorlut, vec2(set.r,0.)).r);						//look up red
	gout = float (texture(colorlut, vec2(set.g,0.)).g);						//look up green
	bout = float (texture(colorlut, vec2(set.b,0.)).b);						//look up blue
	mapped = vec4 (rout, gout, bout,t2.a);
	t2 = mix(t2, mapped, amt);

	// grey curve
    set = mix(floor(t2*mapdim),t2*mapdim,float(interp));					//multiply color value for LUT range
	rout = float (texture(colorlut, vec2(set.r,0.)).a);						//look up red
	gout = float (texture(colorlut, vec2(set.g,0.)).a);						//look up green
	bout = float (texture(colorlut, vec2(set.b,0.)).a);						//look up blue
	mapped = vec4 (rout, gout, bout,t2.a);
	t2 = mix(t2, mapped, amt);

	// mask
    float mask = texture(maskTex, vtexcoord).a;

	// output
    outputColor = vec4(t2.rgb, 1-mask);
}

