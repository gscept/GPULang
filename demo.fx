//------------------------------------------------------------------------------
//  demo.fx
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------

#define FOO 5
#define BAR 4

// A couple of example variable declarations
samplerstate TestSampler
{
	Samplers = { DiffuseTexture };
	Filter = MinMagMipLinear;
	AddressU = Border;
	AddressV = Border;
	BorderColor = { 1.0f, 0.0f, 1.0f, 1.0f };
};

samplerstate FooSampler
{
	Samplers = { NormalTexture };
	Filter = MinMagMipLinear;
	AddressU = Border;
	AddressV = Border;
	BorderColor = { 1.0f, 0.0f, 0.0f, 1.0f };
};

struct Test
{
	float foo;
	float bar;
};

sampler2D NormalTexture;
sampler2D DiffuseTexture;

float TessFactorInner = 1.0f;
float TessFactorOuter = 1.0f;

const float testFloat = 5.0f;
const float constantFloatArray[5] = { 1.0f, 152314, 1.0f, 1.0f, 1.0f };
const vec2 constantVecArray[5] = { vec2(1.0f, 1.0f), vec2(2.0f, 3.0f), vec2(1.0f, 3.0f), vec2(3.0f, 1.0f), vec2(1.0f, 3.0f) };

state OpaqueState
{
	MultisampleEnabled = true;
};

state AlphaState
{
	DepthEnabled = true;
	BlendEnabled[0] = true;
	SrcBlend[0] = One;
	DstBlend[0] = DstColor;
	CullMode = Back;
	//MultisampleEnabled = true;
};

// a variable block containing a set of variables, this will instantiated only once in the effects system
// this block of variables will be shared by all other .fx files compiled during runtime with the same name and the [shared] qualifier
varblock Transforms 
{
	mat4 View;
	mat4 Projection;
	mat4 ViewProjection;
};

mat4 Model[3];

varblock Material
{
	vec2 Random = vec2(10.0f, 0.0f);
	float SpecularIntensity = float(1.0f);
	vec4 MaterialColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

//------------------------------------------------------------------------------
/**
	Simple vertex shader which transforms basic geometry so it can be used for deferred lighting.
	
	The function header here complies (and has to comply) with the AnyFX standard, although the function code is written in a specific target language.
	
	This language is compliant with GLSL
*/
shader 
void
vsStatic(in vec3 position, in vec2 uv, out vec2 UV) 
{
	gl_Position = ViewProjection * Model[1] * vec4(position, 1.0f);
	UV = uv;
}

//------------------------------------------------------------------------------
/**
*/
shader 
void
vsTessellated(in vec3 position, in vec2 uv, out vec3 Position, out vec2 UV)
{
	Position = position;
	UV = uv;
}

//------------------------------------------------------------------------------
/**
	Simple hull shader with application defined tessellation levels
*/
[inputvertices] = 3
[outputvertices] = 6

shader 
void
hsStatic(in vec3 inPosition[], in vec2 inUV[], out vec3 outPosition[], out vec2 outUV[])
{
	outPosition[gl_InvocationID] = inPosition[gl_InvocationID];
	outUV[gl_InvocationID] = inUV[gl_InvocationID];
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = TessFactorInner;
		gl_TessLevelOuter[0] = TessFactorOuter;
		gl_TessLevelOuter[1] = TessFactorOuter;
		gl_TessLevelOuter[2] = TessFactorOuter;
	}	
}

//------------------------------------------------------------------------------
/**
	Simple domain shader which just performs tessellation without displacement
*/
[inputvertices] = 6
[winding] = ccw
[topology] = triangle
[partition] = odd

shader 
void
dsStatic(in vec3 Position[], in vec2 inUV[], out vec3 outPosition, out vec2 outUV)
{
	vec3 p0 = gl_TessCoord.x * Position[0];
	vec3 p1 = gl_TessCoord.y * Position[1];
	vec3 p2 = gl_TessCoord.z * Position[2];
	vec3 pos = normalize(p0+p1+p2);
	outPosition = pos;
	gl_Position = ViewProjection * Model[1] * vec4(pos, 1);
	
	vec2 t0 = gl_TessCoord.x * inUV[0];
	vec2 t1 = gl_TessCoord.y * inUV[1];
	vec2 t2 = gl_TessCoord.z * inUV[2];
	outUV = (t0+t1+t2);
}

//------------------------------------------------------------------------------
/**
*/
[inputprimitive] = triangles
[outputprimitive] = triangle_strip
[maxvertexcount] = 3
shader 
void
gsStatic(in vec3 Position[], in vec2 inUV[], out vec2 outUV, out vec3 outTriDistance)
{
	outTriDistance = vec3(1,0,0);
	outUV = inUV[0];
	gl_Position = gl_in[0].gl_Position; EmitVertex();
	
	outTriDistance = vec3(0,1,0);
	outUV = inUV[1];
	gl_Position = gl_in[1].gl_Position; EmitVertex();
	
	outTriDistance = vec3(0,0,1);
	outUV = inUV[2];
	gl_Position = gl_in[2].gl_Position; EmitVertex();
	
	EndPrimitive();
}

//------------------------------------------------------------------------------
/**
	Simple pixel shader which writes normals and diffuse colors. 
	
	Here, we use multiple render targeting using input/output attributes.
	
	We also apply a function attribute which tells OpenGL to perform early depth testing
*/
[earlydepth]
shader 
void 
psStatic(in vec2 uv, [color0] out vec4 Color) 
{
	vec4 color = texture(DiffuseTexture, uv);	
	vec4 extColor = texture(NormalTexture, uv);
	Color = color * extColor;
}

//------------------------------------------------------------------------------
/**
*/
float
amplify(float d, const float scale, const float offset) 
{	
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

//------------------------------------------------------------------------------
/**
*/
[earlydepth]
shader 
void 
psTessellation(in vec2 uv, in vec3 TriDistance, [color0] out vec4 Color1)
{
	float d1 = min(min(TriDistance.x, TriDistance.y), TriDistance.z);
	vec4 tex1 = texture(DiffuseTexture, uv);
	Color1 = vec4(amplify(d1, 40.0f, -0.5f) * tex1.xyz, tex1.a);
}

write rgba32f image2D img;
float roll;

//------------------------------------------------------------------------------
/**
*/
[localsizex] = 16
[localsizey] = 16
shader 
void
csStatic()
{
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0f);
	float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y) * 0.1 + roll) * 0.5f;
	imageStore(img, storePos, vec4(1.0f - globalCoef, 20.0f - globalCoef, 30.0f - globalCoef, 40.0f - globalCoef));
}

//------------------------------------------------------------------------------
/**
	Two programs, they share shaders but not render states, and also provide an API-available data field.
*/
program Solid [ string Mask = "Opaque"; ]
{
	VertexShader = vsStatic();
	PixelShader = psStatic();
	RenderState = OpaqueState;
};

program Alpha [ string Mask = "Alpha"; ]
{
	VertexShader = vsStatic();
	PixelShader = psStatic();
	RenderState = AlphaState;
};


program Tessellated
{
	VertexShader = vsTessellated();
	HullShader = hsStatic();
	DomainShader = dsStatic();
	GeometryShader = gsStatic();
	PixelShader = psTessellation();
	RenderState = OpaqueState;
};


program Calculator
{
	ComputeShader = csStatic();
};
