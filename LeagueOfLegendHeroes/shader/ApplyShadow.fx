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

float4 vLightPos;
float4 vCameraPos;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal :   NORMAL;
   float2 UV :       TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position :         POSITION0;
   float2 UV :                TEXCOORD0;
   float3 ViewDirection : TEXCOORD1;
   float3 LightDirection : TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   float4 ClipPosition :    TEXCOORD4;
};

VS_OUTPUT ShadowMapping_ApplyShadow_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   float4 worldPosition = mul(Input.Position, matWorld);
   Output.Position = mul(worldPosition, matViewProjection);
   
   Output.UV = Input.UV;
   
   float3 fvObjectPosition = mul( Input.Position, matWorld );
   
   Output.ViewDirection    = vCameraPos - fvObjectPosition;
   Output.LightDirection   = vLightPos - fvObjectPosition;
   
   Output.ViewDirection    = mul(Output.ViewDirection, (float3x3)matViewProjection);
   Output.LightDirection   = mul(Output.LightDirection, (float3x3)matViewProjection);
   
   Output.Normal = mul(Input.Normal, (float3x3)matWorld);
   Output.Normal = mul(Output.Normal, (float3x3)matViewProjection );

   Output.ClipPosition     = mul(worldPosition, matLightView);
   Output.ClipPosition     = mul(Output.ClipPosition, matLightProjection);
      
   return( Output );
   
}




float4 fvAmbient = float4( 0.37, 0.37, 0.37, 1.00 );
float4 fvSpecular = float4( 0.49, 0.49, 0.49, 1.00 );
float4 fvDiffuse = float4( 0.89, 0.89, 0.89, 1.00 );
float fSpecularPower = float( 25.00 );
float fLightWeight = 1.0f;
texture ShadowMap_Tex;
sampler2D ShadowSampler = sampler_state
{
   Texture = (ShadowMap_Tex);
};
texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};
bool bTexture = true;

struct PS_INPUT
{
   float2 UV :              TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   float4 ClipPosition :    TEXCOORD4;
};

float4 ShadowMapping_ApplyShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   float depth = Input.ClipPosition.z / Input.ClipPosition.w;
   
   float2 uv = Input.ClipPosition.xy / Input.ClipPosition.w;
   
   uv.y = -uv.y;
   
   uv = uv * 0.5 + 0.5;

   uv = saturate(uv);

   float shadowMapDepth = tex2D(ShadowSampler, uv).r;
   
   float colorWeight = 1.0f * fLightWeight;
   
   if (depth > shadowMapDepth + 0.001f)
   {
      colorWeight = 0.5f;
   }
   
   float3 fvLightDirection = normalize( Input.LightDirection );
   float3 fvNormal         = normalize( Input.Normal );
   float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
   float3 fvReflection = normalize(reflect(-fvLightDirection, fvNormal));
   float3 fvViewDirection = normalize(Input.ViewDirection);
   float fRDotV = fNDotL > 0 ? max(0.0f, dot(fvReflection, fvViewDirection)) : 0.0f;
   
   float4 fvBaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

   if (bTexture)
   {
      fvBaseColor = tex2D( DiffuseSampler, Input.UV );
   }

   float4 fvTotalAmbient   = fvAmbient * fvBaseColor; 
   float4 fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; 
   float4 fvTotalSpecular  = fvSpecular * pow( fRDotV, fSpecularPower ) * 0.3f;
   
   return( saturate( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular ) * colorWeight );
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

