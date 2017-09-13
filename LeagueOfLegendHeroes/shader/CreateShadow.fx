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
// CreateShadow
//--------------------------------------------------------------//
string ShadowMapping_CreateShadow_Model : ModelData = ".\\ShaderProgramming\\10_ShadowMapping\\Torus.x";

texture ShadowMap_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {2048,2048};
   string Format="D3DFMT_R32F";
   float  ClearDepth=1.000000;
   int    ClearColor=-1;
>;
float4x4 matWorld : World;
float4x4 matLightView;
float4x4 matLightProjection : Projection;

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
};

struct VS_OUTPUT
{
   float4 Position : POSITION0;
   float4 ClipPosition : TEXCOORD1;
};

VS_OUTPUT ShadowMapping_CreateShadow_Vertex_Shader_vs_main(VS_INPUT Input)
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
   
   Output.Position = mul(Input.Position, matWorld);
   Output.Position = mul(Output.Position, lightViewMatrix);
   Output.Position = mul(Output.Position, matLightProjection);
   
   Output.ClipPosition = Output.Position;
   
   return Output;
}
struct PS_INPUT
{
   float4 ClipPosition : TEXCOORD1;
};

float4 ShadowMapping_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   float depth = Input.ClipPosition.z / Input.ClipPosition.w;
   return float4(depth.xxx, 1);
   
}




//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass CreateShadow
   <
      string Script = "RenderColorTarget0 = ShadowMap_Tex;"
                      "ClearColor = (255, 255, 255, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_3_0 ShadowMapping_CreateShadow_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 ShadowMapping_CreateShadow_Pixel_Shader_ps_main();
   }

}

