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
// EdgeDetection
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string EdgeDetection_Pass_0_Model : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\ElephantBody.3ds";

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4x4 matWorldViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float4 Normal : NORMAL;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;   
};

VS_OUTPUT EdgeDetection_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorld );
   float3 fvWorldNormal = normalize(mul (Input.Normal, matWorld));
   
   Output.Position.xy += fvWorldNormal.xy * 1.5f;
   
   Output.Position = mul( Output.Position, matViewProjection );
   
   return( Output );
   
}




float4 EdgeDetection_Pass_0_Pixel_Shader_ps_main() : COLOR0
{   
   return( float4( 1.0f, 0.0f, 0.0f, 1.0f ) );
   
}




//--------------------------------------------------------------//
// Pass 1
//--------------------------------------------------------------//
string EdgeDetection_Pass_1_Model : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\ElephantBody.3ds";

float3 fvLightPosition
<
   string UIName = "fvLightPosition";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -100.00;
   float UIMax = 100.00;
> = float3( -100.00, 100.00, -100.00 );
float3 fvEyePosition
<
   string UIName = "fvEyePosition";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -100.00;
   float UIMax = 100.00;
> = float3( 0.00, 0.00, -100.00 );
float4x4 EdgeDetection_Pass_1_Vertex_Shader_matWorld : World;
float4x4 EdgeDetection_Pass_1_Vertex_Shader_matViewProjection : ViewProjection;
float4x4 EdgeDetection_Pass_1_Vertex_Shader_matWorldViewProjection : WorldViewProjection;

struct EdgeDetection_Pass_1_Vertex_Shader_VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
   
};

struct EdgeDetection_Pass_1_Vertex_Shader_VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   
};

EdgeDetection_Pass_1_Vertex_Shader_VS_OUTPUT EdgeDetection_Pass_1_Vertex_Shader_vs_main( EdgeDetection_Pass_1_Vertex_Shader_VS_INPUT Input )
{
   EdgeDetection_Pass_1_Vertex_Shader_VS_OUTPUT Output;

   Output.Position         = mul( Input.Position, EdgeDetection_Pass_1_Vertex_Shader_matWorldViewProjection );
   Output.Texcoord         = Input.Texcoord;
   
   float3 fvObjectPosition = mul( Input.Position, EdgeDetection_Pass_1_Vertex_Shader_matWorld );
   
   Output.ViewDirection    = fvEyePosition - fvObjectPosition;
   Output.LightDirection   = fvLightPosition - fvObjectPosition;
   
   Output.ViewDirection    = mul(Output.ViewDirection, EdgeDetection_Pass_1_Vertex_Shader_matViewProjection);
   Output.LightDirection   = mul(Output.LightDirection, EdgeDetection_Pass_1_Vertex_Shader_matViewProjection);
   
   Output.Normal           = mul( Input.Normal, EdgeDetection_Pass_1_Vertex_Shader_matWorldViewProjection );
      
   return( Output );
   
}



float4 fvAmbient
<
   string UIName = "fvAmbient";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.37, 0.37, 0.37, 1.00 );
float4 fvSpecular
<
   string UIName = "fvSpecular";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.49, 0.49, 0.49, 1.00 );
float4 fvDiffuse
<
   string UIName = "fvDiffuse";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.89, 0.89, 0.89, 1.00 );
float fSpecularPower
<
   string UIName = "fSpecularPower";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 1.00;
   float UIMax = 100.00;
> = float( 25.00 );
texture base_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Fieldstone.tga";
>;
sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float3 ViewDirection :   TEXCOORD1;
   float3 LightDirection:   TEXCOORD2;
   float3 Normal :          TEXCOORD3;
   
};

float4 EdgeDetection_Pass_1_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{      
   float3 fvLightDirection = normalize( Input.LightDirection );
   float3 fvNormal         = normalize( Input.Normal );
   float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
   float3 fvReflection = normalize(reflect(-fvLightDirection, fvNormal));
   float3 fvViewDirection = normalize(Input.ViewDirection);
   float fRDotV = fNDotL > 0 ? max(0.0f, dot(fvReflection, fvViewDirection)) : 0.0f;
   
   float4 fvBaseColor      = tex2D( baseMap, Input.Texcoord );
   float4 fvTotalAmbient   = fvAmbient * fvBaseColor; 
   float4 fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; 
   float4 fvTotalSpecular  = fvSpecular * pow( fRDotV, fSpecularPower );
   
   return( saturate( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular ) );
      
}



//--------------------------------------------------------------//
// Technique Section for EdgeDetection
//--------------------------------------------------------------//
technique EdgeDetection
{
   pass Pass_0
   {
      CULLMODE = CW;
      ZWRITEENABLE = FALSE;

      VertexShader = compile vs_3_0 EdgeDetection_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 EdgeDetection_Pass_0_Pixel_Shader_ps_main();
   }

   pass Pass_1
   {
      CULLMODE = CCW;
      ZWRITEENABLE = TRUE;

      VertexShader = compile vs_3_0 EdgeDetection_Pass_1_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 EdgeDetection_Pass_1_Pixel_Shader_ps_main();
   }

}

