/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// TUTPROG6.CPP - VGA Trainer Program 6 (in Turbo C++ 3.0)                 //
//                                                                         //
// "The VGA Trainer Program" is written by Denthor of Asphyxia. However it //
// was limited to Pascal only in its first run.  All I have done is taken  //
// his original release, translated it to C++ and touched up a few things. //
// I take absolutely no credit for the concepts presented in this code and //
// am NOT the person to ask for help if you are having trouble.            //
//                                                                         //
// Program Notes : This program demonstrates the advantages of             //
//                 pregenerated arrays.                                    //
//                                                                         //
//                 The Compact memory model (-mc) seems to provide the     //
//                 best results for this tutorial.  Remember, use this     //
//                 memory model when you have little code and lots of      //
//                 data.                                                   //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : January 4, 1995                                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//               //
// INCLUDE FILES //
//               //

  #include &lt;conio.h&gt;
			   // clrscr(), getch(), kbhit(), gotoxy(),
			   // directvideo
  #include &lt;dos.h&gt;
			   // MK_FP, geninterrupt()
  #include &lt;iostream.h&gt;
			   // cout, memset(), memmove()
  #include &lt;math.h&gt;
			   // sin(), cos()
  #include &lt;stdlib.h&gt;
			   // calloc(), free(), exit()

//           //
// CONSTANTS //
//           //

  const PI = 3.1415927;
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

  // pointer to the offset of the VGA memory
  unsigned char *vga = (unsigned char *) MK_FP(0xA000, 0);

  // This is our temporary pallette.  We only use colors 1 to 20, so we
  // only have variables for those ones.
  unsigned char Pall[18][3];


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void main() {

  int loop1;

  clrscr();
  cout
    &lt;&lt; "Hi there! This program will demonstrate the usefullness of\n"
    &lt;&lt; "pregenerated arrays, also known as lookup tables. The program\n"
    &lt;&lt; "will first draw a spiral without using a lookup table, rotate\n"
    &lt;&lt; "the pallette until a key is pressed, the calculate the lookup\n"
    &lt;&lt; "table, then draw the same spiral using the lookup table.\n\n";
  cout
    &lt;&lt; "This is merely one example for the wide range of uses of a\n"
    &lt;&lt; "lookup table.\n\n";
  cout
    &lt;&lt; "Hit any key to contine ...";
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
  cout
    &lt;&lt; "All done. This concludes the sixth sample program in the ASPHYXIA\n"
    &lt;&lt; "Training series. You may reach DENTHOR under the name of GRANT\n"
    &lt;&lt; "SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n"
    &lt;&lt; "ASPHYXIA BBS. I am also an avid Connectix BBS user.\n"
    &lt;&lt; "Get the numbers from Roblist, or write to :\n"
    &lt;&lt; "             Grant Smith\n"
    &lt;&lt; "             P.O. Box 270\n"
    &lt;&lt; "             Kloof\n"
    &lt;&lt; "             3640\n"
    &lt;&lt; "I hope to hear from you soon!\n\n";
  cout
    &lt;&lt; "Hit any key to exit ...";
  getch();

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetMCGA() - This function gets you into 320x200x256 mode.               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetMCGA() {
  _AX = 0x0013;
  geninterrupt (0x10);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetText() - This function gets you into text mode.                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetText() {
  _AX = 0x0003;
  geninterrupt (0x10);
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Cls() - This clears the screen to the specified color.                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Cls(unsigned char Col) {
  memset(vga, Col, 0xffff);
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

void Putpixel (int x, int y, unsigned char Col) {
  memset(vga+x+(y*320),Col,1);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// WaitRetrace() - This waits until you are in a Verticle Retrace.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void WaitRetrace() {

  _DX = 0x03DA;

  l1: asm {
	in  al,dx;
	and al,0x08;
	jnz l1;
      }

  l2: asm {
	in  al,dx;
	and al,0x08;
	jz  l2;
      }
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// rad() - This calculates the degrees of an angle.                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

float rad(float theta) {
  return ((theta * PI)/180);
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
  cout &lt;&lt; "Without pregenerated arrays.";

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
    cout &lt;&lt; "Insufficient memory for lookup tables, exiting...";
    exit(1);
  }

  Cls(0);
  gotoxy(1,1);
  cout &lt;&lt; "Generating variables....";

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
  cout &lt;&lt; "With pregenerated arrays.";

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
