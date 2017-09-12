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
// SpecularMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string SpecularMapping_Pass_0_Model : ModelData = ".\\ShaderProgramming\\05_DiffuseSpecularMapping\\Sphere.x";

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL;
   float2 UV : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 UV : TEXCOORD0;
   float3 Diffuse : TEXCOORD1;
   float3 ViewDir : TEXCOORD2;
   float3 Reflection : TEXCOORD3;
};

float4x4 matView : View;
float4x4 matProjection : Projection;
float4x4 matWorld : World;
float4   vLightPos
<
   string UIName = "vLightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );
float4   vCameraPos : ViewPosition;

VS_OUTPUT SpecularMapping_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorld );
   
   float3 lightDir = Output.Position.xyz - vLightPos.xyz;
   lightDir = normalize(lightDir);
   
   float3 cameraDir = Output.Position.xyz - vCameraPos.xyz;
   Output.ViewDir = cameraDir;
   
   Output.Position = mul( Output.Position, matView );
   Output.Position = mul( Output.Position, matProjection );
   
   float3 worldNormal = mul( Input.Normal, (float3x3)matWorld );
   worldNormal = normalize(worldNormal);
   
   Output.Diffuse = dot(-lightDir, worldNormal);
   Output.Reflection = reflect(lightDir, worldNormal);
   Output.UV = Input.UV;
   
   return( Output );
   
}




texture DiffuseMap_Tex
<
   string ResourceName = ".\\ShaderProgramming\\05_DiffuseSpecularMapping\\Fieldstone_DM.tga";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};
texture SpecularMap_Tex
<
   string ResourceName = ".\\ShaderProgramming\\05_DiffuseSpecularMapping\\fieldstone_SM.tga";
>;
sampler2D SpecularSampler = sampler_state
{
   Texture = (SpecularMap_Tex);
};

float3 vLightColor
<
   string UIName = "vLightColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 0.70, 0.70, 1.00 );

struct PS_INPUT
{
   float2 UV : TEXCOORD0;
   float3 Diffuse : TEXCOORD1;
   float3 ViewDir : TEXCOORD2;
   float3 Reflection : TEXCOORD3;
};


float4 SpecularMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
   float4 albedo = tex2D(DiffuseSampler, Input.UV);
   
   float3 diffuse = vLightColor * albedo.rgb * saturate(Input.Diffuse);
   float3 reflection = normalize(Input.Reflection);
   float3 viewDir = normalize(Input.ViewDir);
   
   float3 specular = 0;
   if (diffuse.x > 0)
   {
      specular = saturate(dot(reflection, -viewDir));
      specular = pow(specular, 20.0f);

      float4 specularIntensity = tex2D(SpecularSampler, Input.UV);
      specular *= specularIntensity.rgb * vLightColor;
   }
   
   
   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   
   return float4( ambient + specular + diffuse, 1 );
   
}




//--------------------------------------------------------------//
// Technique Section for SpecularMapping
//--------------------------------------------------------------//
technique SpecularMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 SpecularMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 SpecularMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

