/* MOUSERTN.C

   Routines for accessing a mouse from Turbo C.
   Calling program should #include <dos.h> and "mouse.h".

   Author: Rodney Loos, Compuserve ID 75756,1446
   These routines released into the public domain,
   feel free to distribute them, use and improve them as you see fit.

   These routines are mainly implementations straight from the mouse users
   guide.  They provide access to the Microsoft mouse driver routines.
   The source should compile under any memory model.  I have only tested it
   under small and large models.

*/

#include <dos.h>
#include "mouse.h"
/*
typedef struct {
        int m1,m2,m3,m4;
        } mparams;*/  /* parameters for mouse calls */

mparams *tail, mp;

void mouse(mparams *p)
{
 union REGS mousreg;

 mousreg.x.ax = p->m1;
 mousreg.x.bx = p->m2;
 mousreg.x.cx = p->m3;
 mousreg.x.dx = p->m4;
 int86(0x33,&mousreg,&mousreg);  /* call the mouse interrupt */
 p->m1 = mousreg.x.ax;
 p->m2 = mousreg.x.bx;
 p->m3 = mousreg.x.cx;
 p->m4 = mousreg.x.dx;
}


/* Microsoft mouse function 0 */
int mouse_init(void)   /* Function returns 0 if mouse software not installed */
{
 union REGS mreg;
 struct SREGS segs;

 tail=&mp;
 if (_osmajor < 2 )
 	return(0);  /* need Dos 2.0 or higher to use these mouse routines */
 tail->m1=tail->m2=tail->m3=tail->m4=0; /* initialize variables to 0 */
 if (_osmajor >= 3)
 	mouse(tail); /* status returned in tail->m1, if 0 then not installed */
 else{  /* its version 2 */
 	mreg.h.ah = 0x35; /* Function to get interrupt vector */
 	mreg.h.al = 0x33; /* mouse interrupt number */
 	intdosx(&mreg,&mreg,&segs);
	if (segs.es == 0 && mreg.x.bx == 0)
		tail->m1=0; /* if vector points to 0000:0000, mouse not in */
	else
		mouse(tail); /* initialize mouse */
 }
 return(tail->m1);
}


void show_cursor(void)  /* Msoft mouse function 1 */
{
 tail->m1=1;
 mouse(tail);
}


void hide_cursor(void)  /* Msoft mouse function 2 */
{
 tail->m1=2;
 mouse(tail);
}


void get_status(int *button, int *curx, int *cury)  /* mouse function 3 */
/* Function returns the status and postion of the mouse */
{
 tail->m1=3;
 mouse(tail);
 *button=tail->m2;
 *curx=tail->m3;
 *cury=tail->m4;
}


void pos_mouse(int x, int y)  /*  mouse function 4 */
/* Function to place the mouse at x,y */
{
 tail->m1=4;
 tail->m3=x;
 tail->m4=y;
 mouse(tail);
}


void b_press(int btn, int *bnow, int *bcount, int *x, int *y) /* function #5 */
/* Function to get information about individual button presses */
{
 tail->m1=5;
 tail->m2=btn;
 mouse(tail);
 *bnow=tail->m1; /* status of button now */
 *bcount=tail->m2; /* number of times pressed since last call to function */
 *x=tail->m3; /* current horizontal position */
 *y=tail->m4; /* current vertical position */
}


/* Microsoft mouse function 7 */
void sethbounds(int l, int r) /* set left and right boundaries */
{
 tail->m1 = 7;
 tail->m3 = l;
 tail->m4 = r;
 mouse(tail);
}


/* mouse function 8 */
void setvbounds(int t, int b) /*set top and bottom bounds */
{
 tail->m1 = 8;
 tail->m3 = t;
 tail->m4 = b;
 mouse(tail);
}


