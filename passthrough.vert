#version 150 core

in vec3 inp_Position;
in vec3 inp_Normal;
in vec2 inp_Texcoord;

uniform vec4 in_Color;
uniform mat4x4 projectionMatrix;
uniform mat4x4 modelviewMatrix;
uniform mat4x4 normalMatrix;

out vec3 worldPos;
out vec3 worldNormal;
out vec4 ex_Color;
out vec2 ex_TexCoord;

out float depth;

void main() {
	gl_Position = modelviewMatrix * vec4(inp_Position, 1.0);

	depth = -gl_Position.z;
	worldPos = gl_Position.xyz/gl_Position.w;

	gl_Position = projectionMatrix * gl_Position;
	
	worldNormal = normalize((normalMatrix * vec4(inp_Normal,0.0)).xyz);
	ex_Color = in_Color;
	ex_TexCoord = inp_Texcoord;

}

