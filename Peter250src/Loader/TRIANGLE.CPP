//-----------------------------------------------------------------------------
// File: Triangle.cpp
//
// Desc: Simple tutorial code to show the rendering of one triangle.
//
// Copyright (c) 1998 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#define D3D_OVERLOADS
#include <math.h>
#include "dxsupport.h"


//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
CMaterial*	lpcObjectMtrl = NULL;
D3DVERTEX	g_pvTriangleVertices[6];

extern CDirect3D*	lpcD3D;


//-----------------------------------------------------------------------------
// Name: App_InitDeviceObjects()
// Desc: Initialize scene objects. This function is called after all the
//       DirectDraw and Direct3D objects have been initialized. It makes sense
//       to structure code this way, separating the DDraw/D3D initialization
//       code from the app-specific intialization code.
//-----------------------------------------------------------------------------
HRESULT App_InitDeviceObjects( CDirect3DDevice* lpcD3DDevice,
                               CDirect3DViewport* lpcViewport )
{
	// Data for the geometry of the triangle. Note that this tutorial only
	// uses ambient lighting, so the vertices' normals are not actually used.
	D3DVECTOR p1( 0.0f, 3.0f, 0.0f );
	D3DVECTOR p2( 3.0f,-3.0f, 0.0f );
	D3DVECTOR p3(-3.0f,-3.0f, 0.0f );
	D3DVECTOR vNormal( 0.0f, 0.0f, 1.0f );
	
	// Initialize the 3 vertices for the front of the triangle
	g_pvTriangleVertices[0] = D3DVERTEX( p1, vNormal, 0.0f, 0.0f );
	g_pvTriangleVertices[1] = D3DVERTEX( p2, vNormal, 0.0f, 0.0f );
	g_pvTriangleVertices[2] = D3DVERTEX( p3, vNormal, 0.0f, 0.0f );
    
	// Initialize the 3 vertices for the back of the triangle
	g_pvTriangleVertices[3] = D3DVERTEX( p1, -vNormal, 0.0f, 0.0f );
	g_pvTriangleVertices[4] = D3DVERTEX( p3, -vNormal, 0.0f, 0.0f );
	g_pvTriangleVertices[5] = D3DVERTEX( p2, -vNormal, 0.0f, 0.0f );
    
    // Create the object material. This material will be used to draw the
	// triangle. Note: that when we use textures, the object material is
	// usually omitted or left as white.
	if ((lpcObjectMtrl = new CMaterial(lpcD3D, lpcD3DDevice)) == NULL)
		return DDERR_GENERIC;

	// Set the object material as yellow. We're setting the ambient color here
	// since this tutorial only uses ambient lighting. For apps that use real
	// lights, the diffuse and specular values should be set. (In addition, the
	// polygons' vertices need normals for true lighting.)
    D3DMATERIAL       mtrl;
    D3DMATERIALHANDLE hmtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL) );
    mtrl.dwSize       = sizeof(D3DMATERIAL);
    mtrl.dcvAmbient.r = 1.0f;
    mtrl.dcvAmbient.g = 1.0f;
    mtrl.dcvAmbient.b = 0.0f;
    lpcObjectMtrl->SetMaterial( &mtrl );

	// Put the object material into effect. Direct3D is a state machine, and
	// calls like this set the current state. After this call, any polygons
	// rendered will be drawn using this material.
    lpcObjectMtrl->GetHandle(hmtrl);
    lpcD3DDevice->SetLightState(  D3DLIGHTSTATE_MATERIAL, hmtrl );

	// The ambient lighting value is another state to set. Here, we are turning
	// ambient lighting on to full white.
    lpcD3DDevice->SetLightState(  D3DLIGHTSTATE_AMBIENT,  0xffffffff );

    // Set the transform matrices. Direct3D uses three independant matrices:
	// the world matrix, the view matrix, and the projection matrix. For
	// convienence, we are first setting up an identity matrix.
    D3DMATRIX mat;
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = mat._41 = 0.0f;
	mat._21 = mat._23 = mat._24 = mat._42 = 0.0f;
	mat._31 = mat._32 = mat._34 = mat._43 = 0.0f;
	
	// The world matrix controls the position and orientation of the polygons
	// in world space. We'll use it later to spin the triangle.
	D3DMATRIX matWorld = mat;
    lpcD3DDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	// The view matrix defines the position and orientation of the camera.
	// Here, we are just moving it back along the z-axis by 10 units.
	D3DMATRIX matView = mat;
	matView._43 = 10.0f;
    lpcD3DDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, &matView );

	// The projection matrix defines how the 3D scene is "projected" onto the
	// 2D render target (the backbuffer surface). Refer to the docs for more
	// info about projection matrices.
	D3DMATRIX matProj = mat;
	matProj._11 =  2.0f;
	matProj._22 =  2.0f;
	matProj._34 =  1.0f;
	matProj._43 = -1.0f;
	matProj._44 =  0.0f;
    lpcD3DDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: App_FrameMove()
