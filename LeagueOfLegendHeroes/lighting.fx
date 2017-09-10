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
// Lighting
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Lighting_Pass_0_Model : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
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

VS_OUTPUT Lighting_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
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
   
   return( Output );
   
}




struct PS_INPUT
{
   float3 mDiffuse : TEXCOORD1;
   float3 mViewDir : TEXCOORD2;
   float3 mReflection : TEXCOORD3;
};


float4 Lighting_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
   float3 diffuse = saturate(Input.mDiffuse);
   float3 reflection = normalize(Input.mReflection);
   float3 viewDir = normalize(Input.mViewDir);
   
   float3 specular = 0;
   if (diffuse.x > 0)
   {
      specular = saturate(dot(reflection, -viewDir));
      specular = pow(specular, 20.0f);
   }
   
   float3 ambient = float3(0.1f, 0.1f, 0.1f); 
   
   return float4( ambient + specular + diffuse, 1 );
   
}




//--------------------------------------------------------------//
// Technique Section for Lighting
//--------------------------------------------------------------//
technique Lighting
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Lighting_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Lighting_Pass_0_Pixel_Shader_ps_main();
   }

}

