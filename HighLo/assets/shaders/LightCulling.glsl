// Sources:
// - SIGGRAPH 2011
// - https://github.com/bcrusco/Forward-Plus-Renderer
// - https://gitlab.com/chernoprojects/Hazel-dev/

#version 430 core
#pragma shader:compute

#define TILE_SIZE 16

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_InverseViewProjectionMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
};

struct PointLight
{
	vec3 Position;
	float Multiplier;
	vec3 Radiance;
	float MinRadius;
	float Radius;
	float Falloff;
	float LightSize;
	bool CastsShadows;
};

layout(std140, binding = 4) uniform PointLightData
{
	uint u_PointLightsCount;
	PointLight u_PointLights[1024];
};

layout(push_constant) uniform ScreenData
{
	ivec2 ScreenSize;
} u_ScreenData;

layout(std430, binding = 14) writeonly buffer VisibleLightIndicesBuffer
{
	int indices[];
} visibleLightIndicesBuffer;

layout(set = 0, binding = 15) uniform sampler2D u_PreDepthMap;

// Shared values between all the threads
shared uint minDepthInt;
shared uint maxDepthInt;
shared uint visibleLightCount;
shared vec4 frustumPlanes[6];
shared int visibleLightIndices[1024];

layout(local_size_x = TILE_SIZE, local_size_y = TILE_SIZE, local_size_z = 1) in;
void main()
{
	ivec2 location = ivec2(gl_GlobalInvocationID.xy);
	ivec2 itemID = ivec2(gl_LocalInvocationID.xy);
	ivec2 tileID = ivec2(gl_WorkGroupID.xy);
	ivec2 tileNumber = ivec2(gl_NumWorkGroups.xy);
	uint index = tileID.y * tileNumber.x + tileID.x;

	// Initialize shared global values for depth and light count
	if (gl_LocalInvocationIndex == 0)
	{
		minDepthInt = 0xFFFFFFFF;
		maxDepthInt = 0;
		visibleLightCount = 0;
	}

	barrier();

	// Step 1: Calculate the minimum and maximum depth values (from the depth buffer) for this group's tile
	vec2 tc = vec2(location) / u_ScreenData.ScreenSize;
	float depth = texture(u_PreDepthMap, tc).r;
	depth = u_ProjectionMatrix[3][2] / (depth + u_ProjectionMatrix[2][2]);

	uint depthInt = floatBitsToUint(depth);
	atomicMin(minDepthInt, depthInt);
	atomicMax(maxDepthInt, depthInt);

	barrier();

	// Step 2: One thread should calculate the frustum planes to be used for this tile
	if (gl_LocalInvocationIndex == 0)
	{
		// Convert the min and max back to float for the current thread
		float minDepth = uintBitsToFloat(minDepthInt);
		float maxDepth = uintBitsToFloat(maxDepthInt);

		// Steps based on tile scale
		vec2 negStep = (2.0 * vec2(tileID)) / vec2(tileNumber);
		vec2 posStep = (2.0 * vec2(tileID + ivec2(1, 1))) / vec2(tileNumber);

		// Set up starting values for planes using steps and min and max z values
		frustumPlanes[0] = vec4(1.0, 0.0, 0.0, 1.0 - negStep.x); // Left
		frustumPlanes[1] = vec4(-1.0, 0.0, 0.0, -1.0 + posStep.x); // Right
		frustumPlanes[2] = vec4(0.0, 1.0, 0.0, 1.0 - negStep.y); // Bottom
		frustumPlanes[3] = vec4(0.0, -1.0, 0.0, -1.0 + posStep.y); // Top
		frustumPlanes[4] = vec4(0.0, 0.0, -1.0, -minDepth); // Near
		frustumPlanes[5] = vec4(0.0, 0.0, 1.0, maxDepth); // Far
		
		// Transform the first four planes
		for (uint i = 0; i < 4; i++)
		{
			frustumPlanes[i] *= u_ViewProjectionMatrix;
			frustumPlanes[i] /= length(frustumPlanes[i].xyz);
		}

		// Transform the depth planes
		frustumPlanes[4] *= u_ViewMatrix;
		frustumPlanes[4] /= length(frustumPlanes[4].xyz);
		frustumPlanes[5] *= u_ViewMatrix;
		frustumPlanes[5] /= length(frustumPlanes[5].xyz);
	}

	barrier();

	// Step 3: Cull lights.
	uint threadCount = TILE_SIZE * TILE_SIZE;
	uint passCount = (u_PointLightsCount + threadCount - 1) / threadCount;
	for (uint i = 0; i < passCount; ++i)
	{
		uint lightIndex = i * threadCount + gl_LocalInvocationIndex;
		if (lightIndex >= u_PointLightsCount)
			break;

		vec4 position = vec4(u_PointLights[lightIndex].Position, 1.0f);
		float radius = u_PointLights[lightIndex].Radius;
		radius += radius * 0.3f;
		
		// Check if light radius is in frustum
		float distance = 0.0;
		for (uint j = 0; j < 6; j++)
		{
			distance = dot(position, frustumPlanes[j]) + radius;
			if (distance <= 0.0) // No intersection
				break;
		}

		// If greater than zero, then it is a visible light
		if (distance > 0.0)
		{
			// Add index to the shared array of visible indices
			uint offset = atomicAdd(visibleLightCount, 1);
			visibleLightIndices[offset] = int(lightIndex);
		}
	}
	
	barrier();

	// One thread should fill the global light buffer
	if (gl_LocalInvocationIndex == 0)
	{
		uint offset = index * 1024; // Determine bosition in global buffer
		for (uint i = 0; i < visibleLightCount; ++i)
		{
			visibleLightIndicesBuffer.indices[offset + i] = visibleLightIndices[i];
		}

		if (visibleLightCount != 1024)
		{
			// Unless we have totally filled the entire array, mark it's end with -1
			// Final shader step will use this to determine where to stop (without having to pass the light count)
			visibleLightIndicesBuffer.indices[offset + visibleLightCount] = -1;
		}
	}
}

