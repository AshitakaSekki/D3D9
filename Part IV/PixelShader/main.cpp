#include "d3dUtility.h"

IDirect3DPixelShader9* MultiTexPS = 0;
ID3DXConstantTable* MultiTexCT = 0;

IDirect3DVertexBuffer9* QuadVB = 0;

IDirect3DTexture9* BaseTex = 0;
IDirect3DTexture9* SpotLightTex = 0;
IDirect3DTexture9* StringTex = 0;

D3DXHANDLE BaseTexHandle = 0;
D3DXHANDLE SpotLightTexHandle = 0;
D3DXHANDLE StingHandle = 0;

D3DXCONSTANT_DESC BaseTexDesc;
D3DXCONSTANT_DESC SpotLightTexDesc;
D3DXCONSTANT_DESC StringTexDesc;

struct MultiTexVertex 
{
	MultiTexVertex(
		float x, float y, float z,
		float u0, float v0,
		float u1, float v1,
		float u2, float v2)
	{
		_x = x; _y = y; _z = z;
		_u0 = u0; _v0 = v0;
		_u1 = u1; _v1 = v1;
		_u2 = u2; _v2 = v2;
	}

	float _x, _y, _z;
	float _u0, _v0;
	float _u1, _v1;
	float _u2, _v2;

	static const DWORD FVF;
};
const DWORD MultiTexVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX3;