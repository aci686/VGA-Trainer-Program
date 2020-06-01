//               //
// INCLUDE FILES //
//               //
#include &lt;stdio.h&gt;
#include &lt;dpmi.h&gt; // for __dpmi_regs
#include &lt;string.h&gt; // for memset and memcpy
#include &lt;sys/farptr.h&gt;
#include &lt;go32.h&gt; /* for _dos_ds */
#include &lt;sys/movedata.h&gt; // for dosmemput
#include &lt;conio.h&gt; // for kbhit()

#include &lt;math.h&gt;	// sin(), cos()
#include &lt;stdlib.h&gt; // calloc(), free(), exit()

//           //
// CONSTANTS //
//           //

  #define VGA 0xa0000

  const PIE = 3.1415927;
  const TABLESIZE = 900;

//                     //
// FUNCTION PROTOTYPES //
//                     //

  // UTILITY FUNCTIONS
  void  SetMCGA();
  void  SetText();
  void  Cls(unsigned char Col);
  void  Pal(unsigned char ColorNo, unsigned char R,
		 unsigned char G,       unsigned char B);
  void  Putpixel(int x, int y, unsigned char Col);
  void  WaitRetrace();
  float rad(float theta);

  // MID-LEVEL FUNCTIONS
  void  NormCirc();
  void  LookupCirc();
  void  PalPlay();

