#include "d3dUtility.h"

//
// Globals
//

IDirect3DDevice9* Device = 0; 

const int Width  = 640;
const int Height = 480;

IDirect3DVertexBuffer9* Triangle_VB = 0; // vertex buffer to store
										 // our triangle data.

IDirect3DVertexBuffer9* Cube_VB = 0;	 // Cube vertex buffer.
IDirect3DIndexBuffer9* Cube_IB = 0;		 // Cube Index buffer.

//
// Classes and Structures
//

struct Vertex
{
	Vertex(){}

	Vertex(float x, float y, float z)
	{
		_x = x;	 _y = y;  _z = z;
	}

	float _x, _y, _z;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;

//
// Framework Functions
//
bool Setup()
{
	//
	// Create the Triangle vertex buffer.
	//

	Device->CreateVertexBuffer(
		3 * sizeof(Vertex), // size in bytes
		D3DUSAGE_WRITEONLY, // flags
		Vertex::FVF,        // vertex format
		D3DPOOL_MANAGED,    // managed memory pool
		&Triangle_VB,          // return create vertex buffer
		0);                 // not used - set to 0

	//
	// Create the Cube vertex and index buffer.
	//

	Device->CreateVertexBuffer(
		8 * sizeof(Vertex), // size in bytes
		D3DUSAGE_WRITEONLY, // flags
		Vertex::FVF,        // vertex format
		D3DPOOL_MANAGED,    // managed memory pool
		&Cube_VB,          // return create vertex buffer
		0);                 // not used - set to 0

	Device->CreateIndexBuffer(
		6 * 6 * sizeof(WORD), //6 vertices in each face
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&Cube_IB,
		0);

	//
	// Fill the buffers with vertex data.
	//

	Vertex* Triangle;
	Triangle_VB->Lock(0, 0, (void**)&Triangle, 0);

	Triangle[0] = Vertex(-1.0f, 0.0f, 1.0f);
	Triangle[1] = Vertex(0.0f, 1.0f, 1.0f);
	Triangle[2] = Vertex(1.0f, 0.0f, 1.0f);

	Triangle_VB->Unlock();

	Vertex* Cube;
	Cube_VB->Lock(0, 0, (void**)&Cube, 0);

	Cube[0] = Vertex(-1.0f, -1.0f, -1.0f);
	Cube[1] = Vertex(-1.0f,  1.0f, -1.0f);
	Cube[2] = Vertex( 1.0f,  1.0f, -1.0f);
	Cube[3] = Vertex( 1.0f, -1.0f, -1.0f);
	Cube[4] = Vertex(-1.0f, -1.0f,  1.0f);
	Cube[5] = Vertex(-1.0f,  1.0f,  1.0f);
	Cube[6] = Vertex( 1.0f,  1.0f,  1.0f);
	Cube[7] = Vertex( 1.0f, -1.0f,  1.0f);

	Cube_VB->Unlock();

	WORD* Cube_Indices = 0;
	Cube_IB->Lock(0, 0, (void**)&Cube_Indices, 0);

	// front side
	Cube_Indices[0] = 0; Cube_Indices[1] = 1; Cube_Indices[2] = 2;
	Cube_Indices[3] = 0; Cube_Indices[4] = 2; Cube_Indices[5] = 3;

	// back side
	Cube_Indices[6] = 4; Cube_Indices[7]  = 6; Cube_Indices[8]  = 5;
	Cube_Indices[9] = 4; Cube_Indices[10] = 7; Cube_Indices[11] = 6;

	// left side
	Cube_Indices[12] = 4; Cube_Indices[13] = 5; Cube_Indices[14] = 1;
	Cube_Indices[15] = 4; Cube_Indices[16] = 1; Cube_Indices[17] = 0;

	// right side
	Cube_Indices[18] = 3; Cube_Indices[19] = 2; Cube_Indices[20] = 6;
	Cube_Indices[21] = 3; Cube_Indices[22] = 6; Cube_Indices[23] = 7;

	// top side
	Cube_Indices[24] = 1; Cube_Indices[25] = 5; Cube_Indices[26] = 6;
	Cube_Indices[27] = 1; Cube_Indices[28] = 6; Cube_Indices[29] = 2;

	// bottom side
	Cube_Indices[30] = 4; Cube_Indices[31] = 0; Cube_Indices[32] = 3;
	Cube_Indices[33] = 4; Cube_Indices[34] = 3; Cube_Indices[35] = 7;

	Cube_IB->Unlock();

	//
	// Position and aim the camera
	//
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX View;

	D3DXMatrixLookAtLH(&View, &position, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &View);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,                        // result
			D3DX_PI * 0.5f,               // 90 - degrees
			(float)Width / (float)Height, // aspect ratio
			1.0f,                         // near plane
			1000.0f);                     // far plane
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// Set wireframe mode render state.
	//

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle_VB);
	d3d::Release<IDirect3DVertexBuffer9*>(Cube_VB);
	d3d::Release<IDirect3DIndexBuffer9*>(Cube_IB);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		// Spin the cube
		D3DXMATRIX Rx, Ry;

		// rotate 45 degrees on x-axis
		D3DXMatrixRotationX(&Rx, D3DX_PI / 4.0f);

		// increment y-rotation angle each frame
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y += timeDelta;

		// reset angle to zero when angle reaches 2*PI
		if (y >= 2 * D3DX_PI)
		{
			y = 0.0f;
		}

		// combine x-axis and y-axis rotation transformations
		D3DXMATRIX p = Rx * Ry;
		Device->SetTransform(D3DTS_WORLD, &p);

		// draw the scene
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		// draw triangle
		Device->SetStreamSource(0, Triangle_VB, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		// draw cube
		Device->SetStreamSource(0, Cube_VB, 0, sizeof(Vertex));
		Device->SetIndices(Cube_IB);
		Device->SetFVF(Vertex::FVF);

		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

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
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
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
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}