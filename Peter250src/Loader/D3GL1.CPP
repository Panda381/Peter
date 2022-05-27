
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#include "Main.h"

/***************************************************************************\
*																			*
*							3D rozhraní - OpenGL 1.1						*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// data

// funkce OpenGL 1.1
GLARRAYELEMENT		pglArrayElement		= NULL;
GLBINDTEXTURE		pglBindTexture		= NULL;
GLCOLORPOINTER		pglColorPointer		= NULL;
GLDELETETEXTURES	pglDeleteTextures	= NULL;
GLDISABLECLIENTSTATE pglDisableClientState = NULL;
GLDRAWARRAYS		pglDrawArrays		= NULL;
GLDRAWELEMENTS		pglDrawElements		= NULL;
GLENABLECLIENTSTATE pglEnableClientState = NULL;
GLGENTEXTURES		pglGenTextures		= NULL;
GLINDEXPOINTER		pglIndexPointer		= NULL;
GLNORMALPOINTER		pglNormalPointer	= NULL;
GLTEXCOORDPOINTER	pglTexCoordPointer	= NULL;
GLVERTEXPOINTER		pglVertexPointer	= NULL;

// OpenGL 1.1
bool				D3GL1D3Test			= false;	// probìhl test rozhraní OpenGL 1.1
bool				D3GL1OK				= false;	// rozhraní OpenGL 1.1 vytvoøeno OK

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní OpenGL 1.1 (vrací TRUE=rozhraní je vytvoøeno)

bool D3GL1InitD3()
{
// test, zda inicializace již probìhla
	if (!D3GL1D3Test)
	{
		D3GL1D3Test = true;

// inicializace rozhraní OpenGL 1.0
		if (D3GL0InitD3())
		{

// vyhledání funkcí OpenGL 1.1
			pglArrayElement = (GLARRAYELEMENT)::GetProcAddress(D3GL0Lib, "glArrayElement");
			pglBindTexture = (GLBINDTEXTURE)::GetProcAddress(D3GL0Lib, "glBindTexture");
			pglColorPointer = (GLCOLORPOINTER)::GetProcAddress(D3GL0Lib, "glColorPointer");
			pglDeleteTextures = (GLDELETETEXTURES)::GetProcAddress(D3GL0Lib, "glDeleteTextures");
			pglDisableClientState = (GLDISABLECLIENTSTATE)::GetProcAddress(D3GL0Lib, "glDisableClientState");
			pglDrawArrays = (GLDRAWARRAYS)::GetProcAddress(D3GL0Lib, "glDrawArrays");
			pglDrawElements = (GLDRAWELEMENTS)::GetProcAddress(D3GL0Lib, "glDrawElements");
			pglEnableClientState = (GLENABLECLIENTSTATE)::GetProcAddress(D3GL0Lib, "glEnableClientState");
			pglGenTextures = (GLGENTEXTURES)::GetProcAddress(D3GL0Lib, "glGenTextures");
			pglIndexPointer = (GLINDEXPOINTER)::GetProcAddress(D3GL0Lib, "glIndexPointer");
			pglNormalPointer = (GLNORMALPOINTER)::GetProcAddress(D3GL0Lib, "glNormalPointer");
			pglTexCoordPointer = (GLTEXCOORDPOINTER)::GetProcAddress(D3GL0Lib, "glTexCoordPointer");
			pglVertexPointer = (GLVERTEXPOINTER)::GetProcAddress(D3GL0Lib, "glVertexPointer");

// kontrola funkcí OpenGL 1.1
			if (
				(pglArrayElement != NULL) &&
				(pglBindTexture != NULL) &&
				(pglColorPointer != NULL) &&
				(pglDeleteTextures != NULL) &&
				(pglDisableClientState != NULL) &&
				(pglDrawArrays != NULL) &&
				(pglDrawElements != NULL) &&
				(pglEnableClientState != NULL) &&
				(pglGenTextures != NULL) &&
				(pglIndexPointer != NULL) &&
				(pglNormalPointer != NULL) &&
				(pglTexCoordPointer != NULL) &&
				(pglVertexPointer != NULL)
				)
			{
				D3GL1OK = true;
			}
		}
	}

// test, zda je rozhraní OpenGL 1.1 funkèní
	return D3GL1OK;
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní OpenGL 1.1

void D3GL1TermD3()
{
// ukonèení renderování
	D3GL0Stop();

// zrušení funkcí
/*
	pglArrayElement = NULL;
	pglBindTexture = NULL;
	pglColorPointer = NULL;
	pglDeleteTextures = NULL;
	pglDisableClientState = NULL;
	pglDrawArrays = NULL;
	pglDrawElements = NULL;
	pglEnableClientState = NULL;
	pglGenTextures = NULL;
	pglIndexPointer = NULL;
	pglNormalPointer = NULL;
	pglTexCoordPointer = NULL;
	pglVertexPointer = NULL;
*/
// zrušení pøíznaku, že byla inicializace
	D3GL1D3Test = false;
	D3GL1OK = false;
}

