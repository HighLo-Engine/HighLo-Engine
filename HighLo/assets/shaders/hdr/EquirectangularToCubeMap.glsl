#version 450 core
#pragma shader:compute

const float PI = 3.141592;

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_CubeMap;
layout(binding = 1) uniform sampler2D u_EquirectangularTex;

vec3 GetCubeMapTexCoord()
{
	vec2 st = gl_GlobalInvocationID.xy / vec2(imageSize(o_CubeMap));
	vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);
	
	vec3 result;
	if (gl_GlobalInvocationID.z == 0)
	{
		result = vec3(1.0, uv.y, -uv.x);
	}
	else if (gl_GlobalInvocationID.z == 1)
	{
		result = vec3(-1.0, uv.y, uv.x);
	}
	else if (gl_GlobalInvocationID.z == 2)
	{
		result = vec3(uv.x, 1.0, -uv.y);
	}
	else if (gl_GlobalInvocationID.z == 3)
	{
		result = vec3(uv.x, -1.0f, uv.y);
	}
	else if (gl_GlobalInvocationID.z == 4)
	{
		result = vec3(uv.x, uv.y, 1.0);
	}
	else if (gl_GlobalInvocationID.z == 5)
	{
		result = vec3(-uv.x, uv.y, -1.0);
	}
	
	return normalize(result);
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
	vec3 cubeTextureCoord = GetCubeMapTexCoord();
	
	float phi = atan(cubeTextureCoord.z, cubeTextureCoord.x);
	float theta = acos(cubeTextureCoord.y);
	vec2 uv = vec2(phi / (2.0 * PI) + 0.5, theta / PI);
	
	vec4 color = texture(u_EquirectangularTex, uv);
	imageStore(o_CubeMap, ivec3(gl_GlobalInvocationID), color);
}