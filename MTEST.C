/****************** #pragma inline  ******************************/
#include <stdio.h>
#include <dos.h>
#include <conio.h>

#include "mouse.h"


void near clear(void);
/* clears the screen */

void near gotoyx(int y, int x);
/* position the cursor */





main()
{
  char c;
  int button,xpos,ypos;
  int status,count;

  if ( ! mouse_init()){
      printf("\nMOUSE NOT INSTALLED!!\n");
      exit(1);
      }
  clear();
  gotoyx(0,0);
  printf("Move the mouse and press the buttons,\n");
  printf("then press any key to go on.\n");
  settext(0,0xffff,0x7700);
  pos_mouse(16,24);
  show_cursor();
  setspeed(15,15);/* set the ratio of mouse movement to cursor movement */
  while (! kbhit()) {
      get_status(&button,&xpos,&ypos); /* get status of buttons and position */
      if (button>0) {
          xpos/=8; /* the positions returned are in pixels, 8x8 per char */
          ypos/=8;
          hide_cursor();     /* hide cursor during screen update, otherwise */
          gotoyx(ypos,xpos); /* when you move the mouse the screen goes back */
          if (button == 1) {
              putchar('\xAE');
              gotoyx(23,0);
              printf(" Left button pressed, at %3d,%3d",xpos,ypos);
          }else if (button == 2) {
              putchar('\xAF');
              gotoyx(23,0);
              printf("Right button pressed, at %3d,%3d",xpos,ypos);
          }else{	/* then button == 3, both buttons were pressed */
              putchar('\xD8');
              gotoyx(23,0);
              printf("Both buttons pressed, at %3d,%3d",xpos,ypos);
              }
          show_cursor();
          }

  } /* end while */
  getch(); /* get rid of the character that was pressed */
  hide_cursor();
  clear();
  printf("This time let's limit the mouse to the center area of the screen\n");
  printf("Notice the mouse cursor has new boundaries. press any key...\n");
  pos_mouse(40*8,12*8);
  setvbounds(9*8,20*8);
  sethbounds(20*8,60*8);
  show_cursor();
  b_press(0,&status,&count,&xpos,&ypos);
  while (! kbhit() ) {
      get_status(&button,&xpos,&ypos);
      gotoyx(ypos/8,xpos/8);
      if (button >2){ /* if both buttons pressed */
	  hide_cursor();
          puts("Both buttons pressed...");
          show_cursor();
          }
      }
  getch(); /* get rid of the character that was pressed */
  hide_cursor();
}
