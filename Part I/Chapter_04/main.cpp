#include "d3dUtility.h"

//
// Globals
//
IDirect3DDevice9* Device;

const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9* Triangle_VB = 0;

// Define the ColorVertex
struct ColorVertex
{
	float x, y, z;
	D3DCOLOR color;
	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

bool Setup()
{
	ColorVertex T[3];
	T[]
}