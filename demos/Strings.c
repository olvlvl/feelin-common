/*
	Written by Olivier LAVIALE <gofromiel@gofromiel.com>

	This demo shows how to use and customise FC_String objects.
*/

///Header

#include <libraries/feelin.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <proto/feelin.h>

struct FeelinBase                  *FeelinBase;

#ifdef __amigaos4__
struct FeelinIFace                 *IFeelin;
#else
#define GfxBase                     FeelinBase -> Graphics
#endif

///drawstri
SAVEDS void drawstri(struct RastPort *rp,struct FeelinRect *rect,uint32 a,uint32 b)
{
	WORD  x1 = rect -> x1,
			x3 = rect -> x2,
			x2 = (x3 - x1) / 3 + x1,
			y1 = rect -> y1,
			y2 = rect -> y2;

	static UWORD pt[] = {0xFF00,0x7F80,0x3FC0,0x1FE0,
								0x0FF0,0x07F8,0x03FC,0x01FE,
								0x00FF,0x807F,0xC03F,0xE01F,
								0xF00F,0xF807,0xFC03,0xFE01};

	_APen(a);
	_BPen(b);
	rp -> AreaPtrn = pt;
	rp -> AreaPtSz = 4;
	_Boxf(x2+1,y1,x3,y2);
	rp -> AreaPtSz = 0;
	rp -> AreaPtrn = NULL;

	_APen(b);
	_Boxf(x1,y1,x2,y2);
}
//+
///drawback
F_HOOKM(void,drawback,FS_ImageDisplay_HookDraw)
{
	drawstri(Msg -> Render -> RPort,Msg -> Region,Msg -> Render -> Palette -> Pens[FV_Pen_Fill],Msg -> Render -> Palette -> Pens[FV_Pen_HalfShadow]);
}
//+
///drawcursor
F_HOOKM(void,drawcursor,FS_ImageDisplay_HookDraw)
{
	struct RastPort  *rp = Msg -> Render -> RPort;
	WORD              x1 = Msg -> Rect -> x1,
							x2 = Msg -> Rect -> x2,
							y1 = Msg -> Rect -> y1,
							y2 = Msg -> Rect -> y2;
	uint32             ap = Msg -> Render -> Palette -> Pens[FV_Pen_Shine],
							bp = Msg -> Render -> Palette -> Pens[FV_Pen_Shadow],
							cp = Msg -> Render -> Palette -> Pens[FV_Pen_HalfShine];

	drawstri(rp,Msg -> Region,ap,cp);

	_APen(ap); _Move(x1+1,y2); _Draw(x2,y2); _Draw(x2,y1+1);
	_APen(bp); _Move(x1,y2-1); _Draw(x1,y1); _Draw(x2-1,y1);
}
//+
///drawcursor2
F_HOOKM(void,drawcursor2,FS_ImageDisplay_HookDraw)
{
	drawstri(Msg -> Render -> RPort,Msg -> Region,Msg -> Render -> Palette -> Pens[FV_Pen_Shine],Msg -> Render -> Palette -> Pens[FV_Pen_HalfShine]);
}
//+
///drawblink
F_HOOKM(void,drawblink,FS_ImageDisplay_HookDraw)
{
	struct RastPort  *rp = Msg -> Render -> RPort;
	WORD              x1 = Msg -> Rect -> x1,
							x2 = Msg -> Rect -> x2,
							y1 = Msg -> Rect -> y1,
							y2 = Msg -> Rect -> y2;
	uint32             ap = Msg -> Render -> Palette -> Pens[FV_Pen_HalfShine],
							bp = Msg -> Render -> Palette -> Pens[FV_Pen_HalfDark],
							cp = Msg -> Render -> Palette -> Pens[FV_Pen_Fill];

	drawstri(rp,Msg -> Region,ap,cp);

	_APen(ap); _Move(x1+1,y2); _Draw(x2,y2); _Draw(x2,y1+1);
	_APen(bp); _Move(x1,y2-1); _Draw(x1,y1); _Draw(x2-1,y1);
}
//+

static struct Hook hook_drawback     	= { { NULL,NULL }, (HOOKFUNC) drawback,     NULL,NULL };
static struct Hook hook_drawcursor   	= { { NULL,NULL }, (HOOKFUNC) drawcursor,   NULL,NULL };
static struct Hook hook_drawcursor2  	= { { NULL,NULL }, (HOOKFUNC) drawcursor2,  NULL,NULL };
static struct Hook hook_drawblink  		= { { NULL,NULL }, (HOOKFUNC) drawblink,    NULL,NULL };
//+

STATIC F_ENTITIES_ARRAY =
{
	F_ENTITIES_ADD_EMPTY("hook:back"),
	F_ENTITIES_ADD_EMPTY("hook:cursor"),
	F_ENTITIES_ADD_EMPTY("hook:cursor2"),
	F_ENTITIES_ADD_EMPTY("hook:blink"),

	F_ARRAY_END
};

enum    {
	
		FV_ENTITY_HOOK_BACK,
		FV_ENTITY_HOOK_CURSOR,
		FV_ENTITY_HOOK_CURSOR2,
		FV_ENTITY_HOOK_BLINK
 
		};

///Main
int main(void)
{
	if (F_FEELIN_OPEN)
	{
		FObject app;
 
		STRPTR myback = IFEELIN F_StrNew(NULL, "0x%08lx",&hook_drawback);
		STRPTR mycursor = IFEELIN F_StrNew(NULL, "0x%08lx",&hook_drawcursor);
		STRPTR mycursor2 = IFEELIN F_StrNew(NULL, "0x%08lx",&hook_drawcursor2);
		STRPTR myblink = IFEELIN F_StrNew(NULL, "0x%08lx",&hook_drawblink);
		
		F_ENTITIES_SET(FV_ENTITY_HOOK_BACK, myback);
		F_ENTITIES_SET(FV_ENTITY_HOOK_CURSOR, mycursor);
		F_ENTITIES_SET(FV_ENTITY_HOOK_CURSOR2, mycursor2);
		F_ENTITIES_SET(FV_ENTITY_HOOK_BLINK, myblink);
 
		app = XMLApplicationObject,

			"Source", "strings.xml",
			"SourceType", FV_Document_SourceType_File,
			"Entities", F_ENTITIES_PTR,
	  
			End;
			
		if (app)
		{
			IFEELIN F_Do(app,(uint32) "Run");

			IFEELIN F_DisposeObj(app);
		}
		
		IFEELIN F_Dispose(myback);
		IFEELIN F_Dispose(mycursor);
		IFEELIN F_Dispose(mycursor2);
		IFEELIN F_Dispose(myblink);
		
		F_FEELIN_CLOSE;
	}
	else
	{
		IDOS_ Printf("Unable to open feelin.library v%ld\n",FV_FEELIN_VERSION);
	}
	return 0;
}
//+
