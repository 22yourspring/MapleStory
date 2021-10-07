
cbuffer TransformMatrix : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProj;
    row_major matrix matWV;
    row_major matrix matWVP;
}

cbuffer SHADER_PARAM : register(b1)
{
    int4     g_int;   
    float4   g_float;
    float2x4 g_vec2;
    float4x4 g_vec4;    
}

cbuffer ANIM2D : register(b2)
{    
    float2 g_vLT;
    float2 g_vSize;
    int g_bAnim2dUse;
    int g_iPadding[3];
}

cbuffer GLOBAL : register(b4)
{
	float g_fDT;
	float g_fAccTime;
	float g_fWidth;
	float g_fHeight;
	int   g_iLightCount;
	int   g_iSom[3];
};


Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);




// Animation Texture 
Texture2D g_tex_Anim : register(t16);


SamplerState g_sam_0 : register(s0);



//===========================================
// 

//Shader - 정점 색상으로 출력하는 쉐이더
// AlphaBlend = false
//===========================================
struct VS_INPUT
{
    float3 vPos : POSITION;    
    float4 vColor : COLOR;
	float2 vUV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
	float2 vOutUV : TEXCOORD;
};

VS_OUTPUT VS_Color(VS_INPUT _input)
{
    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
    VS_OUTPUT output = (VS_OUTPUT) 0.f;       
    output.vOutPos = mul(float4(_input.vPos, 1.f), matWVP);
    output.vOutColor = _input.vColor;
    return output;
}

float4 PS_Color(VS_OUTPUT _input) : SV_Target
{     
    return _input.vOutColor;
}


//=================================
// Std Shader - 2D 표준 쉐이더
// AlphaBlend = true
// g_tex_0 : diffuse texture
// Animation2D 기반으로 렌더링 할것인지 아닌지
//==================================
VS_OUTPUT VS_Std2D(VS_INPUT _input)
{
    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
   
    output.vOutPos = mul(float4(_input.vPos, 1.f), matWVP);
    output.vOutColor = _input.vColor;
    output.vOutUV = _input.vUV;

    return output;
}

float4 PS_Std2D(VS_OUTPUT _input) : SV_Target
{
    float4 vCol = (float4) 0.f;
    
    if(g_bAnim2dUse)
    {
        // 정점에서 가져온 UV 값을 픽셀의 상대적인 위치를 찾는 비율로 활용한다.
        float2 vAnimUV = float2(g_vLT.x + (g_vSize.x * _input.vOutUV.x), g_vLT.y + (g_vSize.y * _input.vOutUV.y));
        vCol = g_tex_Anim.Sample(g_sam_0, vAnimUV);
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vOutUV);
    }

    if (0.f == vCol.a)
        clip(-1);
//	vCol += float4(0.15, 0.15, 0.15, 0);
	//vCol *= float4(1.2, 1.2, 1.2, 1);

    return vCol;
}


//======================================================
// Collider Shader - 충돌체를 그리는 쉐이더
// AlphaBlend = false
// g_int[0] : true(충돌중, 빨간색), false(충돌x, 초록색)
//=======================================================
VS_OUTPUT VS_Collider(VS_INPUT _input)
{
    // input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
   
    output.vOutPos = mul(float4(_input.vPos, 1.f), matWVP);
    output.vOutColor = _input.vColor;
    output.vOutUV = _input.vUV;

    return output;
}

float4 PS_Collider(VS_OUTPUT _input) : SV_Target
{        
    if(g_int[0])
    { // 충돌 중
        return float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    { // 충돌 x
        return float4(0.2f, 1.f, 0.2f, 1.f);        
    }
}


VS_OUTPUT VS_FADE(VS_INPUT _input)
{
	// input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
	VS_OUTPUT output = (VS_OUTPUT) 0.f;

	output.vOutPos = mul(float4(_input.vPos, 1.f), matWVP);
	output.vOutColor = _input.vColor;
	output.vOutUV = _input.vUV;

	return output;
}

float4 PS_FADE(VS_OUTPUT _input) : SV_Target
{
	float4 vCol = (float4) 0.f;

	if (vCol.a != 1.f)
	{
		float2 vAnimUV = float2(g_vLT.x + (g_vSize.x * _input.vOutUV.x), g_vLT.y + (g_vSize.y * _input.vOutUV.y));
		vCol = g_tex_Anim.Sample(g_sam_0, vAnimUV);
		vCol.a *= g_fAccTime;
	}

	if (0.f == vCol.a)
		clip(-1);

	return vCol;
}