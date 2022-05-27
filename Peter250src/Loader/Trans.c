/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: trans.c
 *
 ***************************************************************************/

#include "rmdemo.h"

unsigned char check[] = {
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
    1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1,
    2, 3, 3, 2, 1, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
};

D3DRMPALETTEENTRY checkPal[] = {
    {0, 0, 0, D3DRMPALETTE_READONLY},
    {127, 255, 127, D3DRMPALETTE_READONLY},
    {0, 0, 255, D3DRMPALETTE_READONLY},
    {255, 255, 127, D3DRMPALETTE_READONLY},
    {255, 255, 255, D3DRMPALETTE_READONLY},
};

D3DRMIMAGE checkImage = {
    32, 32,
    1, 1,
    8, FALSE,
    32,
    check, NULL,
    0xff, 0xff, 0xff, 0xff,
    5, checkPal,
};


BOOL
BuildScene(LPDIRECT3DRMDEVICE dev, LPDIRECT3DRMVIEWPORT view,
	   LPDIRECT3DRMFRAME scene, LPDIRECT3DRMFRAME camera)
{
    LPDIRECT3DRMFRAME box1 = NULL;
    LPDIRECT3DRMFRAME box2 = NULL;
    LPDIRECT3DRMFRAME sphere = NULL;
    LPDIRECT3DRMFRAME decal = NULL;
    LPDIRECT3DRMFRAME light = NULL;
    LPDIRECT3DRMMESH obj = NULL;
    LPDIRECT3DRMLIGHT dlight = NULL;
    LPDIRECT3DRMLIGHT alight = NULL;
    LPDIRECT3DRMTEXTURE tex = NULL;
    LPDIRECT3DRMMESHBUILDER builder = NULL;
    LPDIRECT3DRMWRAP wrap = NULL;
    LPDIRECT3DRMTEXTURE sphereTex = NULL;
    LPDIRECT3DRMMATERIAL mat = NULL;
    D3DRMBOX box;
    D3DVALUE miny, maxy;
    D3DVALUE height;
    HRESULT rval;

    view = view;
    dev = dev;
    camera = camera;		/* unused */

    if (FAILED(scene->lpVtbl->SetSceneBackground(scene, D3DRGB(D3DVAL(0.2), D3DVAL(0.2),
                                      D3DVAL(0.2)))))
				      goto generic_error;

    if (FAILED(lpD3DRM->lpVtbl->CreateFrame(lpD3DRM, scene, &light)))
	goto generic_error;
    if (FAILED(light->lpVtbl->SetPosition(light, scene, D3DVAL(2.0), D3DVAL(3.0),
                               D3DVAL(8.0))))
			       goto generic_error;
    if (FAILED(light->lpVtbl->SetOrientation(light, scene, D3DVAL(-1.0), D3DVAL(-1.0),
                                  D3DVAL(1.0), D3DVAL(0.0), D3DVAL(1.0), D3DVAL(0.0))))
				  goto generic_error;
    if (FAILED(lpD3DRM->lpVtbl->CreateLightRGB(lpD3DRM, D3DRMLIGHT_DIRECTIONAL, D3DVAL(1),
                                  D3DVAL(1), D3DVAL(1), &dlight)))
				  goto generic_error;
    if (FAILED(light->lpVtbl->AddLight(light, dlight)))
	goto generic_error;
    RELEASE(dlight);
    RELEASE(light);

    if (FAILED(lpD3DRM->lpVtbl->CreateLightRGB(lpD3DRM, D3DRMLIGHT_AMBIENT, D3DVAL(0.1),
                                  D3DVAL(0.1), D3DVAL(0.1), &alight)))
				  goto generic_error;
    if (FAILED(scene->lpVtbl->AddLight(scene, alight)))
	goto generic_error;
    RELEASE(alight);




    if (FAILED(lpD3DRM->lpVtbl->CreateFrame(lpD3DRM, scene, &box2)))
	goto generic_error;
    if (FAILED(box2->lpVtbl->SetPosition(box2, scene, D3DVAL(0.0), D3DVAL(0.0), D3DVAL(6.0))))
	goto generic_error;
    if (FAILED(lpD3DRM->lpVtbl->CreateMeshBuilder(lpD3DRM, &builder)))
	goto generic_error;
    rval = builder->lpVtbl->Load(builder, "ncube.x", NULL,
    			D3DRMLOAD_FROMFILE, NULL, NULL);
    if (rval != D3DRM_OK) {
        Msg("Failed to load ncube.x.\n");
	goto ret_with_error;
    }
    rval = lpD3DRM->lpVtbl->LoadTexture(lpD3DRM, "checker.ppm", &tex);
    if (rval != D3DRM_OK) {
	Msg("Failed to load checker.ppm.\n");
	goto ret_with_error;
    }

    if (FAILED(tex->lpVtbl->SetDecalTransparency(tex, TRUE)))
	goto generic_error;

	tex->lpVtbl->SetDecalTransparentColor(tex, 0);

//	builder->lpVtbl->SetTexture(builder, tex);
	box2->lpVtbl->SetTexture(box2, tex);
	box2->lpVtbl->SetMaterialMode(box2, D3DRMMATERIAL_FROMFRAME);

    if (FAILED(box2->lpVtbl->AddVisual(box2, (LPDIRECT3DRMVISUAL) builder)))
	goto generic_error;
    RELEASE(builder);
    RELEASE(tex);
    RELEASE(box2);


    RELEASE(box1);
    RELEASE(decal);
    RELEASE(sphere);
    return TRUE;
generic_error:
    Msg("A failure occurred while building the scene.\n");
ret_with_error:
    RELEASE(box1);
    RELEASE(box2);
    RELEASE(sphere);
    RELEASE(decal);
    RELEASE(light);
    RELEASE(obj);
    RELEASE(dlight);
    RELEASE(alight);
    RELEASE(tex);
    RELEASE(builder);
    RELEASE(wrap);
    RELEASE(sphereTex);
    RELEASE(mat);
    return FALSE;
}

void
OverrideDefaults(Defaults* defaults)
{
    defaults->bConstRenderQuality = TRUE;
    lstrcpy(defaults->Name, "Transparency Direct3DRM Example");
}
