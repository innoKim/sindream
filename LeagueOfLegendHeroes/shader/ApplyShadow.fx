//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// ShadowMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// ApplyShadow
//--------------------------------------------------------------//
string ShadowMapping_ApplyShadow_Model : ModelData = ".\\ShaderProgramming\\10_ShadowMapping\\Torus.x";

float4x4 matWorld : World;
float4x4 matLightView;
float4x4 matLightProjection : Projection;
float4x4 matViewProjection : ViewProjection;

float4 vLightPos
<
   string UIName = "vLightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float4 ClipPosition : TEXCOORD1;
   float Diffuse : TEXCOORD2;
};

VS_OUTPUT ShadowMapping_ApplyShadow_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   float4x4 lightViewMatrix = matLightView;
   
   float3 dirZ = -normalize(vLightPos.xyz);
   float3 up = float3(0, 1, 0);
   
   float3 dirX = cross(up, dirZ);
   float3 dirY = cross(dirZ, dirX);
   
   lightViewMatrix = float4x4(
      float4(dirX, -dot(vLightPos.xyz, dirX)),
      float4(dirY, -dot(vLightPos.xyz, dirY)),
      float4(dirZ, -dot(vLightPos.xyz, dirZ)),
      float4(0, 0, 0, 1));
   lightViewMatrix = transpose(lightViewMatrix);

   float4 worldPosition = mul(Input.Position, matWorld);
   Output.Position = mul(worldPosition, matViewProjection);
   
   Output.ClipPosition = mul(worldPosition, lightViewMatrix);
   Output.ClipPosition = mul(Output.ClipPosition, matLightProjection);
   
   float3 lightDir = normalize(worldPosition.xyz - vLightPos.xyz);
   float3 worldNormal = normalize(mul(Input.Normal, (float3x3)matWorld));
   
   Output.Diffuse = dot(-lightDir, worldNormal);
   
   return( Output );
   
}




texture ShadowMap_Tex
<
   string ResourceName = "..\\Downloads\\RavioliGameTools_v2.9\\Ezreal\\";
>;
sampler2D ShadowSampler = sampler_state
{
   Texture = (ShadowMap_Tex);
};
float4 vObjectColor
<
   string UIName = "vObjectColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.00, 1.00, 1.00, 1.00 );

struct PS_INPUT
{
   float4 ClipPosition : TEXCOORD1;
   float Diffuse : TEXCOORD2;
};

float4 ShadowMapping_ApplyShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   float3 rgb = saturate(Input.Diffuse) * vObjectColor;
   
   float currentDepth = Input.ClipPosition.z / Input.ClipPosition.w;
   
   float2 uv = Input.ClipPosition.xy / Input.ClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;
   
   if (currentDepth > shadowDepth + 0.0000125f)
   {
      rgb *= 0.5f;
   }   
   
   return( float4( rgb, 1.0f ) );
   
}




//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass ApplyShadow
   {
      VertexShader = compile vs_3_0 ShadowMapping_ApplyShadow_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 ShadowMapping_ApplyShadow_Pixel_Shader_ps_main();
   }

}

