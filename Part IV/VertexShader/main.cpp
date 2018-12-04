#include "d3dUtility.h"

IDirect3DDevice9* Device;

void main()
{
	
}

bool CompileShader()
{
	ID3DXConstantTable* TransformConstantTable = 0;
	ID3DXBuffer* shader = 0;
	ID3DXBuffer* errorBuffer = 0;

	HRESULT hr = D3DXCompileShaderFromFile("Transform.hlsl", 0, 0, "Main", "vs_2_0", D3DXSHADER_DEBUG, &shader, &errorBuffer, &TransformConstantTable);

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
}