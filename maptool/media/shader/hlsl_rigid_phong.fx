
// -------------------------------------------------------------
// ��������
// -------------------------------------------------------------
float4x4 mWorld;
float4x4 mVP;		// ���ÿ��� �������������� ��ǥ��ȯ
float4x4 mWIT;
float4x4 mWVPT;
float3 vLightDir = {0, -1, 0};
float3 vEyePos;
float4 vFog;
float4 fogColor = {0.58823f, 0.58823f, 0.58823f, 1}; // RGB(150,150,150)

// ���� ���.
float4 I_a = {0.3f, 0.3f, 0.3f, 0.0f}; // ambient
float4 I_d = {1.f, 1.f, 1.f, 0.0f}; // diffuse
float4 I_s = {1.f, 1.f, 1.f, 0.0f}; // specular

// �ݻ���
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


// ------------------------------------------------------------
// �ؽ�ó
// ------------------------------------------------------------
texture Tex;
sampler Samp = sampler_state
{
    Texture = <Tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};


// ------------------------------------------------------------
// �׸��ڸ�
// ------------------------------------------------------------
texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

	AddressU = Clamp;
	AddressV = Clamp;
};



// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float3 Eye : TEXCOORD1;
	float3 N : TEXCOORD2;
};


// -------------------------------------------------------------
// 1�н�:�������̴�
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// ���� ���� ���.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // ���� ��ǥ�迡���� ����.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 1�н�:�ȼ����̴�
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	Out = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	Out = Out * tex2D(Samp, In.Tex);
	//Out = tex2D(Samp, In.Tex);
    return Out;
}


// -------------------------------------------------------------
// 1�н�:�������̴�, ���� ���
// -------------------------------------------------------------
VS_OUTPUT VS_pass1(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// ���� ���� ���.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // ���� ��ǥ�迡���� ����.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
    
    return Out;
}

// -------------------------------------------------------------
// 1�н�:�ȼ����̴�, ���� ���.
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	Out = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	Out = Out * tex2D(Samp, In.Tex);

	float distance = length(In.Eye);
	float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	Out = lerp(Out, fogColor, l);

    return Out;
}



// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// �� + �׸���.
// -------------------------------------------------------------
struct VS_OUTPUT_SHADOW
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float4 TexShadow : TEXCOORD1;
	float3 Eye : TEXCOORD2;
	float3 N : TEXCOORD3;
};


// -------------------------------------------------------------
// 2�н�:�������̴�, ���� ���, �׸��� ���.
// -------------------------------------------------------------
VS_OUTPUT_SHADOW VS_pass2(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT_SHADOW Out = (VS_OUTPUT_SHADOW)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// ���� ���� ���.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // ���� ��ǥ�迡���� ����.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
	Out.TexShadow = mul( Pos, mWVPT );
    
    return Out;
}

// -------------------------------------------------------------
// 2�н�:�ȼ����̴�, ���� ���.
// -------------------------------------------------------------
float4 PS_pass2(VS_OUTPUT_SHADOW In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	float4 Color = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	float4 decale = tex2D(Samp, In.Tex);
	Out = Color * decale;

	float4 shadow = tex2Dproj( ShadowMapSamp, In.TexShadow );
	Out = Out * saturate(Color - (0.8f*shadow));

	float distance = length(In.Eye);
	float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	Out = lerp(Out, fogColor, l);

    return Out;
}





// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// �׸��� �� ����.
// -------------------------------------------------------------
struct VS_SHADOW_OUTPUT
{
	float4 Pos : POSITION;
	float4 Diffuse : COLOR0;
};


// -------------------------------------------------------------
// 4�н�:�������̴�, �׸��� �� ����.
// -------------------------------------------------------------
VS_SHADOW_OUTPUT VS_pass3(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_SHADOW_OUTPUT Out = (VS_SHADOW_OUTPUT)0;  // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );
	Out.Diffuse = float4(1,1,1,1);

    return Out;
}


	
// -------------------------------------------------------------
// ��ũ��
// -------------------------------------------------------------
technique TShader
{

	// �� ���̵�
    pass P0
    {
        VertexShader = compile vs_3_0 VS_pass0();
		PixelShader  = compile ps_3_0 PS_pass0();
    }


	// ���� ���̵�.
    pass P1
    {
        VertexShader = compile vs_3_0 VS_pass1();
		PixelShader  = compile ps_3_0 PS_pass1();
    }


    // �� + �׸��� ��� ���̴�
    pass P2
    {
        VertexShader = compile vs_3_0 VS_pass2();
		PixelShader  = compile ps_3_0 PS_pass2();
    }


	// �׸��� �� ��� ���̴�
	Pass P3
	{
		VertexShader = compile vs_3_0 VS_pass3();
	}

}
