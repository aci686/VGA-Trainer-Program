#include &lt;stdio.h&gt;
#include &lt;string.h&gt; // for the memcpy() function

#define VGA 0xa0000

unsigned char virscr[64000]; // our virtual screen

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This procedure gets you into 320x200x256 mode.

void Readkey(void)
{
        #pragma aux Readkey =   \
        "mov ax, 0001h",        \
        "int 16h"               \
        modify [ax];
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This procedure gets you into 320x200x256 mode.

void SetMCGA(void)
{
        #pragma aux SetMCGA =   \
        "mov ax, 0013h",        \
        "int 10h"               \
        modify [ax];
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//  This procedure returns you to text mode.

void SetText(void)
{
        #pragma aux SetText =   \
        "mov ax, 0003h",        \
        "int 10h"               \
        modify [ax];
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void WaitRetrace(void)
  // This waits until you are in a Verticle Retrace ... this means that all
  //  screen manipulation you do only appears on screen in the next verticle
  //  retrace ... this removes most of the "fuzz" that you see on the screen
  //  when changing the pallette. It unfortunately slows down your program
  //  by "synching" your program with your monitor card ... it does mean
  //  that the program will run at almost the same speed on different
  //  speeds of computers which have similar monitors. In our SilkyDemo,
  //  we used a WaitRetrace, and it therefore runs at the same (fairly
  //  fast) speed when Turbo is on or off. 

{
  while(inp(0x3DA) &amp; 0x08);
  while(!(inp(0x3DA) &amp; 0x08 ));
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This puts a pixel on the VGA screen by writing directly to memory.

void putpixel1(unsigned int x, unsigned int y, unsigned char col)
{
        *((char*) VGA + (y*320) + x) = col;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This puts a pixel on the virtual screen by writing directly to memory.

void putpixel2(unsigned int x, unsigned int y, unsigned char col)
{
        virscr[y*320 + x] = col;
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This clears the VGA screen to the specified color

void Cls1(unsigned char col)
{
        memset((char *) VGA, col, 64000);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This clears the virtual screen to the specified color

void Cls2(unsigned char col)
{
        memset((char *) virscr, col, 64000);
}


void flip(void)
{
   // This flips the virtual screen to the VGA screen.

  memcpy((char *) VGA,virscr,64000);

}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void BlockMove(void)
{
   // This tests various ways of moving a block around the screen

  int loop1,loop2,loop3;

  for (loop1=1;loop1&lt;50; loop1++)
  {                                            // This draw a block
    for (loop2=1;loop2&lt;50;loop2++)             // directly to VGA, no
       for (loop3=1;loop3&lt;50;loop3++)          // flipping
         putpixel1 (loop1+loop2,loop3,32);



    cls1 (0);
  }


  for (loop1=1;loop1&lt;50;loop1++)
  {                                             // This draws a block
    for (loop2=1;loop2&lt;50;loop2++)              // to the virtual screen,
      for (loop3=1;loop3&lt;50;loop3++)            // then flips it to VGA
        putpixel2 (loop1+loop2,loop3,32);

    flip();
    cls2 (0);
  }


  for (loop1=1;loop1&lt;50;loop1++)
  {                                             // This draws a block
    for (loop2=1;loop2&lt;50;loop2++)              // to the virtual screen,
      for (loop3=1;loop3&lt;50;loop3++)            // waits for a retrace,
        putpixel2 (loop1+loop2,loop3,32);       // then flips it to VGA
    waitretrace();
    flip();
    cls2 (0);
  }


}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void PatternDraw(void)
{
   // This test the speed of flipping by drawing two patterns and flipping
   //  them
   
int loop1,loop2;

  for (loop1=1;loop1&lt;100;loop1++)               // This draws pattern one
    for (loop2=1;loop2&lt;100;loop2++)             // to the virtual screen
      putpixel2 (loop1,loop2,loop1);            // then flips it to VGA
  flip();

  for (loop1=1;loop1&lt;100;loop1++)               // This draws pattern two
    for (loop2=1;loop2&lt;100;loop2++)             // to the virtual screen
      putpixel2 (loop1,loop2,loop2);            // then flips it to VGA
  flip();
}



int main(void)
{

  printf("This program will demonstrate the power of virtual screens.\n");
  printf("A block will firstly move across the screen, being drawn and\n");
  printf("erased totally on the VGA. Then the same block will move\n");
  printf("across, but will be drawn on the virtual screen and flipped\n");
  printf("to the VGA screen without a retrace (see part 2). The the\n");
  printf("block will go again, with flipping and a retrace.\n");
  printf("\n");
  printf("I will then draw a pattern, flip it to VGA, draw another\n");
  printf("pattern, flip it to VGA, and repeat that until a key is pressed.\n");
  printf("This will demonstrate that even when I put down 10000 pixels,\n");
  printf("then flip them to the VGA, it is still relatively fast.      \n");
  printf("\n");  printf("\n");
  printf("Hit any key to continue ...\n");
  Readkey();
  SetMCGA();

  cls2 (0);           // After you have got the memory for the virtual screen,
                      // it is usually filled with random garbage. It is always
                      // wise to clear the virtual screen directly afterwards

  BlockMove();


  while(!kbhit())
  {
    PatternDraw();
  }


  Readkey();

  SetText();

  printf("All done. This concludes the fourth sample program in the ASPHYXIA\n");
  printf("Training series. You may reach DENTHOR under the name of GRANT\n");
  printf("SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n");
  printf("ASPHYXIA BBS. Get the numbers from Roblist, or write to :\n");
  printf("             Grant Smith\n");
  printf("             P.O. Box 270\n");
  printf("             Kloof\n");
  printf("             3640\n");
  printf("I hope to hear from you soon!\n");
  printf("\n");  printf("\n");
  printf("Hit any key to exit ...\n");
  Readkey();

  return 0;

} // end of main program
