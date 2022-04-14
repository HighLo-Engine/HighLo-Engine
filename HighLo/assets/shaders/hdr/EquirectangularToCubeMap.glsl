#version 430 core
#pragma shader:compute

#include <EnvironmentMapping.glslh>

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_Cubemap;
layout(binding = 1) uniform sampler2D u_EquirectangularTexture;

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
	vec3 cubeTC = GetCubeMapTexCoord(vec2(imagesize(o_Cubemap)));
	
	float phi = atan(cubeTC.z, cubeTC.x);
	float theta = acos(cubeTC.y);
	vec2 uv = vec2(phi / (2.0 * PI) + 0.5, theta / PI);
	
	vec4 color = texture(u_EquirectangularTexture, uv);
	imageStore(o_Cubemap, ivec3(gl_GlobalInvocationID), color);
}