/* mouse function 9 */
void setgraphics(int hspot, int vspot, void far *cmask)
/* Function sets a graphics cursor,
   should now work with all models, but must use "mouse.h"  to prototype
   the pointer to cmask as a far pointer */
{
 struct SREGS msregs;
 union REGS mousreg;
 unsigned maskseg,maskoff;

 maskseg = FP_SEG(cmask);
 maskoff = FP_OFF(cmask);
 mousreg.x.ax=9;
 mousreg.x.bx=hspot; /* horizontal hot spot of cursor */
 mousreg.x.cx=vspot; /* vertical hot spot of cursor */
 mousreg.x.dx=maskoff; /* offset of mask  */
 segread(&msregs); /* copy the current segment registers to msregs */
 msregs.es=maskseg; /*  segment address of cursor mask */
 int86x(0x33,&mousreg,&mousreg,&msregs);
}


void setspeed(int x, int y) /*  function 15 */
/* function to set the mickey to pixel ratio */
{
 tail->m1 = 15; /* function # */
 tail->m3 = x;  /* horizontal mickey/pixel ratio */
 tail->m4 = y;  /* vertical mickey/pixel ratio   */
 mouse(tail);
}



/* mouse function call #6, get button releases */
void b_release( int btn, int *bnow, int *bcount, int *x, int *y)
{
  tail->m1 = 6;  /* mouse call #6 */
  tail->m2 = btn; /* which button to check, 0=left,1=right */
  mouse(tail);
  *bnow = tail->m1;   /* status of button now */
  *bcount = tail->m2; /* number of button releases since last call      */
  		      /* to this function                               */
  *x = tail->m3;      /* horizontal position at last button release     */
  *y = tail->m4;      /* vertical position at last release              */
}


/* mouse function 10, set cursor mode.  Sets cursor to text or software
   mode, and type of cursor                                              */
void settext(int cursor,int scrnmask,int cursmask)
{
  tail->m1 = 10; /* mouse call #10 */
  tail->m2 = cursor;	/* if 0, use software cursor; 1, use hardware cursor */
  tail->m3 = scrnmask;	/* if software cursor, defines screen mask	     */
  			/* if hardware cursor, defines scan line start       */
  tail->m4 = cursmask;	/* if software cursor, defines cursor mask,          */
  			/* if hardware cursor, defines scan line stop.       */
  mouse(tail);
}


/* mouse function 19, set the threshold speed for doubling the cursor's
   movements
*/
void set_threshold(int x)
{
  tail->m1 = 19;	/* function 19 */
  tail->m4 = x;		/* speed threshold */
  mouse(tail);
}



/* function 12 defines a subroutine to be executed conditionally
   by the mouse software
*/
void set_subroutine(int mask,void (far * func)(void))
{
  _ES = FP_SEG(func);	/* segment of function */
  _DX = FP_OFF(func);	/* offset of function  */
  _CX = mask;		/* condition call mask */
  _AX = 12;		/* mouse function 12   */
  geninterrupt(0x33);	/* mouse function call */
  	/*****  note: the subroutine called must do a far return.
                on entry to the subroutine,
                ax contains the condition mask,
                bx contains the button state,
                cx contains the horizontal cursor coordinate,
                dx contains the vertical cursor coordinate.
       *******/
}



/* reads the motion of the mouse */
void motion(int *x,int *y)
{
   tail->m1 = 11;  /* mouse function #11 */
   mouse(tail);
   *x = tail->m3;  /* horizontal distance since last call */
   *y = tail->m4;  /* vertical distance since last call   */
}


/* this function switches the light pen emulation mode on or off.
   I don't know of any c programs that use the light pen emulation,
   but this function is just provided for completeness.
   Light pen emulation mode is on by default after mouse initialization.
   A zero parameter turns it on, any other value turns it off.
*/
void light_pen(int sw)
{
  tail->m1 = (sw ? 13 : 14 );
  mouse(tail);
}



/* This function hides the mouse if it is in the region when this function
   is called.  Afterwards your program must call show_cursor
   to show the cursor again.
*/
void conditional_off(int x1,int y1,int x2,int y2)
{
  _SI = x2;	/* lower x screen coordinates */
  _DI = y2;	/* lower y screen coordinates */
  _AX = 16;	/* mouse function 16          */
  _CX = x1;	/* upper x screen coordinates */
  _DX = y1;	/* upper y screen coordinates */
  geninterrupt(0x33); /* mouse interrupt */
}
