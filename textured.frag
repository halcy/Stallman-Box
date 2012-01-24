#version 150 core

in vec3 worldPos;
in vec3 worldNormal;
in vec4 ex_Color;
in vec2 ex_TexCoord;

out vec4 out_FragColor;

uniform sampler2D texture1;

void main() {
	out_FragColor = texture2D(texture1,ex_TexCoord.xy).rgba;
	//out_FragColor.a = 1.0;
}
