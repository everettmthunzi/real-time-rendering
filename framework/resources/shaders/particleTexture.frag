#version 420

uniform sampler2D sampler;
in vec2 vertex_uv;

layout (location = 0) out vec4 fragment_color;

//calculate diffuse color part (rgb)
vec3 diffuseLighting(in vec3 N, in vec3 L){

	float diffuseTerm = max(dot(N,L),0);
	return pass_Color * diffuse * diffuseTerm;

}

void main() {
	vec4 looked_up_color = texture(sampler, vertex_uv);
	if (0.0 == looked_up_color.a) {
		discard;
	}

	fragment_color = vec4(texture(sampler, vertex_uv).rgb, looked_up_color.a*0.5);
}