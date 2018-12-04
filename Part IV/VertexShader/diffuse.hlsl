matrix ViewMatrix;
matrix ViewProjMatrix;

vector AmbientMtrl;
vector DiffuseMtrl;

static vector LightDirection;

vector DiffuseLightIntensity = { 0.0f, 0.0f, 1.0f, 1.0f };
vector AmbientLightIntensity = { 0.0f, 0.0f, 0.2f, 1.0f };

struct VS_INPUT
{
    vector position : POSITION;	//vertex position
    vector normal : NORMAL;		//vertex normal
};

struct VS_OUTPUT
{
    vector position : POSITION;	//vertex position
    vector diffuse  : COLOR;	//vertex color
};

VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = mul(input.position, ViewProjMatrix);

    LightDirection.w = 0.0f;
    input.normal.w = 0.0f;
    LightDirection = mul(LightDirection, ViewMatrix);
    input.normal = mul(input.normal, ViewMatrix);

    float s = dot(LightDirection, input.normal);
    if (s<0.0f)
        s = 0.0f;

    output.diffuse = (AmbientMtrl * AmbientLightIntensity) + (s * (DiffuseLightIntensity * DiffuseMtrl));

    return output;
}