//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  // This is our temporary pallette.  We only use colors 1 to 20, so we
  // only have variables for those ones.
  unsigned char Pall[18][3];

  __dpmi_regs regs;

  char screen_buffer [64000]; // screen is 320x200  

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int main() {

  int loop1;

  printf (
	 "Hi there! This program will demonstrate the usefullness of\n"
	 "pregenerated arrays, also known as lookup tables. The program\n"
	 "will first draw a spiral without using a lookup table, rotate\n"
	 "the pallette until a key is pressed, the calculate the lookup\n"
	 "table, then draw the same spiral using the lookup table.\n\n");
  printf (
	 "This is merely one example for the wide range of uses of a\n"
	 "lookup table.\n\n");
  printf (
	 "Hit any key to contine ...");
  getch();
  SetMCGA();

  // This handy trick allows you to use gotoxy() and cout in graphics mode.
  directvideo = 0;

  // This sets our pallette colors 0 to 17 with red values between 12 to 63.
  for (loop1=0;loop1&lt;18;loop1++) {
	 Pall[loop1][0] = ((loop1+1)*3)+9;
	 Pall[loop1][1] = 0;
	 Pall[loop1][2] = 0;
  }

  WaitRetrace();

  // This sets the true pallette to variable Pall
  for (loop1=0;loop1&lt;18;loop1++)
	 // loop+1 (make sure to leave color 0 for black)
	 Pal(loop1+1,Pall[loop1][0],Pall[loop1][1],Pall[loop1][2]);

  // This draws a spiral without lookups
  NormCirc();
  while (!kbhit()) PalPlay();
  getch(); // clear the keyboard buffer after kbhit()

  // This draws a spiral with lookups
  LookupCirc();
  while (!kbhit()) PalPlay();
  getch();

  getch();
  SetText();
  printf (
	 "All done. This concludes the sixth sample program in the ASPHYXIA\n"
	 "Training series. You may reach DENTHOR under the name of GRANT\n"
	 "SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n"
	 "ASPHYXIA BBS. I am also an avid Connectix BBS user.\n"
	 "Get the numbers from Roblist, or write to :\n"
	 "             Grant Smith\n"
	 "             P.O. Box 270\n"
	 "             Kloof\n"
	 "             3640\n"
	 "I hope to hear from you soon!\n\n");
  printf (
	 "Hit any key to exit ...");
  getch();

  return (0);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetMCGA() - This function gets you into 320x200x256 mode.               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetMCGA() {
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x13;
  __dpmi_int(0x10, &amp;regs);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetText() - This function gets you into text mode.                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetText() {
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x03;
  __dpmi_int(0x10, &amp;regs);
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Cls() - This clears the screen to the specified color.                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Cls(unsigned char Col) {
        memset(&amp;screen_buffer, Col, 64000);
        dosmemput(screen_buffer,64000,VGA);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Pal() - This sets the Red, Green, and Blue values of a certain color.   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal(unsigned char ColorNo, unsigned char R,
	 unsigned char G,       unsigned char B) {

  outp (0x03C8,ColorNo); // here is the pallette color I want to set
  outp (0x03C9,R);
  outp (0x03C9,G);
  outp (0x03C9,B);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Putpixel() - This puts a pixel at X,Y using color Col, on VGA or the    //
//              Virtual Screen;                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Putpixel (int x, int y, unsigned char col) {
	 _farpokeb(_dos_ds, VGA + x + y * 320, col);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// WaitRetrace() - This waits until you are in a Verticle Retrace.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void WaitRetrace()

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


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// rad() - This calculates the degrees of an angle.                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

float rad(float theta) {
  return ((theta * PIE)/180);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// NormCirc() - This generates a spiral without using a lookup table.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void NormCirc() {

  int loop1,x,y;
  float deg,radius;

  gotoxy(1,1);
  printf ("Without pregenerated arrays.");

  for (loop1=60;loop1&gt;42;loop1--) {
	 deg = 0.0;
	 radius = (float) loop1;

	 do {
		x = radius*cos(rad(deg));
		y = radius*sin(rad(deg));
		Putpixel(x+160,y+100,61-loop1);
		deg += 0.4;          // Increase the degree so the circle is round
		radius -= 0.019;     // Decrease the radius for a spiral effect

		// NOTE: If you change this last statement to "radius -= 0.02;" like
		// the Pascal code has it, you will get holes in the spiral.  You
		// get bonus credit if you can tell me why this is.

	 }
	 while (radius &gt;= 0.0); // continue until the radius is zero (at center)
  }
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// LookupCirc() - This draws a spiral using a lookup table.                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void LookupCirc() {

  int x, y, pos, loop1;
  float radius, deg;
  float *costbl=NULL, *sintbl=NULL;

  // allocate memory for the two tables
  costbl = (float *) calloc(TABLESIZE,sizeof(float));
  sintbl = (float *) calloc(TABLESIZE,sizeof(float));

  // always check to see if enough memory was allocated
  if ((costbl == NULL) || (sintbl == NULL)) {
	 SetText();
	 printf ("Insufficient memory for lookup tables, exiting...");
	 exit(1);
  }

  Cls(0);
  gotoxy(1,1);
  printf ("Generating variables....");

  // There are 360 degrees in a circle.  If you increase the degrees by 0.4,
  // the number of needed parts of the table is 360/0.4=900. (TABLESIZE)
  //
  // For greater accuracy I increase the degrees by 0.4, because if I
  // increase them by one, holes are left in the final product as a
  // result of the rounding error margin. This means the pregenerated array
  // is bigger, takes up more memory and is slower to calculate, but
  // the finished product looks better.

  deg = 0.0;
  for (loop1=0;loop1&lt;TABLESIZE;loop1++) {
	 costbl[loop1] = cos(rad(deg));
	 sintbl[loop1] = sin(rad(deg));
	 deg += 0.4;
  }

  gotoxy(1,1);
  printf ("With pregenerated arrays.");

  // NOTE: The spiral drawn with this function leaves a small piece missing
  // on the right side.  It looks like a pie with a piece cut out.  You get
  // extra credit if you can tell me why this is.

  for (loop1=60;loop1&gt;42;loop1--) {
	 pos = 0;
	 radius = loop1;
	 do {
		// Note how I am not recalculating sin and cos for each point.
		x = radius*costbl[pos];
		y = radius*sintbl[pos];
		Putpixel(x+160,y+100,61-loop1);
		// Decrease the radius for a spiral effect
		radius -= 0.020;
		// I only made a table from 1 to 900, so it must never exceed that,
		// or the program will probably crash.  Go ahead and increment
		// pos while you are at it (more compact but less readable). :)
		if (++pos &gt; TABLESIZE-1) pos = 0;
	 }
	 while (radius &gt;= 0.0);
  }

  // Freeing the memory taken up by the tables. This is very important.
  free(costbl);
  free(sintbl);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PalPlay() - This function mucks about with our "virtual pallette", then //
//             shoves it to the screen.                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void PalPlay() {

  // This is used as a "temporary color" in our pallette
  unsigned char Tmp[3];
  int loop1;

  // This copies color 0 from our virtual pallette to the Tmp variable.
  memmove(Tmp,Pall[0],3);

  // This moves the entire virtual pallette down one color.
  memmove(Pall[0],Pall[1],17*3);

  // This copies the Tmp variable to the bottom of the virtual pallette.
  // Don't change 0: leave this always black to not change overscan color.
  memmove(Pall[17],Tmp,3);

  WaitRetrace();
  for (loop1=0;loop1&lt;18;loop1++)
	Pal(loop1+1,Pall[loop1][0], Pall[loop1][1], Pall[loop1][2]);

}