/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3GL1Start(int type)
{
	if (!D3GL1InitD3()) return false;

	if (!D3GL0Start(type)) return false;

// opakovaná inicializace, pokud byla uvolnìna knihovna OpenGL
	D3GL1InitD3();

// inicializace adres funkcí zaøízení
	pD3Render = D3GL1Render;		// vyrenderování jednoho objektu
	pD3TextureRes = D3GL1TextureRes;
	pD3TextureSet = D3GL1TextureSet;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderování jednoho objektu

void D3GL1Render(D3DFITEM* item)
{
	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);

// nastavení matice
	pglMatrixMode(GL_MODELVIEW);

	if (isPic)
	{
		pglLoadIdentity();
		pglMatrixMode(GL_PROJECTION);
		pglPushMatrix();
		pglLoadIdentity();
	}
	else
	{
		pglLoadMatrixf((float*)&D3DViewMatrix);
		pglMultMatrixf((float*)&item->MatrixWorld);
	}

// pøíprava pole normál
	if (isPic)
	{
		pglDisableClientState(GL_NORMAL_ARRAY);
	}
	else
	{
		pglEnableClientState(GL_NORMAL_ARRAY);
		pglNormalPointer(GL_FLOAT, sizeof(D3DVECTOR), item->Normal);
	}

// pøíprava pole souøadnic textury
	pglEnableClientState(GL_TEXTURE_COORD_ARRAY);
	pglTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), item->TextUV[0]);

// pøíprava bufferu vrcholù
	pglEnableClientState(GL_VERTEX_ARRAY);

	D3DVECTOR* buf = NULL;

	if (isPic)
	{
		int n = item->VertNum;
		buf = (D3DVECTOR*)MemGet(n * sizeof(D3DVECTOR));

		double sinz1 = item->SinZ*D3DHeight/D3DWidth;
		double sinz2 = item->SinZ*D3DWidth/D3DHeight;

		double sx = item->TransX;
		double sy = item->TransY;

		if (isFlare)
		{
			D3DVECTOR vv;
			vv.x = item->MatrixWorld._41;
			vv.y = item->MatrixWorld._42;
			vv.z = item->MatrixWorld._43;

			VecXMat(&vv, &vv, &(D3DViewMatrix));

			if (vv.z > 0.001)
			{
				sx = vv.x/vv.z * item->ScaleZ / D3DWidthHeight2;
				sy = vv.y/vv.z * item->ScaleZ * D3DWidthHeight2;
			}
			else
			{
				sx = -1;
				sy = -1;
			}
		}

		for (int i = 0; i < n; i++)
		{
			double x = item->Vertex[i].x*item->ScaleX;
			double y = item->Vertex[i].y*item->ScaleY;
			buf[i].x = (float)(2*(x*item->CosZ - y*sinz1 + sx) - 0.25/D3DWidth);
			buf[i].y = (float)(2*(x*sinz2 + y*item->CosZ + sy) - 0.25/D3DHeight);
			buf[i].z = 0;
		}

		pglVertexPointer(3, GL_FLOAT, sizeof(D3DVECTOR), buf);
	}
	else
	{
		pglVertexPointer(3, GL_FLOAT, sizeof(D3DVECTOR), item->Vertex);
	}

// pøíprava pole barev
	pglEnable(GL_COLOR_MATERIAL);
//	pglColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	if ((item->VertColF != NULL) && (item->MatSource == MATSOURCE_OBJECT))
	{
		pglEnableClientState(GL_COLOR_ARRAY);
		pglColorPointer(4, GL_FLOAT, sizeof(D3DCOLORVALUE), item->VertColF);
	}
	else
	{
		pglDisableClientState(GL_COLOR_ARRAY);
	}

// vykreslení objektu
	pglDrawElements(GL_TRIANGLES, item->FaceNum*3, GL_UNSIGNED_INT, item->Face);

	if (isPic)
	{
		MemFree(buf);
		pglPopMatrix();
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky textury pøed ukonèením ovladaèe

void D3GL1TextureRes(D3DTITEM* item)
{
	if (item->Data1 != NULL)
	{
		pglDeleteTextures(1, (GLuint*)&(item->Data1));
		item->Data1 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení a zapnutí textury

void D3GL1TextureSet(D3DTITEM* item, int stage)
{
	if (stage != 0) return;

	pglEnable(GL_TEXTURE_2D);

	if (item->Data1 == NULL)
	{
		PICTUREDATA* picdata = item->Pic.Data();
		pglGenTextures(1, (GLuint*)&(item->Data1));
		pglBindTexture(GL_TEXTURE_2D, (GLuint)item->Data1);
		GLTexture(picdata, item->Mips);
	}

	pglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	pglBindTexture(GL_TEXTURE_2D, (GLuint)item->Data1);
	// po pøíkazu glBindTexture jsou pøepínaèe neplatné
	ResetD3DSwitch();
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
