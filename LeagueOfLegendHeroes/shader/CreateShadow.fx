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

texture ShadowMap_Tex : RenderColorTarget;
float4x4 matWorld : World;
float4x4 matLightView;
float4x4 matLightProjection : Projection;

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
      
   Output.Position = mul(Input.Position, matWorld);
   Output.Position = mul(Output.Position, matLightView);
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
   {
      VertexShader = compile vs_3_0 ShadowMapping_CreateShadow_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 ShadowMapping_CreateShadow_Pixel_Shader_ps_main();
   }

}

