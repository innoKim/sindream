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
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_0_Model : ModelData = ".\\ShaderProgramming\\06_ToonShader\\teapot.x";

float4x4 matWorldViewProjection : WorldViewProjection;
float4x4 matWorldInverse : WorldInverse;

float4 LightPos
<
   string UIName = "LightPos";
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
   float3 Diffuse : TEXCOORD1;
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorldViewProjection );
   
   float3 objectLightPos = mul( LightPos, matWorldInverse );
   
   float3 lightDir = normalize(Input.Position.xyz - objectLightPos.xyz);
   
   Output.Diffuse = dot(-lightDir, normalize(Input.Normal));
   
   return( Output );
   
}




float3 SurfaceColor
<
   string UIName = "SurfaceColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 0.00, 1.00, 0.00 );

struct PS_INPUT 
{
   float3 Diffuse : TEXCOORD1;
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   float3 diffuse = saturate( Input.Diffuse );

   diffuse = ceil(diffuse * 5) / 5.0f;
   
   return( float4( SurfaceColor + diffuse.xyz, 1.0f ) );
   
}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

