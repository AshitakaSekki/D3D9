#pragma once

#include <d3d9caps.h>
#include "d3dUtility.h"

namespace test
{
	D3DCAPS9 caps;


	bool CheckHWTransAndLight()
	{
		IDirect3D9* d3d9 = 0;
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		if (!d3d9)
		{
			::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		}
		else
		{
			d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		}


		bool supportsHardwareVertexProcessing;

		if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
			supportsHardwareVertexProcessing = true;
		}
		else {
			supportsHardwareVertexProcessing = false;
		}

		return supportsHardwareVertexProcessing;
	}

}
