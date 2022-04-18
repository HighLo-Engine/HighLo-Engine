#version 450 core
#pragma shader:compute

#include <EnvironmentMapping.glslh>
#include <PBR.glslh>

#define NUM_SAMPLES 1024
#define NUM_MIP_LEVELS 1
#define PARAM_LEVEL 0
#define INVERSE_NUM_SAMPLES (1.0 / float(NUM_SAMPLES))

layout(binding = 0, rgba32f) restrict writeonly uniform imageCube outputTexture[NUM_MIP_LEVELS];
layout(binding = 1) uniform samplerCube inputTexture;

#ifdef __VULKAN__
layout (push_constant) uniform Uniforms
{
	float Roughness;
} u_Uniforms;
#else
layout(binding = 2, std140) uniform Uniforms
{
	float Roughness;
} u_Uniforms;
#endif

#define PARAM_ROUGHNESS u_Uniforms.Roughness

vec2 SampleHammersley(uint i)
{
	return vec2(i * INVERSE_NUM_SAMPLES, RadicalInverse_VdC(i));
}

layout(local_size_x=32, local_size_y=32, local_size_z=1) in;
void main(void)
{
	// Make sure we won't write past output when computing higher mipmap levels.
	ivec2 outputSize = imageSize(outputTexture[PARAM_LEVEL]);
	if(gl_GlobalInvocationID.x >= outputSize.x || gl_GlobalInvocationID.y >= outputSize.y) {
		return;
	}
	
	// Solid angle associated with a single cubemap texel at zero mipmap level.
	// This will come in handy for importance sampling below.
	vec2 inputSize = vec2(textureSize(inputTexture, 0));
	float wt = 4.0 * PI / (6 * inputSize.x * inputSize.y);
	
	// Approximation: Assume zero viewing angle (isotropic reflections).
	vec3 N = GetCubeMapTexCoord(vec2(imageSize(outputTexture[PARAM_LEVEL])));
	vec3 Lo = N;
	
	vec3 S, T;
	ComputeBasisVectors(N, S, T);

	vec3 color = vec3(0);
	float weight = 0;

	// Convolve environment map using GGX NDF importance sampling.
	// Weight by cosine term since Epic claims it generally improves quality.
	for(uint i = 0; i < NUM_SAMPLES; i++)
	{
		vec2 u = SampleHammersley(i);
		vec3 Lh = TangentToWorld(SampleGGX(u.x, u.y, PARAM_ROUGHNESS), N, S, T);

		// Compute incident direction (Li) by reflecting viewing direction (Lo) around half-vector (Lh).
		vec3 Li = 2.0 * dot(Lo, Lh) * Lh - Lo;

		float cosLi = dot(N, Li);
		if(cosLi > 0.0)
		{
			// Use Mipmap Filtered Importance Sampling to improve convergence.
			// See: https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch20.html, section 20.4

			float cosLh = max(dot(N, Lh), 0.0);

			// GGX normal distribution function (D term) probability density function.
			// Scaling by 1/4 is due to change of density in terms of Lh to Li (and since N=V, rest of the scaling factor cancels out).
			float pdf = NdfGGX(cosLh, PARAM_ROUGHNESS) * 0.25;

			// Solid angle associated with this sample.
			float ws = 1.0 / (NUM_SAMPLES * pdf);

			// Mip level to sample from.
			float mipLevel = max(0.5 * log2(ws / wt) + 1.0, 0.0);

			color  += textureLod(inputTexture, Li, mipLevel).rgb * cosLi;
			weight += cosLi;
		}
	}

	color /= weight;
	imageStore(outputTexture[PARAM_LEVEL], ivec3(gl_GlobalInvocationID), vec4(color, 1.0));
}

