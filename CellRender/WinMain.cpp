#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <d3dx9.h>

LPDIRECT3D9					g_pD3D			= NULL;
LPDIRECT3DDEVICE9			g_pd3dDevice	= NULL;

LPDIRECT3DTEXTURE9			g_pTexMark		= NULL;	//掩码图
LPDIRECT3DSURFACE9			g_pSurMark		= NULL;	//掩码图的表面

LPDIRECT3DTEXTURE9			g_pTexTeapot	= NULL;	//用来渲染茶壶的纹理
LPDIRECT3DSURFACE9			g_pSurTeapot	= NULL; //g_pTexTeapot(茶壶纹理)的表面

LPDIRECT3DSURFACE9			g_pbackbuffer	= NULL;
LPD3DXMESH					g_pmesh			= NULL;

LPD3DXEFFECT				g_peffect		= NULL;
HWND						g_hwnd;

float g_fwidth;
float g_fheight;


struct VERTEX_SCREEN
{
	float x, y, z;
	float u, v;
};
#define D3D_FVF_SCREEN	(D3DFVF_XYZ | D3DFVF_TEX1)



VERTEX_SCREEN g_v_screen[] = {{-1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
								{1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
								{-1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
								{1.0f, -1.0f, 0.0f, 1.0f, 1.0f}};


HRESULT InitData()
{
	RECT rect;
	GetClientRect(g_hwnd, &rect);
	g_fwidth = 1.0f/(rect.right-1);
	g_fheight = 1.0f/(rect.bottom-1);
	if(D3D_OK != D3DXCreateTexture(g_pd3dDevice, rect.right, rect.bottom, 1, D3DUSAGE_RENDERTARGET, 
						D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pTexMark))
	{
		MessageBox(NULL, "11111","错误", MB_OK);
	}

	if(D3D_OK != D3DXCreateTexture(g_pd3dDevice, rect.right, rect.bottom, 1, D3DUSAGE_RENDERTARGET, 
		D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &g_pTexTeapot))
	{
		MessageBox(NULL, "2222","错误", MB_OK);
	}

	g_pTexMark->GetSurfaceLevel(0, &g_pSurMark);
	g_pTexTeapot->GetSurfaceLevel(0, &g_pSurTeapot);

	D3DXCreateTeapot(g_pd3dDevice, &g_pmesh, NULL);
	LPD3DXBUFFER	pbuff = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(g_pd3dDevice, "my_fx.fx", NULL, NULL, 
							D3DXSHADER_DEBUG, NULL, &g_peffect, &pbuff);


	if(pbuff)
	{
		MessageBox(0, (char*)(pbuff->GetBufferPointer()), 0, MB_OK);
		pbuff->Release();
		return E_FAIL;
	}
	if(FAILED(hr))
	{
		return E_FAIL;
	}

	g_peffect->SetFloat("u", g_fwidth);
	g_peffect->SetFloat("v", g_fheight);
	return S_OK;
}

HRESULT InitD3D()
{
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	
	if(E_FAIL == InitData())
	{
		return E_FAIL;
	}

	g_pd3dDevice->GetRenderTarget(0, &g_pbackbuffer);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);

	return S_OK;
}


VOID Logic()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-10.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );

	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.a = 1.0f;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Direction = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.r = 0.75f;
	mtrl.Diffuse.g = 0.0f;
	mtrl.Diffuse.b = 0.0f;
	mtrl.Diffuse.a = 0.0f;
	g_pd3dDevice->SetMaterial(&mtrl);



}
VOID Render()
{
	if( NULL == g_pd3dDevice )
		return;


	UINT npass;
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
//用PS渲染掩码图
		g_pd3dDevice->SetRenderTarget(0, g_pSurMark);	//设置渲染对象为掩码图表面
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x0, 1.0f, 0 );

		g_peffect->SetTechnique( "PS_MARK" );
		g_pd3dDevice->SetTexture(0, 0);
		g_peffect->Begin(&npass, 0);
		for(int i=0; i<npass; ++i)
		{
			g_peffect->BeginPass(i);
			g_pmesh->DrawSubset(0);		//渲染茶壶到掩码图里面
			g_peffect->EndPass();
		}
		g_peffect->End();

//用固定管线渲染茶壶到第2张缓冲中
		g_pd3dDevice->SetRenderTarget(0, g_pSurTeapot);		//设置渲染对象为茶壶纹理表面
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0 );

		g_peffect->SetTechnique( "PS_CARTOON" );

		g_peffect->Begin(&npass, 0);
		for(int i=0; i<npass; ++i)
		{
			g_peffect->BeginPass(i);
			g_pmesh->DrawSubset(0);		//渲染茶壶到卡通纹理中
			g_peffect->EndPass();
		}
		g_peffect->End();


//用PS对掩码图做卷积运算，然后将2张缓冲纹理的颜色做乘法运算。
		g_pd3dDevice->SetRenderTarget(0, g_pbackbuffer);
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xff0000ff, 1.0f, 0 );

		g_peffect->SetTexture("g_txTeapot", g_pTexTeapot);
		g_peffect->SetTexture("g_txMark", g_pTexMark);
		g_peffect->SetTechnique( "my_tech" );
		g_pd3dDevice->SetFVF(D3D_FVF_SCREEN);

		g_peffect->Begin(&npass, 0);
		for(int i=0; i<npass; ++i)
		{
			g_peffect->BeginPass(i);
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_v_screen, sizeof(VERTEX_SCREEN));
			g_peffect->EndPass();
		}
		g_peffect->End();
		
		g_pd3dDevice->EndScene();
	}
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

VOID Cleanup()
{
	if( g_pSurTeapot != NULL)
		g_pSurTeapot->Release();

	if( g_pTexTeapot != NULL)
		g_pTexTeapot->Release();

	if( g_pmesh != NULL)
		g_pmesh->Release();

	if (g_peffect != NULL)
		g_peffect->Release();

	if( g_pbackbuffer != NULL)
		g_pbackbuffer->Release();

	if( g_pSurMark != NULL)
		g_pSurMark->Release();

	if( g_pTexMark != NULL)
		g_pTexMark->Release();

	if( g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if( g_pD3D != NULL)
		g_pD3D->Release();
}


LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}





INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Tutorial", NULL };
	RegisterClassEx( &wc );

	g_hwnd = CreateWindow( "D3D Tutorial", "D3D Tutorial 01: CreateDevice", 
		WS_OVERLAPPEDWINDOW, 100, 100, 500, 500,
		NULL, NULL, wc.hInstance, NULL );


	ShowWindow( g_hwnd, SW_SHOWDEFAULT );
	UpdateWindow( g_hwnd );

	if(SUCCEEDED(InitD3D()))
	{
		MSG msg;
		while(true)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					break;
				DispatchMessage(&msg);
			}
			else
			{
				Logic();
				Render();
				Sleep(1);
			}

		}
	}
	//D3DXSaveSurfaceToFile("111.tga", D3DXIFF_TGA, g_pSurMark, 0, 0);
	//D3DXSaveSurfaceToFile("222.tga", D3DXIFF_TGA, g_pSurTeapot, 0, 0);
	Cleanup();
	UnregisterClass( "D3D Tutorial", wc.hInstance );
	return 0;
}