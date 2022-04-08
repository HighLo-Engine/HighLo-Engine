// Source: https://www.shadertoy.com/view/llSSDR

#version 450 core
#pragma shader:compute

#define PI 3.14159265359

layout(binding = 0, rgba32f) restrict writeonly uniform imageCube o_CubeMap;

layout(push_constant) uniform Uniforms
{
	vec3 TurbidityAzimuthInclination;
} u_Uniforms;

vec3 GetCubeMapTexCoord()
{
    vec2 st = gl_GlobalInvocationID.xy / vec2(imageSize(o_CubeMap));
    vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);

    vec3 ret;
    if (gl_GlobalInvocationID.z == 0)      ret = vec3(  1.0, uv.y, -uv.x);
    else if (gl_GlobalInvocationID.z == 1) ret = vec3( -1.0, uv.y,  uv.x);
    else if (gl_GlobalInvocationID.z == 2) ret = vec3( uv.x,  1.0, -uv.y);
    else if (gl_GlobalInvocationID.z == 3) ret = vec3( uv.x, -1.0,  uv.y);
    else if (gl_GlobalInvocationID.z == 4) ret = vec3( uv.x, uv.y,   1.0);
    else if (gl_GlobalInvocationID.z == 5) ret = vec3(-uv.x, uv.y,  -1.0);
    return normalize(ret);
}


float saturatedDot(vec3 a, vec3 b )
{
	return max(dot(a, b), 0.0);   
}

vec3 YxyToXYZ(vec3 yxy)
{
	float Y = yxy.r;
	float x = yxy.g;
	float y = yxy.b;

	float X = x * (Y / y);
	float Z = (1.0 - x - y) * (Y / y);

	return vec3(X, Y, Z);
}

vec3 XYZToRGB(vec3 xyz)
{
	// CIE/E
	mat3 M = mat3
	(
		 2.3706743, -0.9000405, -0.4706338,
		-0.5138850,  1.4253036,  0.0885814,
 		 0.0052982, -0.0146949,  1.0093968
	);

	return xyz * M;
}


vec3 YxyToRGB(vec3 yxy)
{
	vec3 XYZ = YxyToXYZ(yxy);
	vec3 RGB = XYZToRGB(XYZ);
	return RGB;
}

void calculatePerezDistribution(float t, vec3 A, vec3 B, vec3 C, vec3 D, vec3 E)
{
	A = vec3( 0.1787 * t - 1.4630, -0.0193 * t - 0.2592, -0.0167 * t - 0.2608);
	B = vec3(-0.3554 * t + 0.4275, -0.0665 * t + 0.0008, -0.0950 * t + 0.0092);
	C = vec3(-0.0227 * t + 5.3251, -0.0004 * t + 0.2125, -0.0079 * t + 0.2102);
	D = vec3( 0.1206 * t - 2.5771, -0.0641 * t - 0.8989, -0.0441 * t - 1.6537);
	E = vec3(-0.0670 * t + 0.3703, -0.0033 * t + 0.0452, -0.0109 * t + 0.0529);
}

vec3 calculateZenithLuminanceYxy(float t, float thetaS)
{
	float chi  	 = ( 4.0 / 9.0 - t / 120.0 ) * ( PI - 2.0 * thetaS );
	float Yz   	 = ( 4.0453 * t - 4.9710 ) * tan( chi ) - 0.2155 * t + 2.4192;

	float theta2 = thetaS * thetaS;
    float theta3 = theta2 * thetaS;
    float T 	 = t;
    float T2 	 = t * t;

	float xz =
      ( 0.00165 * theta3 - 0.00375 * theta2 + 0.00209 * thetaS + 0.0)     * T2 +
      (-0.02903 * theta3 + 0.06377 * theta2 - 0.03202 * thetaS + 0.00394) * T +
      ( 0.11693 * theta3 - 0.21196 * theta2 + 0.06052 * thetaS + 0.25886);

    float yz =
      ( 0.00275 * theta3 - 0.00610 * theta2 + 0.00317 * thetaS + 0.0)     * T2 +
      (-0.04214 * theta3 + 0.08970 * theta2 - 0.04153 * thetaS + 0.00516) * T +
      ( 0.15346 * theta3 - 0.26756 * theta2 + 0.06670 * thetaS + 0.26688);

	return vec3(Yz, xz, yz);
}

vec3 calculatePerezLuminanceYxy(float theta, float gamma, vec3 a, vec3 b, vec3 c, vec3 d, vec3 e)
{
	return (1.0 + a * exp(b / cos( theta ))) * (1.0 + c * exp(d * gamma) + e * cos(gamma) * cos(gamma));
}

vec3 calculateSkyLuminanceRGB(vec3 s, vec3 e, float t)
{
	vec3 A, B, C, D, E;
	calculatePerezDistribution(t, A, B, C, D, E);

	float thetaS = acos(saturatedDot(s, vec3(0,1,0)));
	float thetaE = acos(saturatedDot(e, vec3(0,1,0)));
	float gammaE = acos(saturatedDot(s, e));

	vec3 Yz = calculateZenithLuminanceYxy( t, thetaS );

	vec3 fThetaGamma = calculatePerezLuminanceYxy(thetaE, gammaE, A, B, C, D, E);
	vec3 fZeroThetaS = calculatePerezLuminanceYxy(0.0,    thetaS, A, B, C, D, E);

	vec3 Yp = Yz * (fThetaGamma / fZeroThetaS);
	return YxyToRGB(Yp);
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
	vec3 cubeTC         = GetCubeMapTexCoord();
	float turbidity     = u_Uniforms.TurbidityAzimuthInclination.x;
    float azimuth       = u_Uniforms.TurbidityAzimuthInclination.y;
    float inclination   = u_Uniforms.TurbidityAzimuthInclination.z;
    vec3 sunDir     	= normalize(vec3(sin(inclination) * cos(azimuth), cos(inclination), sin(inclination) * sin(azimuth)));
    vec3 viewDir  		= cubeTC;
    vec3 skyLuminance 	= calculateSkyLuminanceRGB(sunDir, viewDir, turbidity);
    
    vec4 color = vec4(skyLuminance * 0.05, 1.0);
	imageStore(o_CubeMap, ivec3(gl_GlobalInvocationID), color);
}

