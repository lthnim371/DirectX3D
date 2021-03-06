
// -------------------------------------------------------------
// ��������
// -------------------------------------------------------------
float4x4 mWVP;		// ���ÿ��� �������������� ��ǥ��ȯ
float4x4 mWIT;
float3 vLightDir;
float3 vEyePos;

// ���� ���.
float4 I_a = {0.3f, 0.3f, 0.3f, 0.0f}; // ambient
float4 I_d = {0.7f, 0.7f, 0.7f, 0.0f}; // diffuse
float4 I_s = {1.f, 1.f, 1.f, 0.0f}; // diffuse

// �ݻ���
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
};


// -------------------------------------------------------------
// 1�н�:�������̴�
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL		// ��������
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    // ��ǥ��ȯ
	Out.Pos = mul( Pos, mWVP );

	// ���� ��
	float3 L = -vLightDir;
	float3 N = normalize(mul(Normal, (float3x3)mWIT)); // ���� ��ǥ�迡���� ����.
	
	float3 eye = normalize(vEyePos - Pos.xyz);  //������ ���ϱ�
	float3 H = normalize(L+eye);  //������ + ������

	Out.Diffuse = I_a * K_a
						 + I_d * K_d * max(0, dot(N,L))
						 + I_s * pow( max(0, dot(N,H)), 32);

    return Out;
}


// -------------------------------------------------------------
// 1�н�:�ȼ����̴�
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
    float4 Out;

	Out = In.Diffuse;

    return Out;
}


// -------------------------------------------------------------
// ��ũ��
// -------------------------------------------------------------
technique TShader
{
    pass P0
    {
        // ���̴�
        VertexShader = compile vs_3_0 VS_pass0();
        PixelShader  = compile ps_3_0 PS_pass0();
    }
}
