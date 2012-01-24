#version 150 core

in vec3 worldPos;
in vec3 worldNormal;
in vec4 ex_Color;
in vec2 ex_TexCoord;

out vec4 out_FragColor;

uniform sampler2D texture1;
uniform sampler2D bg;
uniform float t;

void main() {
	float kw = 0.0005;
	vec4 res = vec4(0);
	for(int x = -3; x <= 3; x++) {
		for(int y = -3; y <= 3; y++) {
			res += texture2D(texture1,vec2(ex_TexCoord.x+x*kw,ex_TexCoord.y+y*kw));
		}
	}
	out_FragColor = texture2D(bg,vec2(1.0,1.0)-ex_TexCoord.xy) + texture2D(texture1,ex_TexCoord.xy) + res*0.05;

	/*if(t > 0.0){
		float shift = (sin(t*7.0)+16.0)/16.0;
		out_FragColor =
			(texture2D(texture1,ex_TexCoord.xy).rgba +
			texture2D(texture1,((ex_TexCoord.xy-vec2(0.5,0.5))/(1.05*shift))+vec2(0.5,0.5)).rgba * 0.7 +
			texture2D(texture1,((ex_TexCoord.xy-vec2(0.5,0.5))/(1.10*shift))+vec2(0.5,0.5)).rgba * 0.4 +
			texture2D(texture1,((ex_TexCoord.xy-vec2(0.5,0.5))/(1.15*shift))+vec2(0.5,0.5)).rgba * 0.1)/(1.0+0.7+0.4+0.1);

		out_FragColor.rgb *= (sin(t*14.0)+1.7)*3.0;
	}

	out_FragColor.rgb *= 0.25;
	out_FragColor.a = 0.15;*/
}
