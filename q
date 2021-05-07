[1mdiff --git a/HighLo/assets/shaders/DefaultShader.glsl b/HighLo/assets/shaders/DefaultShader.glsl[m
[1mindex 58dced6..1ab2e88 100644[m
[1m--- a/HighLo/assets/shaders/DefaultShader.glsl[m
[1m+++ b/HighLo/assets/shaders/DefaultShader.glsl[m
[36m@@ -59,8 +59,6 @@[m [mconst int LightCount = 1;[m
 // Constant normal incidence Fresnel factor for all dielectrics.[m
 const vec3 Fdielectric = vec3(0.04);[m
 [m
[31m-const float ao = 1.0;[m
[31m-[m
 struct Light {[m
 	vec3 Direction;[m
 	vec3 Radiance;[m
[36m@@ -79,7 +77,11 @@[m [min VS_TO_PS[m
 [m
 out vec4 out_Color;[m
 [m
[31m-layout(binding = 0) uniform sampler2D DiffuseTextureSampler;[m
[32m+[m[32mlayout(binding = 0) uniform sampler2D AlbedoTextureSampler;[m
[32m+[m[32mlayout(binding = 1) uniform sampler2D NormalTextureSampler;[m
[32m+[m[32mlayout(binding = 2) uniform sampler2D MetallicTextureSampler;[m
[32m+[m[32mlayout(binding = 3) uniform sampler2D RoughnessTextureSampler;[m
[32m+[m[32mlayout(binding = 4) uniform sampler2D aoTextureSampler;[m
 [m
 layout (std140, binding = 2) uniform MaterialDataBuffer[m
 {[m
[36m@@ -88,9 +90,26 @@[m [mlayout (std140, binding = 2) uniform MaterialDataBuffer[m
     float   u_Metallic;[m
 };[m
 [m
[31m-vec4 CalculateTextureColor()[m
[32m+[m[32mvec3 getNormalFromMap()[m
[32m+[m[32m{[m
[32m+[m[32m    vec3 tangentNormal = texture(NormalTextureSampler, PS_Input.UV).xyz * 2.0 - 1.0;[m
[32m+[m
[32m+[m[32m    vec3 Q1  = dFdx(PS_Input.WorldPosition);[m
[32m+[m[32m    vec3 Q2  = dFdy(PS_Input.WorldPosition);[m
[32m+[m[32m    vec2 st1 = dFdx(PS_Input.UV);[m
[32m+[m[32m    vec2 st2 = dFdy(PS_Input.UV);[m
[32m+[m
[32m+[m[32m    vec3 N   = normalize(PS_Input.Normal);[m
[32m+[m[32m    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);[m
[32m+[m[32m    vec3 B  = -normalize(cross(N, T));[m
[32m+[m[32m    mat3 TBN = mat3(T, B, N);[m
[32m+[m
[32m+[m[32m    return normalize(TBN * tangentNormal);[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvec4 CalculateAlbedoTextureColor()[m
 {[m
[31m-	return u_Color * texture(DiffuseTextureSampler, PS_Input.UV);[m
[32m+[m	[32mreturn u_Color * texture(AlbedoTextureSampler, PS_Input.UV);[m
 }[m
 [m
 float DistributionGGX(vec3 N, vec3 H, float roughness)[m
[36m@@ -135,12 +154,16 @@[m [mvec3 fresnelSchlick(float cosTheta, vec3 F0)[m
 [m
 void main()[m
 {[m
[31m-    vec4 TextureColor = CalculateTextureColor();[m
[32m+[m[32m    vec4  AlbedoColor    = CalculateAlbedoTextureColor();[m
[32m+[m[32m    float MetallicValue  = texture(MetallicTextureSampler, PS_Input.UV).r;[m
[32m+[m[32m    float RoughnessValue = texture(RoughnessTextureSampler, PS_Input.UV).r;[m
[32m+[m[32m    float aoValue        = texture(aoTextureSampler, PS_Input.UV).r;[m
[32m+[m
 [m
[31m-	vec3 N = normalize(PS_Input.Normal);[m
[32m+[m	[32mvec3 N = getNormalFromMap();[m
     vec3 V = normalize(PS_Input.CameraPosition - PS_Input.WorldPosition);[m
 [m
[31m-	vec3 F0 = mix(Fdielectric, vec3(TextureColor), u_Metallic);[m
[32m+[m	[32mvec3 F0 = mix(Fdielectric, vec3(AlbedoColor), MetallicValue);[m
 [m
     vec3 Lo = vec3(0.0);[m
 [m
[36m@@ -152,8 +175,8 @@[m [mvoid main()[m
     vec3 radiance = PS_Input.LightColor * attenuation;[m
 [m
 	// Cook-Torrance BRDF[m
[31m-    float NDF = DistributionGGX(N, H, u_Roughness);   [m
[31m-    float G   = GeometrySmith(N, V, L, u_Roughness);      [m
[32m+[m[32m    float NDF = DistributionGGX(N, H, RoughnessValue);[m[41m   [m
[32m+[m[32m    float G   = GeometrySmith(N, V, L, RoughnessValue);[m[41m      [m
     vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);[m
            [m
     vec3 nominator    = NDF * G * F; [m
[36m@@ -169,15 +192,15 @@[m [mvoid main()[m
     // multiply kD by the inverse metalness such that only non-metals [m
     // have diffuse lighting, or a linear blend if partly metal (pure metals[m
     // have no diffuse light).[m
[31m-    kD *= 1.0 - u_Metallic;[m
[32m+[m[32m    kD *= 1.0 - MetallicValue;[m
 [m
     // scale light by NdotL[m
     float NdotL = max(dot(N, L), 0.0);[m
 [m
     // add to outgoing radiance Lo[m
[31m-    Lo += (kD * vec3(TextureColor) / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again[m
[32m+[m[32m    Lo += (kD * vec3(AlbedoColor) / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again[m
 [m
[31m-    vec3 ambient = vec3(0.03) * vec3(TextureColor) * ao;[m
[32m+[m[32m    vec3 ambient = vec3(0.03) * vec3(AlbedoColor) * aoValue;[m
     vec3 color = ambient + Lo;[m
 [m
     // HDR tonemapping[m
[36m@@ -185,5 +208,5 @@[m [mvoid main()[m
     // gamma correct[m
     color = pow(color, vec3(1.0 / 2.2)); [m
 [m
[31m-    out_Color = vec4(color, TextureColor.a);[m
[32m+[m[32m    out_Color = vec4(color, AlbedoColor.a);[m
 } [m
[1mdiff --git a/HighLo/assets/textures/rustediron2_metallic.png b/HighLo/assets/textures/rustediron2_metallic.png[m
[1mdeleted file mode 100644[m
[1mindex 76ca751..0000000[m
Binary files a/HighLo/assets/textures/rustediron2_metallic.png and /dev/null differ
[1mdiff --git a/HighLo/assets/textures/rustediron_albedo.png b/HighLo/assets/textures/rustediron_albedo.png[m
[1mdeleted file mode 100644[m
[1mindex 91896ad..0000000[m
Binary files a/HighLo/assets/textures/rustediron_albedo.png and /dev/null differ
[1mdiff --git a/HighLo/assets/textures/rustediron_normal.png b/HighLo/assets/textures/rustediron_normal.png[m
[1mdeleted file mode 100644[m
[1mindex cc191ec..0000000[m
Binary files a/HighLo/assets/textures/rustediron_normal.png and /dev/null differ
[1mdiff --git a/HighLo/assets/textures/rustediron_roughness.png b/HighLo/assets/textures/rustediron_roughness.png[m
[1mdeleted file mode 100644[m
[1mindex c5cecf7..0000000[m
Binary files a/HighLo/assets/textures/rustediron_roughness.png and /dev/null differ
[1mdiff --git a/HighLo/vendor/assimp b/HighLo/vendor/assimp[m
[1m--- a/HighLo/vendor/assimp[m
[1m+++ b/HighLo/vendor/assimp[m
[36m@@ -1 +1 @@[m
[31m-Subproject commit ec9136dca7e10a985bab63d34c003601dcd8bc22[m
[32m+[m[32mSubproject commit ec9136dca7e10a985bab63d34c003601dcd8bc22-dirty[m
[1mdiff --git a/Sandbox/src/Sandbox.cpp b/Sandbox/src/Sandbox.cpp[m
[1mindex 4cda2ef..fcd9bee 100644[m
[1m--- a/Sandbox/src/Sandbox.cpp[m
[1m+++ b/Sandbox/src/Sandbox.cpp[m
[36m@@ -10,17 +10,20 @@[m [mvoid Sandbox::OnInitialize()[m
 	[m
 	ImGuiRenderer::ShouldDisplayDebugInformation(true);[m
 [m
[31m-	m_CabinTexture = Texture2D::LoadFromFile("assets/textures/WoodenCabin.jpg");[m
[31m-	m_CabinModel = AssetLoader::LoadStaticModel("assets/models/WoodenCabin.obj", false);[m
[31m-	m_CabinModel.AddTexture(m_CabinTexture);[m
[32m+[m	[32mauto albedo		= Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Albedo.jpg");[m
[32m+[m	[32mauto normal		= Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Normal.jpg");[m
[32m+[m	[32mauto metallic	= Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Metallic.jpg");[m
[32m+[m	[32mauto roughness	= Texture2D::LoadFromFile("assets/textures/PBR_Sphere_Roughness.jpg");[m
[32m+[m	[32mauto ao			= Texture2D::CreateFromColor({ 255, 255, 255 });[m
 [m
[31m-	m_CabinTransform.Rotate(180.0f, { 0, 1, 0 });[m
[31m-	m_CabinTransform.Scale(0.3f);[m
[31m-	m_CabinTransform.Translate({ 20, -5, 75 });[m
[32m+[m	[32mm_PBR_Sphere = AssetLoader::LoadStaticModel("assets/models/PBR_Sphere.dae").GetMesh(0);[m
[32m+[m	[32mm_PBR_Sphere->material->AddTexture(albedo);[m
[32m+[m	[32mm_PBR_Sphere->material->AddTexture(normal);[m
[32m+[m	[32mm_PBR_Sphere->material->AddTexture(metallic);[m
[32m+[m	[32mm_PBR_Sphere->material->AddTexture(roughness);[m
[32m+[m	[32mm_PBR_Sphere->material->AddTexture(ao);[m
 [m
[31m-	m_Sphere = MeshFactory::CreateSphere(1.0f);[m
[31m-	m_Sphere->material->Properties.m_RenderProperties.m_Color = { 1, 0, 0, 1 };[m
[31m-	m_Sphere->material->AddTexture(Texture2D::CreateFromColor({ 255, 255, 255 }));[m
[32m+[m	[32mm_Light.Position = { 12, 3, 0 };[m
 [m
 	HL_TRACE("Sandbox Initialized");[m
 }[m
[36m@@ -30,14 +33,15 @@[m [mvoid Sandbox::OnUpdate(Timestep timestep)[m
 	m_Camera->Update();[m
 [m
 	Renderer::ClearScreenBuffers();[m
[31m-	Renderer::ClearScreenColor(glm::vec4(0.02f, 0.02f, 0.02f, 1.0f));[m
[32m+[m	[32mRenderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));[m
 [m
[31m-	CoreRenderer::BeginScene(*m_Camera);[m
[32m+[m	[32mCoreRenderer::BeginScene(*m_Camera, m_Light);[m
 [m
[31m-	CoreRenderer::DrawMesh(m_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f));[m
[31m-		[m
[31m-	for (uint64 i = 0; i < m_CabinModel.GetMeshCount(); i++)[m
[31m-		CoreRenderer::DrawMesh(m_CabinModel.GetMesh(i), m_CabinTransform);[m
[32m+[m	[32mstatic float y_rot = 0.0f;[m
[32m+[m	[32my_rot += 0.0004f * timestep;[m
[32m+[m
[32m+[m	[32mCoreRenderer::DrawCube(Transform::FromPosition(m_Light.Position));[m
[32m+[m	[32mCoreRenderer::DrawMesh(m_PBR_Sphere, Transform::FromPosition({ 6, -2, 6 }).Scale(3.0f).Rotate(y_rot, { 0, 1, 0 }));[m
 [m
 	CoreRenderer::EndScene();[m
 }[m
[36m@@ -55,13 +59,13 @@[m [mvoid Sandbox::OnEvent(Event& e)[m
 [m
 void Sandbox::OnImGuiRender()[m
 {[m
[31m-	ImGuiRenderer::StartWindow("Material Editing");[m
[32m+[m	[32m/*ImGuiRenderer::StartWindow("Material Editing");[m
 	ImGuiRenderer::BeginChild("Material Properties", 400, 400);[m
 [m
[31m-	ImGuiRenderer::ColorPicker("Color", m_Sphere->material->Properties.m_RenderProperties.m_Color);[m
[31m-	ImGuiRenderer::SliderFloat("Roughness", m_Sphere->material->Properties.m_RenderProperties.m_Roughness, 0, 1);[m
[31m-	ImGuiRenderer::SliderFloat("Metallic", m_Sphere->material->Properties.m_RenderProperties.m_Metallic, 0, 1);[m
[32m+[m	[32mImGuiRenderer::ColorPicker("Color", m_PBR_Sphere->material->Properties.m_RenderProperties.m_Color);[m
[32m+[m	[32mImGuiRenderer::SliderFloat("Roughness", m_PBR_Sphere->material->Properties.m_RenderProperties.m_Roughness, 0, 1);[m
[32m+[m	[32mImGuiRenderer::SliderFloat("Metallic", m_PBR_Sphere->material->Properties.m_RenderProperties.m_Metallic, 0, 1);[m
 [m
 	ImGuiRenderer::EndChild();[m
[31m-	ImGuiRenderer::EndWindow();[m
[32m+[m	[32mImGuiRenderer::EndWindow();*/[m
 }[m
[1mdiff --git a/Sandbox/src/Sandbox.h b/Sandbox/src/Sandbox.h[m
[1mindex 5763e33..a86a93b 100644[m
[1m--- a/Sandbox/src/Sandbox.h[m
[1m+++ b/Sandbox/src/Sandbox.h[m
[36m@@ -13,13 +13,8 @@[m [mpublic:[m
 	void OnImGuiRender() override;[m
 [m
 private:[m
[32m+[m	[32mLight m_Light;[m
 	Ref<FPSCamera> m_Camera;[m
[31m-	Ref<Mesh> m_Mesh;[m
[31m-[m
[31m-	Ref<Material> m;[m
[31m-	Ref<Texture> m_CabinTexture;[m
[31m-	Model m_CabinModel;[m
[31m-	Transform m_CabinTransform;[m
[31m-[m
[31m-	Ref<Mesh> m_Sphere;[m
[32m+[m[41m	[m
[32m+[m	[32mRef<Mesh> m_PBR_Sphere;[m
 };[m
