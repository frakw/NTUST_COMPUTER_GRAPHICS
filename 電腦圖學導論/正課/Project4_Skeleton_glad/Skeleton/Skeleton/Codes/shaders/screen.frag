#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float vx_offset=0.5;
uniform float screen_w; 
uniform float screen_h; 
uniform float t;
uniform float pixel_w = 15;
uniform float pixel_h = 10; 

uniform int frame_buffer_type;


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
        }
    }
}    