// Desc: Called once per frame, the call is used for animating the scene. The
//       device is used for changing various render states, and the timekey is
//       used for timing of the dynamics of the scene.
//-----------------------------------------------------------------------------
HRESULT App_FrameMove( CDirect3DDevice* lpcD3DDevice, FLOAT fTimeKey )
{
    // For this simple tutorial, we are rotating the triangle about the y-axis.
	// To do this, just set up a 4x4 matrix defining the rotation, and set it
	// as the new world transform.
    D3DMATRIX matSpin;
	matSpin._11 = matSpin._22 = matSpin._33 = matSpin._44 = 1.0f;
	matSpin._12 = matSpin._13 = matSpin._14 = matSpin._41 = 0.0f;
	matSpin._21 = matSpin._23 = matSpin._24 = matSpin._42 = 0.0f;
	matSpin._32 = matSpin._32 = matSpin._34 = matSpin._43 = 0.0f;
	
	matSpin._11 = (FLOAT)cos( fTimeKey );
	matSpin._33 = (FLOAT)cos( fTimeKey );
	matSpin._13 = (FLOAT)sin( fTimeKey );
	matSpin._31 = (FLOAT)sin( fTimeKey );

    lpcD3DDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matSpin );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: App_Render()
// Desc: Called once per frame, the function is called to render the scene.
//       Rendering consists of the following:
//       (1) Clear the viewport
//       (2) Call BeginScene()
//       (3) Draw the polygons (primitives) and change the state as needed
//       (4) Call EndScene()
//
//       After this call completes, the device's render target (the backbuffer
//       surface) will contain the rendered image of the scene. All that is
//       left is to copy the contents of the backbuffer to the primary.
//-----------------------------------------------------------------------------
HRESULT App_Render( CDirect3DDevice* lpcD3DDevice, 
                    CDirect3DViewport* lpcViewport,
                    D3DRECT* prcViewportRect )
{
    // Clear the viewport to a blue color (dwColor = 0x000000ff). For apps with
	// z-buffers, this call can also clear the z-buffer (if present) as well.
    lpcViewport->Clear2( 1UL, prcViewportRect, D3DCLEAR_TARGET, 0x000000ff,
		                0.0f, 0L );

    // Begin the scene
	if (!lpcD3DDevice->BeginScene())
		return DDERR_GENERIC;

	// Draw the triangle using a DrawPrimitive() call. Subsequent
	// tutorials will go into more detail on the various calls for
	// drawing polygons.
    lpcD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX,
                               g_pvTriangleVertices, 6, NULL );

    // End the scene.
    lpcD3DDevice->EndScene();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: App_DeleteDeviceObjects()
// Desc: Called when the device is being deleted, this function deletes any
//       device dependant objects.
//-----------------------------------------------------------------------------
VOID App_DeleteDeviceObjects( CDirect3DDevice*	lpcD3DDevice, 
                              CDirect3DViewport* lpcViewport )
{
	// Release the material that was created earlier.
    if( lpcObjectMtrl )
	{
		delete lpcObjectMtrl;
		lpcObjectMtrl = NULL;
	}
}




