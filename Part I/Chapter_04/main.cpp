#include "d3dUtility.h"

//
// Globals
//
IDirect3DDevice9* Device = 0;

const int Width  = 640;
const int Height = 480;

D3DXMATRIX WorldMatrix;
IDirect3DVertexBuffer9* Triangle_VB = 0;

// Define the ColorVertex
struct ColorVertex
{
	ColorVertex() {};
	ColorVertex(float _x, float _y, float _z, D3DCOLOR _color)
	{
		x = _x; y = _y; z = _z; color = _color;
	}
	float x, y, z;
	D3DCOLOR color;
	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

bool Setup()
{
	Device->CreateVertexBuffer(3 * sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &Triangle_VB, 0);

	ColorVertex* Triangle_Vertices;
	Triangle_VB->Lock(0, 0, (void**)&Triangle_Vertices, 0);
	Triangle_Vertices[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	Triangle_Vertices[1] = ColorVertex( 0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	Triangle_Vertices[2] = ColorVertex( 1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));
	Triangle_VB->Unlock();

	// set perspective projection matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.5, (float)Width / (float)Height, 1.0f, 1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	// turn off lighting
	Device->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle_VB);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff007733, 1.0f, 0);
		Device->BeginScene();

		Device->SetFVF(ColorVertex::FVF);
		Device->SetStreamSource(0, Triangle_VB, 0, sizeof(ColorVertex));

		// draw triangle to the left with flat shading
		D3DXMatrixTranslation(&WorldMatrix, -1.25f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		// set flat shading
		Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		// draw triangle to the left with gouraud shading
		D3DXMatrixTranslation(&WorldMatrix, 1.25f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		// set Gouraud shading
		Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}