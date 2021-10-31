
typedef struct {
        int m1,m2,m3,m4;
        } mparams;  /* parameters for mouse calls */


void mouse(mparams *p);


int mouse_init(void); /* returns 0 if mouse software not installed */

void show_cursor(void);

void hide_cursor(void);

void get_status(int *button, int *curx, int *cury);
/* Function returns the status and postion of the mouse */


void pos_mouse(int x, int y);
/* Function to place the mouse at x,y */


void b_press(int btn, int *bnow, int *bcount, int *x, int *y);
/* Function to get information about individual button presses */


void sethbounds(int l, int r); /* set left and right boundaries */


void setvbounds(int t, int b); /*set top and bottom bounds */



void setgraphics(int hspot, int vspot, void far *cmask);
/* Function sets a graphics cursor,
   should now work with all memory models. */



void setspeed(int x, int y);
/* function to set the mickey to pixel ratio */

void set_threshold(int);
/* set threshold speed for doubling cursor movement */


void set_subroutine(int,void (far *)(void));
/* set subroutine for mouse to conditionally call */
/* the subroutine must be prototyped as void far func (void) */
/* -- it has to do a far return.                             */



void b_release(int,int *,int *,int *,int *);
/* get number of button releases since last call */

void settext(int,int,int);
/* set text mode cursor to hardware cursor or software cursor */


void motion(int *,int *);
/* get distance moved */


void light_pen(int);
/* turn light_pen emulation on or off */


void conditional_off(int,int,int,int);
/* define area of screen to conditionally hide cursor during update */
