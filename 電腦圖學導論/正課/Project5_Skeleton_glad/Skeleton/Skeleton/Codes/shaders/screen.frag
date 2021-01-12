#version 330 core


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D brushTexture;
uniform float vx_offset=0.5;
uniform float screen_w; 
uniform float screen_h; 
uniform float t;
uniform float pixel_w = 15;
uniform float pixel_h = 10; 

uniform int frame_buffer_type;


uniform sampler2D TextureFBO1;
uniform sampler2D TextureFBO2;
uniform sampler2D TextureFBO3;
uniform sampler2D TextureFBO4;
uniform sampler2D TextureFBO5;
uniform sampler2D TextureFBO6;
uniform sampler2D TextureFBO7;
uniform sampler2D TextureFBO8;


//--------------fxaa---------------
uniform vec2 u_texelStep;
uniform int u_showEdges;
uniform int u_fxaaOn;

uniform float u_lumaThreshold;
uniform float u_mulReduce;
uniform float u_minReduce;
uniform float u_maxSpan;
//-------------------------------

float toonify(in float intensity) {
    if (intensity > 0.8)
        return 1.0;
    else if (intensity > 0.5)
        return 0.8;
    else if (intensity > 0.25)
        return 0.3;
    else
        return 0.1;
}

//https://gist.github.com/sugi-cho/6a01cae436acddd72bdf

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float character(int n, vec2 p)
{
	p = floor(p*vec2(4.0, -4.0) + 2.5);
    if (clamp(p.x, 0.0, 4.0) == p.x)
	{
        if (clamp(p.y, 0.0, 4.0) == p.y)	
		{
        	int a = int(round(p.x) + 5.0 * round(p.y));
			if (((n >> a) & 1) == 1) return 1.0;
		}	
    }
	return 0.0;
}

float character(float n, vec2 p) {
  p = floor(p*vec2(4.0, -4.0) + 2.5);
  if (clamp(p.x, 0.0, 4.0) == p.x && clamp(p.y, 0.0, 4.0) == p.y){
    if (int(mod(n/exp2(p.x + 5.0*p.y), 2.0)) == 1) return 1.0;
  }
  return 0.0;
}
float luma(vec3 color) {
  return dot(color, vec3(0.299, 0.587, 0.114));
}

float asciiFilter(vec3 color, vec2 uv, float pixelSize) {
  float threshold = luma(color);
  float n =  65536.0;                  // .
  if (threshold > 0.2) n = 65600.0;    // :
  if (threshold > 0.3) n = 332772.0;   // *
  if (threshold > 0.4) n = 15255086.0; // o
  if (threshold > 0.5) n = 23385164.0; // &
  if (threshold > 0.6) n = 15252014.0; // 8
  if (threshold > 0.7) n = 13199452.0; // @
  if (threshold > 0.8) n = 11512810.0; // #
  vec2 p = mod( uv / ( pixelSize * 0.5 ), 2.0) - vec2(1.0);
  return character(n, p);
}

float asciiFilter(vec3 color, vec2 uv) {
  return asciiFilter(color, uv, 1.0 / 100.0);
}

vec2 barrelDistortion(vec2 coord, float amt)
{
	vec2 cc = coord - 0.5;
	float dist = dot(cc, cc);
	return coord + cc * dist * amt;
}

float sat(float t)
{
	return clamp(t, 0.0, 1.0);
}

float linterp(float t) {
	return sat( 1.0 - abs( 2.0*t - 1.0 ) );
}

float remap(float t, float a, float b) {
	return sat((t - a) / (b - a));
}

vec4 spectrum_offset(float t) {
	vec4 ret;
	float lo = step(t,0.5);
	float hi = 1.0-lo;
	float w = linterp( remap( t, 1.0/6.0, 5.0/6.0 ) );
	ret = vec4(lo,1.0,hi, 1.) * vec4(1.0-w, w, 1.0-w, 1.);

	return pow( ret, vec4(1.0/2.2) );
}

const float max_distort = 2.2;
const int num_iter = 12;
const float reci_num_iter_f = 1.0 / float(num_iter);


