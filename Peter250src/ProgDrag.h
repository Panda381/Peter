
/***************************************************************************\
*																			*
*							Obsluha tažení ikon								*
*																			*
\***************************************************************************/

// parametry tažení
extern	BOOL		Draging;			// pøíznak probíhajícího tažení
extern	int			DragIconWidth;		// šíøka tažené ikony
extern	int			DragIconHeight;		// výška tažené ikony
extern	int			DragHotSpotX;		// bod uchycení prvku X
extern	int			DragHotSpotY;		// bod uchycení prvku Y

/////////////////////////////////////////////////////////////////////////////
// zahájení taženi

void ProgOnBeginDrag(HWND hWnd, TV_ITEM* tvi, int x, int y, BOOL right);


/////////////////////////////////////////////////////////////////////////////
// posun myši pøi tažení

void ProgOnMoveDrag(UINT flags, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// ukonèení tažení

void ProgOnEndDrag();


/////////////////////////////////////////////////////////////////////////////
// pøerušení tažení

void BreakDrag();
