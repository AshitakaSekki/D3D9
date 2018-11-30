matrix ViewProjMatrix;

vector Blue = { 0.0f, 0.0f, 1.0f, 1.0f };

struct VS_INPUT
{
    vector position : POSITION;
};

struct VS_OUTPUT
{
    vector position : POSITION;
    vector diffuse  : COLOR;
};