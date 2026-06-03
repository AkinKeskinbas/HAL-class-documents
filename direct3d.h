#ifndef DIRECT3D_H
#define	DIRECT3D_H
#include <Windows.h>

#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL; } 


bool Direct3D_Initialize(HWND window_handle);
void Direct3D_Begin();
void Direct3D_Present();

void Direct3D_Finalize();

#endif // DIRECT3D_H