void main()
{

    switch(frame_buffer_type){
        case 1:default:{
            vec3 col = texture(screenTexture, TexCoords).rgb;
            FragColor = vec4(col, 1.0);
        }break;
        case 2:{
             vec2 uv = TexCoords.xy;
            vec3 tc = vec3(1.0, 0.0, 0.0);
            if (uv.x < (vx_offset-0.005))
            {
            float dx = pixel_w*(1./screen_w);
            float dy = pixel_h*(1./screen_h);
            vec2 coord = vec2(dx*floor(uv.x/dx),
                                dy*floor(uv.y/dy));
            tc = texture2D(screenTexture, coord).rgb;
            }
            else if (uv.x>=(vx_offset+0.005))
            {
            tc = texture2D(screenTexture, uv).rgb;
            }
            FragColor = vec4(tc, 1.0);
        }break;
        case 3:{
            vec2 uv = TexCoords.xy;
            vec3 col = texture( screenTexture, uv + 0.005*vec2( sin(t+1024.0*uv.x),cos(t+768.0*uv.y))).xyz;
            FragColor = vec4(col, 1.0);
        }break;
        case 4:{
            vec2 iResolution;
            iResolution.x = screen_w;
            iResolution.y = screen_h;
            float x = 1.0 / iResolution.x;
            float y = 1.0 / iResolution.y;
            vec4 horizEdge = vec4( 0.0 );
                horizEdge -= texture2D( screenTexture, vec2( TexCoords.x - x, TexCoords.y - y ) ) * 1.0;
                horizEdge -= texture2D( screenTexture, vec2( TexCoords.x - x, TexCoords.y     ) ) * 2.0;
                horizEdge -= texture2D( screenTexture, vec2( TexCoords.x - x, TexCoords.y + y ) ) * 1.0;
                horizEdge += texture2D( screenTexture, vec2( TexCoords.x + x, TexCoords.y - y ) ) * 1.0;
                horizEdge += texture2D( screenTexture, vec2( TexCoords.x + x, TexCoords.y     ) ) * 2.0;
                horizEdge += texture2D( screenTexture, vec2( TexCoords.x + x, TexCoords.y + y ) ) * 1.0;
           vec4 vertEdge = vec4( 0.0 );
                vertEdge -= texture2D( screenTexture, vec2( TexCoords.x - x, TexCoords.y - y ) ) * 1.0;
                vertEdge -= texture2D( screenTexture, vec2( TexCoords.x    , TexCoords.y - y ) ) * 2.0;
                vertEdge -= texture2D( screenTexture, vec2( TexCoords.x + x, TexCoords.y - y ) ) * 1.0;
                vertEdge += texture2D( screenTexture, vec2( TexCoords.x - x, TexCoords.y + y ) ) * 1.0;
                vertEdge += texture2D( screenTexture, vec2( TexCoords.x    , TexCoords.y + y ) ) * 2.0;
            vertEdge += texture2D( screenTexture, vec2( TexCoords.x + x, TexCoords.y + y ) ) * 1.0;
           vec3 edge = sqrt((horizEdge.rgb * horizEdge.rgb) + (vertEdge.rgb * vertEdge.rgb));
           FragColor = vec4(edge, texture2D(screenTexture, TexCoords.xy).a);
        }break;
        case 5:{
        
	        vec3 hsv = rgb2hsv(texture(screenTexture, TexCoords).rgb);
	        hsv.z = round(hsv.z*5)/5;
            FragColor = vec4(hsv2rgb(hsv),1);
                

                /*
                vec4 color = texture2D(screenTexture, TexCoords);
                float factor = toonify(max(color.r, max(color.g, color.b)));
                FragColor = vec4(factor*color.rgb, color.a);
                */
        }break;
        case 6:{
        const float offset = 1.0 / 300.0;  
                vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
        }break;
        case 7:{
    

    FragColor.r = asciiFilter(vec3(texture(screenTexture,TexCoords).r),TexCoords);
    FragColor.g = asciiFilter(vec3(texture(screenTexture,TexCoords).g),TexCoords);
    FragColor.b = asciiFilter(vec3(texture(screenTexture,TexCoords).b),TexCoords);
      FragColor.a = 1.0f;
        }break;
        case 8:{//motion blur
            FragColor = mix(texture(TextureFBO7, TexCoords), texture(screenTexture, TexCoords), 0.5);
            FragColor = mix(texture(TextureFBO6, TexCoords), FragColor, 0.5);
            FragColor = mix(texture(TextureFBO5, TexCoords), FragColor, 0.5);
            FragColor = mix(texture(TextureFBO4, TexCoords), FragColor, 0.5);
            FragColor = mix(texture(TextureFBO3, TexCoords), FragColor, 0.5);
            FragColor = mix(texture(TextureFBO2, TexCoords), FragColor, 0.5);
            FragColor = mix(texture(TextureFBO1, TexCoords), FragColor, 0.4);
        }break;
        case 9:{
                vec3 rgbM = texture(screenTexture, TexCoords).rgb;

	// Possibility to toggle FXAA on and off.
	if (u_fxaaOn == 0)
	{
		FragColor = vec4(rgbM, 1.0);
		
		return;
	}

	// Sampling neighbour texels. Offsets are adapted to OpenGL texture coordinates. 
	vec3 rgbNW = textureOffset(screenTexture, TexCoords, ivec2(-1, 1)).rgb;
    vec3 rgbNE = textureOffset(screenTexture, TexCoords, ivec2(1, 1)).rgb;
    vec3 rgbSW = textureOffset(screenTexture, TexCoords, ivec2(-1, -1)).rgb;
    vec3 rgbSE = textureOffset(screenTexture, TexCoords, ivec2(1, -1)).rgb;

	// see http://en.wikipedia.org/wiki/Grayscale
	const vec3 toLuma = vec3(0.299, 0.587, 0.114);
	
	// Convert from RGB to luma.
	float lumaNW = dot(rgbNW, toLuma);
	float lumaNE = dot(rgbNE, toLuma);
	float lumaSW = dot(rgbSW, toLuma);
	float lumaSE = dot(rgbSE, toLuma);
	float lumaM = dot(rgbM, toLuma);

	// Gather minimum and maximum luma.
	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	
	// If contrast is lower than a maximum threshold ...
	if (lumaMax - lumaMin <= lumaMax * u_lumaThreshold)
	{
		// ... do no AA and return.
		FragColor = vec4(rgbM, 1.0);
		
		return;
	}  
	
	// Sampling is done along the gradient.
	vec2 samplingDirection;	
	samplingDirection.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    samplingDirection.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    // Sampling step distance depends on the luma: The brighter the sampled texels, the smaller the final sampling step direction.
    // This results, that brighter areas are less blurred/more sharper than dark areas.  
    float samplingDirectionReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * 0.25 * u_mulReduce, u_minReduce);

	// Factor for norming the sampling direction plus adding the brightness influence. 
	float minSamplingDirectionFactor = 1.0 / (min(abs(samplingDirection.x), abs(samplingDirection.y)) + samplingDirectionReduce);
    
    // Calculate final sampling direction vector by reducing, clamping to a range and finally adapting to the texture size. 
    samplingDirection = clamp(samplingDirection * minSamplingDirectionFactor, vec2(-u_maxSpan), vec2(u_maxSpan)) * u_texelStep;
	
	// Inner samples on the tab.
	vec3 rgbSampleNeg = texture(screenTexture, TexCoords + samplingDirection * (1.0/3.0 - 0.5)).rgb;
	vec3 rgbSamplePos = texture(screenTexture, TexCoords + samplingDirection * (2.0/3.0 - 0.5)).rgb;

	vec3 rgbTwoTab = (rgbSamplePos + rgbSampleNeg) * 0.5;  

	// Outer samples on the tab.
	vec3 rgbSampleNegOuter = texture(screenTexture, TexCoords + samplingDirection * (0.0/3.0 - 0.5)).rgb;
	vec3 rgbSamplePosOuter = texture(screenTexture, TexCoords + samplingDirection * (3.0/3.0 - 0.5)).rgb;
	
	vec3 rgbFourTab = (rgbSamplePosOuter + rgbSampleNegOuter) * 0.25 + rgbTwoTab * 0.5;   
	
	// Calculate luma for checking against the minimum and maximum value.
	float lumaFourTab = dot(rgbFourTab, toLuma);
	
	// Are outer samples of the tab beyond the edge ... 
	if (lumaFourTab < lumaMin || lumaFourTab > lumaMax)
	{
		// ... yes, so use only two samples.
		FragColor = vec4(rgbTwoTab, 1.0); 
	}
	else
	{
		// ... no, so use four samples. 
		FragColor = vec4(rgbFourTab, 1.0);
	}

	// Show edges for debug purposes.	
	if (u_showEdges != 0)
	{
		FragColor.r = 1.0;
	}
        }break;
        case 10:{
             vec2 resolution;
            resolution.x = 0.8;
            resolution.y = 0.8;
        	vec2 uv=(TexCoords.xy/resolution.xy*.5)+.25;

	vec4 sumcol = vec4(0.0);
	vec4 sumw = vec4(0.0);
	for ( int i=0; i<num_iter;++i)
	{
		float t = float(i) * reci_num_iter_f;
		vec4 w = spectrum_offset( t );
		sumw += w;
		sumcol += w * texture2D(screenTexture, barrelDistortion(uv, .6 * max_distort*t ) );
	}

	FragColor = sumcol / sumw;
        }break;
    }
}    