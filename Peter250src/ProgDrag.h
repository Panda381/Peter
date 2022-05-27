
/***************************************************************************\
*																			*
*							Obsluha ta�en� ikon								*
*																			*
\***************************************************************************/

// parametry ta�en�
extern	BOOL		Draging;			// p��znak prob�haj�c�ho ta�en�
extern	int			DragIconWidth;		// ���ka ta�en� ikony
extern	int			DragIconHeight;		// v��ka ta�en� ikony
extern	int			DragHotSpotX;		// bod uchycen� prvku X
extern	int			DragHotSpotY;		// bod uchycen� prvku Y

/////////////////////////////////////////////////////////////////////////////
// zah�jen� ta�eni

void ProgOnBeginDrag(HWND hWnd, TV_ITEM* tvi, int x, int y, BOOL right);


/////////////////////////////////////////////////////////////////////////////
// posun my�i p�i ta�en�

void ProgOnMoveDrag(UINT flags, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// ukon�en� ta�en�

void ProgOnEndDrag();


/////////////////////////////////////////////////////////////////////////////
// p�eru�en� ta�en�

void BreakDrag();
