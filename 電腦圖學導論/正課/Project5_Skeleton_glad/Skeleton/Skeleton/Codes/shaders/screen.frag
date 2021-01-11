#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 vertColor;

uniform sampler2D screenTexture;
uniform float vx_offset=0.5;
uniform float screen_w; 
uniform float screen_h; 
uniform float t;
uniform float pixel_w = 15;
uniform float pixel_h = 10; 

uniform int frame_buffer_type;
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

#define HueLevCount 6
#define SatLevCount 7
#define ValLevCount 4
float HueLevels[HueLevCount];
float SatLevels[SatLevCount];
float ValLevels[ValLevCount];
 
vec3 RGBtoHSV( float r, float g, float b) {
   float minv, maxv, delta;
   vec3 res;
 
   minv = min(min(r, g), b);
   maxv = max(max(r, g), b);
   res.z = maxv;            // v
   
   delta = maxv - minv;
 
   if( maxv != 0.0 )
      res.y = delta / maxv;      // s
   else {
      // r = g = b = 0      // s = 0, v is undefined
      res.y = 0.0;
      res.x = -1.0;
      return res;
   }
 
   if( r == maxv )
      res.x = ( g - b ) / delta;      // between yellow & magenta
   else if( g == maxv )
      res.x = 2.0 + ( b - r ) / delta;   // between cyan & yellow
   else
      res.x = 4.0 + ( r - g ) / delta;   // between magenta & cyan
 
   res.x = res.x * 60.0;            // degrees
   if( res.x < 0.0 )
      res.x = res.x + 360.0;
      
   return res;
}
 
vec3 HSVtoRGB(float h, float s, float v ) {
   int i;
   float f, p, q, t;
   vec3 res;
 
   if( s == 0.0 ) {
      // achromatic (grey)
      res.x = v;
      res.y = v;
      res.z = v;
      return res;
   }
 
   h /= 60.0;         // sector 0 to 5
   i = int(floor( h ));
   f = h - float(i);         // factorial part of h
   p = v * ( 1.0 - s );
   q = v * ( 1.0 - s * f );
   t = v * ( 1.0 - s * ( 1.0 - f ) );
 
   if (i==0) {
		res.x = v;
		res.y = t;
		res.z = p;
   	} else if (i==1) {
         res.x = q;
         res.y = v;
         res.z = p;
	} else if (i==2) {
         res.x = p;
         res.y = v;
         res.z = t;
	} else if (i==3) {
         res.x = p;
         res.y = q;
         res.z = v;
	} else if (i==4) {
         res.x = t;
         res.y = p;
         res.z = v;
	} else if (i==5) {
         res.x = v;
         res.y = p;
         res.z = q;
   }
   
   return res;
}
 
float nearestLevel(float col, int mode) {
 
   if (mode==0) {
   		for (int i =0; i<HueLevCount-1; i++ ) {
		    if (col >= HueLevels[i] && col <= HueLevels[i+1]) {
		      return HueLevels[i+1];
		    }
		}
	 }
 
	if (mode==1) {
		for (int i =0; i<SatLevCount-1; i++ ) {
			if (col >= SatLevels[i] && col <= SatLevels[i+1]) {
	          return SatLevels[i+1];
	        }
		}
	}
 
 
	if (mode==2) {
		for (int i =0; i<ValLevCount-1; i++ ) {
			if (col >= ValLevels[i] && col <= ValLevels[i+1]) {
	          return ValLevels[i+1];
	        }
		}
	}
 
    return 0;
}
 
// averaged pixel intensity from 3 color channels
float avg_intensity(vec4 pix) {
 return (pix.r + pix.g + pix.b)/3.;
}
 
vec4 get_pixel(vec2 coords, float dx, float dy) {
 return texture2D(screenTexture,coords + vec2(dx, dy));
}
 
// returns pixel color
float IsEdge(in vec2 coords){
  float dxtex = 1.0 / TexCoords.x ;
  float dytex = 1.0 / TexCoords.y ;
  
  float pix[9];
  
  int k = -1;
  float delta;
 
  // read neighboring pixel intensities
float pix0 = avg_intensity(get_pixel(coords,-1.0*dxtex, -1.0*dytex));
float pix1 = avg_intensity(get_pixel(coords,-1.0*dxtex, 0.0*dytex));
float pix2 = avg_intensity(get_pixel(coords,-1.0*dxtex, 1.0*dytex));
float pix3 = avg_intensity(get_pixel(coords,0.0*dxtex, -1.0*dytex));
float pix4 = avg_intensity(get_pixel(coords,0.0*dxtex, 0.0*dytex));
float pix5 = avg_intensity(get_pixel(coords,0.0*dxtex, 1.0*dytex));
float pix6 = avg_intensity(get_pixel(coords,1.0*dxtex, -1.0*dytex));
float pix7 = avg_intensity(get_pixel(coords,1.0*dxtex, 0.0*dytex));
float pix8 = avg_intensity(get_pixel(coords,1.0*dxtex, 1.0*dytex)); 
  // average color differences around neighboring pixels
  delta = (abs(pix1-pix7)+
          abs(pix5-pix3) +
          abs(pix0-pix8)+
          abs(pix2-pix6)
           )/4.;
 
  return clamp(5.5*delta,0.0,1.0);
}
 

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
        /*
            	HueLevels[0] = 0.0;
	            HueLevels[1] = 80.0;
	            HueLevels[2] = 160.0;
	            HueLevels[3] = 240.0;
	            HueLevels[4] = 320.0;
	            HueLevels[5] = 360.0; 
 
	            SatLevels[0] = 0.0;
	            SatLevels[1] = 0.1;
	            SatLevels[2] = 0.3;
	            SatLevels[3] = 0.5;
	            SatLevels[4] = 0.6;
	            SatLevels[5] = 0.8;
	            SatLevels[6] = 1.0;
 
	            ValLevels[0] = 0.0;
	            ValLevels[1] = 0.3;
	            ValLevels[2] = 0.6;
	            ValLevels[3] = 1.0; 
 
                vec4 colorOrg = texture2D(screenTexture, vertColor.xy);
                vec3 vHSV =  RGBtoHSV(colorOrg.r,colorOrg.g,colorOrg.b);
                vHSV.x = nearestLevel(vHSV.x, 0);
                vHSV.y = nearestLevel(vHSV.y, 1);
                vHSV.z = nearestLevel(vHSV.z, 2);
                float edg = IsEdge(vertColor.xy);
                vec3 vRGB = (edg >= 0.3)? vec3(0.0,0.0,0.0):HSVtoRGB(vHSV.x,vHSV.y,vHSV.z);
                FragColor = vec4(vRGB.x,vRGB.y,vRGB.z,1.0);

                */
                    vec4 color = texture2D(screenTexture, TexCoords);
    float factor = toonify(max(color.r, max(color.g, color.b)));
    FragColor = vec4(factor*color.rgb, color.a);
        }break;
    }
}    