#include "d3dUtility.h"

IDirect3DDevice9* Device;

const int Width = 640;
const int Height = 480;

IDirect3DVertexShader9* DiffuseShader = 0;
ID3DXConstantTable* DiffuseConstTable = 0;

ID3DXMesh* Teapot = 0;

D3DXHANDLE ViewMatrixHandle = 0;
D3DXHANDLE ViewProjMatrixHandle = 0;
D3DXHANDLE AmbientMtrlHandle = 0;
D3DXHANDLE DiffuseMtrlHandle = 0;
D3DXHANDLE LightDirHandle = 0;

D3DXMATRIX Proj;

bool Setup()
{
	HRESULT hr = 0;

	D3DXCreateTeapot(Device, &Teapot, 0);

	//
	// Compile shader
	//
	ID3DXBuffer* shader = 0;
	ID3DXBuffer* errorBuffer = 0;

	hr = D3DXCompileShaderFromFile("diffuse.hlsl", 0, 0, "Main", "vs_1_1", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shader, &errorBuffer, &DiffuseConstTable);;

	if (errorBuffer)
	{
		::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		d3d::Release<ID3DXBuffer*>(errorBuffer);
	}

	if (FAILED(hr))
	{
		::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return false;
	}

	//
	// Create shader
	//
	hr = Device->CreateVertexShader((DWORD*)shader->GetBufferPointer(), &DiffuseShader);

	if (FAILED(hr))
	{
		::MessageBox(0, "CreateVertexShader - FAILED", 0, 0);
		return false;
	}

	//
	// Get Handles
	//
	d3d::Release<ID3DXBuffer*>(shader);

	ViewMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewMatrix");
	ViewProjMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewProjMatrix");
	AmbientMtrlHandle = DiffuseConstTable->GetConstantByName(0, "AmbientMtrl");
	DiffuseMtrlHandle = DiffuseConstTable->GetConstantByName(0, "DiffuseMtrl");
	LightDirHandle = DiffuseConstTable->GetConstantByName(0, "LightDirection");
	
	//
	// Set shader constants
	//
	D3DXVECTOR4 directionToLight(-0.57f, 0.57f, 0.57f, 0.0f);
	DiffuseConstTable->SetVector(Device, LightDirHandle, &directionToLight);

	D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	DiffuseConstTable->SetVector(Device, AmbientMtrlHandle, &ambientMtrl);
	DiffuseConstTable->SetVector(Device, DiffuseMtrlHandle, &diffuseMtrl);
	DiffuseConstTable->SetDefaults(Device);

	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI*0.25f, (float)Width / (float)Height, 1.0f, 1000.0f);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexShader9*>(DiffuseShader);
	d3d::Release<ID3DXConstantTable*>(DiffuseConstTable);
	d3d::Release<ID3DXMesh*>(Teapot);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		static float angle = (3.0f*D3DX_PI) / 2.0f;
		static float height = 3.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += timeDelta;
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= timeDelta;

		D3DXVECTOR3 position(cosf(angle)*7.0f, height, sinf(angle)*7.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		DiffuseConstTable->SetMatrix(Device, ViewMatrixHandle, &V);

		D3DXMATRIX ViewProj = V * Proj;
		DiffuseConstTable->SetMatrix(Device, ViewProjMatrixHandle, &ViewProj);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		Device->SetVertexShader(DiffuseShader);

		Teapot->DrawSubset(0);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

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