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
// Skybox
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Skybox_Pass_0_Model : ModelData = "..\\Downloads\\RavioliGameTools_v2.9\\Ezreal\\";

struct VS_INPUT 
{
   float4 Position : POSITION0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Position3D : TEXCOORD0;
};

float4x4 matViewProjection : ViewProjection;
float4   vCameraPos : ViewPosition;

VS_OUTPUT Skybox_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
    VS_OUTPUT Output;
    
    float4x4 world = float4x4(
              float4(1,0,0,0),
              float4(0,1,0,0),
              float4(0,0,1,0),
              float4(vCameraPos.xyz, 1)
          );
    
    Output.Position = mul(Input.Position, world);
    Output.Position = mul(Output.Position, matViewProjection);
    Output.Position3D = Input.Position;
    
    return Output;
}




texture EnvironmentMap_Tex
<
   string ResourceName = ".\\ShaderProgramming\\08_EnvironmentMapping\\Snow_ENV.dds";
>;
samplerCUBE EnvironmentSampler = sampler_state
{
   Texture = (EnvironmentMap_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};

struct PS_INPUT
{
   float3 Position3D : TEXCOORD0;
};


float4 Skybox_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   return texCUBE(EnvironmentSampler, Input.Position3D);
}




//--------------------------------------------------------------//
// Technique Section for Skybox
//--------------------------------------------------------------//
technique Skybox
{
   pass Pass_0
   {
      CULLMODE = CW;
      ZENABLE = FALSE;

      VertexShader = compile vs_3_0 Skybox_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 Skybox_Pass_0_Pixel_Shader_ps_main();
   }

}

