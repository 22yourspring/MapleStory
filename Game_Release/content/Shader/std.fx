
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

//Shader - ���� �������� ����ϴ� ���̴�
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
    // input ���� ���� ��ġ������ ������ǥ�� ���� ��ȯ�Ѵ�.
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
// Std Shader - 2D ǥ�� ���̴�
// AlphaBlend = true
// g_tex_0 : diffuse texture
// Animation2D ������� ������ �Ұ����� �ƴ���
//==================================
VS_OUTPUT VS_Std2D(VS_INPUT _input)
{
    // input ���� ���� ��ġ������ ������ǥ�� ���� ��ȯ�Ѵ�.
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
        // �������� ������ UV ���� �ȼ��� ������� ��ġ�� ã�� ������ Ȱ���Ѵ�.
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
// Collider Shader - �浹ü�� �׸��� ���̴�
// AlphaBlend = false
// g_int[0] : true(�浹��, ������), false(�浹x, �ʷϻ�)
//=======================================================
VS_OUTPUT VS_Collider(VS_INPUT _input)
{
    // input ���� ���� ��ġ������ ������ǥ�� ���� ��ȯ�Ѵ�.
    VS_OUTPUT output = (VS_OUTPUT) 0.f;
   
    output.vOutPos = mul(float4(_input.vPos, 1.f), matWVP);
    output.vOutColor = _input.vColor;
    output.vOutUV = _input.vUV;

    return output;
}

float4 PS_Collider(VS_OUTPUT _input) : SV_Target
{        
    if(g_int[0])
    { // �浹 ��
        return float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    { // �浹 x
        return float4(0.2f, 1.f, 0.2f, 1.f);        
    }
}


VS_OUTPUT VS_FADE(VS_INPUT _input)
{
	// input ���� ���� ��ġ������ ������ǥ�� ���� ��ȯ�Ѵ�.
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