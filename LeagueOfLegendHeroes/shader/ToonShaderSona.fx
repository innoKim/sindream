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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "..\\..\\..\\Documents\\Visual Studio 2017\\Projects\\LOLH Test2\\Dx3D\\Sona.x";

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
   float2 UV : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Diffuse : TEXCOORD1;
   float2 UV : TEXCOORD0;
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorldViewProjection );
   
   float3 objectLightPos = mul( LightPos, matWorldInverse );
   
   float3 lightDir = normalize(Input.Position.xyz - objectLightPos.xyz);
   
   Output.Diffuse = dot(-lightDir, normalize(Input.Normal));
   Output.UV = Input.UV;
   
   return( Output );
   
}




float3 SurfaceColor;
texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\Documents\\Visual Studio 2017\\Projects\\LOLH Test2\\Dx3D\\sona.dds";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

struct PS_INPUT 
{
   float2 UV : TEXCOORD0;
   float3 Diffuse : TEXCOORD1;
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   float4 albedo = tex2D(DiffuseSampler, Input.UV);
   float3 diffuse = albedo.rgb * saturate( Input.Diffuse );

   albedo = ceil(albedo * 5) / 5.0f;
   
   return( float4( albedo.xyz, 1.0f ) );
   
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

