/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// TUTPROG2.CPP - VGA Trainer Program 2 (in Turbo C++ 3.0)                 //
//                                                                         //
// "The VGA Trainer Program" is written by Denthor of Asphyxia. However it //
// was limited to Pascal only in its first run.  All I have done is taken  //
// his original release, translated it to C++ and touched up a few things. //
// I take absolutely no credit for the concepts presented in this code and //
// am NOT the person to ask for help if you are having trouble.            //
//                                                                         //
// Program Notes : This program presents many new concepts, including:     //
//                 line drawing, pallette manipulation, and fading.        //
//                                                                         //
//                 If you are compiling this code command line, be sure to //
//                 use the "-ml" parameter (large memory model).           //
//                 Otherwise, the program will compile and link, but will  //
//                 lock up your system.                                    //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : November 29, 1994                                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


//               //
// INCLUDE FILES //
//               //

  #include &lt;conio.h&gt;
			  // getch(), clrscr()
  #include &lt;dos.h&gt;
			  // MK_FP, Geninterrupt()
  #include &lt;math.h&gt;
			  // floor(), ceil()
  #include &lt;mem.h&gt;
			  // memset(), movmem()
  #include &lt;iostream.h&gt;
			  // cout
  #include &lt;stdlib.h&gt;
			  // abs(), rand()


//                     //
// FUNCTION PROTOTYPES //
//                     //

  // MODE SETTING FUNCTIONS
  void SetMCGA();
  void SetText();

  // PALLETTE FUNCTIONS
  void GrabPallette();
  void GetPal (unsigned char ColorNo,  unsigned char &amp;R,
	       unsigned char &amp;G,       unsigned char &amp;B);
  void Pal    (unsigned char ColorNo,  unsigned char R,
	       unsigned char G,        unsigned char B);
  void PalPlay  ();
  void Fadeup   ();
  void FadeDown ();
  void Blackout ();
  void RestorePallette();

  // SMALL UTILITY FUNCTIONS
  int  sgn  (long a);
  int  round(long a);

  // DRAWING FUNCTIONS
  void Putpixel (int x, int y, unsigned char Col);
  void Line(int a, int b, int c, int d, int col);
  void WaitRetrace();

  // MID-LEVEL (COMBINATION) FUNCTIONS
  void SetUpScreen();
  void HiddenScreenSetup();


//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  // declare a pointer to the offset of VGA memory
  unsigned char *vga = (unsigned char *) MK_FP(0xA000, 0);

  // This declares the PALL variable. 0 to 255 signifies the colors of the
  // pallette, 1 to 3 signifies the Red, Green and Blue values. I am
  // going to use this as a sort of "virtual pallette", and alter it
  // as much as I want, then suddenly bang it to screen. Pall2 is used
  // to "remember" the origional pallette so that we can restore it at
  // the end of the program. */
  unsigned char Pall[256][3], Pall2[256][3];


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void main() {

  clrscr();
  cout &lt;&lt; "This program will draw lines of different colors across the\n"
       &lt;&lt; "screen and change them only by changing their pallette values.\n"
       &lt;&lt; "The nice thing about using the pallette is that one pallette\n"
       &lt;&lt; "change changes the same color over the whole screen, without\n"
       &lt;&lt; "you having to redraw it. Because I am using a WaitRetrace\n"
       &lt;&lt; "command, turning on and off your turbo during the demonstration\n"
       &lt;&lt; "should have no effect." &lt;&lt; endl &lt;&lt; endl;

  cout &lt;&lt; "The second part of the demo blacks out the screen using the\n"
       &lt;&lt; "pallette, fades in the screen, waits for a keypress, then fades\n"
       &lt;&lt; "it out again. I haven''t put in any delays for the fadein/out,\n"
       &lt;&lt; "so you will have to put them in yourself to get to the speed\n"
       &lt;&lt; "you like. Have fun and enjoy! ;-)" &lt;&lt; endl &lt;&lt; endl &lt;&lt; endl;
  cout &lt;&lt; "Hit any key to continue ...";
  getch();

  SetMCGA();
  GrabPallette();
  SetUpScreen();

  while (!kbhit()) PalPlay();
    getch();  // once a key is pressed, be sure to clear the keyboard buffer

  Blackout();
  HiddenScreenSetup();
  Fadeup();
  getch();
  FadeDown();
  getch();

  RestorePallette();
  SetText();
  cout &lt;&lt; "All done. This concludes the second sample program in the ASPHYXIA\n"
       &lt;&lt; "Training series. You may reach DENTHOR under the name of GRANT\n"
       &lt;&lt; "SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n"
       &lt;&lt; "ASPHYXIA BBS. Get the numbers from Roblist, or write to :\n"
       &lt;&lt; "             Grant Smith\n"
       &lt;&lt; "             P.O. Box 270\n"
       &lt;&lt; "             Kloof\n"
       &lt;&lt; "             3640\n"
       &lt;&lt; "I hope to hear from you soon!" &lt;&lt; endl &lt;&lt; endl &lt;&lt; endl;

  cout &lt;&lt; "Hit any key to exit ...";
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
// GrabPallette() - This function retrieves the current pallette.          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void GrabPallette() {

  for(int loop1=0;loop1&lt;256;loop1++)
    GetPal(loop1,Pall2[loop1][0],Pall2[loop1][1],Pall2[loop1][2]);

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// GetPal() - This reads the values of the Red, Green, and Blue values of  //
//            a certain color.  This function uses pass-by-reference.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void GetPal(unsigned char ColorNo, unsigned char &amp;R,
	    unsigned char &amp;G,      unsigned char &amp;B) {

  outp (0x03C7,ColorNo); // here is the pallette color I want read
  R = inp (0x03C9);
  G = inp (0x03C9);
  B = inp (0x03C9);

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
// PalPlay() - This function mucks about with our "virtual pallette", then //
//             shoves it to the screen.                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void PalPlay() {

unsigned char Tmp[3];

  // This copies color 200 from our virtual pallette to the Tmp variable.
  _fmemmove(Tmp,Pall[200],3);

  // This moves the entire virtual pallette up one color.
  _fmemmove(Pall[2],Pall[1],199*3);

  // This copies the Tmp variable to the bottom of the virtual pallette.
  // Don't change 0: leave this always black to not change overscan color.
  _fmemmove(Pall[1],Tmp,3);

  WaitRetrace();
  for (int loop1=0;loop1&lt;256;loop1++)
   Pal(loop1,Pall[loop1][0], Pall[loop1][1], Pall[loop1][2]);

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Fadeup() - This function slowly fades up the new screen                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Fadeup() {

//This is temporary storage for the values of a color
unsigned char Tmp[3];

  // A color value for Red, green or blue is 0 to 63, so this loop only
  // need be executed a maximum of 64 times.
  for(int loop1=0;loop1&lt;64;loop1++) {

    WaitRetrace();

    for(int loop2=0; loop2&lt;256; loop2++) {
      GetPal(loop2,Tmp[0],Tmp[1],Tmp[2]);

      // If the Red, Green or Blue values of color loop2 are less then they
      // should be, increase them by one.
      if ((Tmp[0] &lt; Pall2[loop2][0]) &amp;&amp; (Tmp[0] &lt; 63)) Tmp[0]++;
      if ((Tmp[1] &lt; Pall2[loop2][1]) &amp;&amp; (Tmp[1] &lt; 63)) Tmp[1]++;
      if ((Tmp[2] &lt; Pall2[loop2][2]) &amp;&amp; (Tmp[2] &lt; 63)) Tmp[2]++;

      // Set the new, altered pallette color.
      Pal (loop2,Tmp[0],Tmp[1],Tmp[2]);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// FadeDown() - This function fades the screen out to black.               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void FadeDown() {

// This is temporary storage for the values of a color
unsigned char Tmp[3];

  for(int loop1=0; loop1&lt;64; loop1++) {

    WaitRetrace();

    for(int loop2=0; loop2&lt;256; loop2++) {
      GetPal(loop2,Tmp[0],Tmp[1],Tmp[2]);

      // If the Red, Green or Blue values of color loop2 are not yet zero,
      // then, decrease them by one.
      if (Tmp[0] &gt; 0) Tmp[0]--;
      if (Tmp[1] &gt; 0) Tmp[1]--;
      if (Tmp[2] &gt; 0) Tmp[2]--;

      // Set the new, altered pallette color.
      Pal(loop2,Tmp[0],Tmp[1],Tmp[2]);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Blackout() - This function just clears the screen.                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Blackout() {

  WaitRetrace();
  for (int loop1=0;loop1&lt;256;loop1++)
    Pal (loop1,0,0,0);

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// RestorePallette() - This function restores the origional pallette.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void RestorePallette() {

  WaitRetrace();
  for(int loop1=0; loop1&lt;255; loop1++)
    Pal(loop1,Pall2[loop1][0],Pall2[loop1][1],Pall2[loop1][2]);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// sgn() - This function is used by Line() to determine the sign of a long //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

int sgn (long a) {

  if (a &gt; 0) return +1;
  else if (a &lt; 0) return -1;
  else return 0;

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// round() - This function is used by Line() to round a long to the        //
//           nearest integer.                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

int round (long a) {
  if ( (a - (int)a) &lt; 0.5) return floor(a);
    else return ceil(a);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Putpixel() - This puts a pixel on the screen by writing directly to     //
//              memory.                                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Putpixel (int x, int y, unsigned char Col) {

  memset(vga+(x+(y*320)),Col,1);

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Line() - This draws a line from a,b to c,d of color col.                //
//          This function will be explained in more detail in tut3new.zip  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Line(int a, int b, int c, int d, int col) {

long u,s,v,d1x,d1y,d2x,d2y,m,n;
int  i;

  u   = c-a;      // x2-x1
  v   = d-b;      // y2-y1
  d1x = sgn(u);   // d1x is the sign of u (x2-x1) (VALUE -1,0,1)
  d1y = sgn(v);   // d1y is the sign of v (y2-y1) (VALUE -1,0,1)
  d2x = sgn(u);   // d2x is the sign of u (x2-x1) (VALUE -1,0,1)
  d2y = 0;
  m   = abs(u);   // m is the distance between x1 and x2
  n   = abs(v);   // n is the distance between y1 and y2

  if (m&lt;=n) {     // if the x distance is greater than the y distance
    d2x = 0;
    d2y = sgn(v); // d2y is the sign of v (x2-x1) (VALUE -1,0,1)
    m   = abs(v); // m is the distance between y1 and y2
    n   = abs(u); // n is the distance between x1 and x2
  }

  s = (int)(m / 2); // s is the m distance (either x or y) divided by 2

  for (i=0;i&lt;round(m);i++) { // repeat this loop until it
			     // is = to m (y or x distance)
    Putpixel(a,b,col);       // plot a pixel at the original x1, y1
    s += n;                  // add n (dis of x or y) to s (dis of x of y)
    if (s &gt;= m) {            // if s is &gt;= m (distance between y1 and y2)
      s -= m;
      a += d1x;
      b += d1y;
    }
    else {
      a += d2x;
      b += d2y;
    }
  }

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// WaitRetrace() - This waits until you are in a Verticle Retrace ...      //
// 		   this means that all screen manipulation you do only     //
//		   appears on screen in the next verticle retrace ... this //
//		   removes most of the "fuzz" that you see on the screen   //
//		   when changing the pallette. It unfortunately slows down //
//		   your program by "synching" your program with your       //
//		   monitor card ... it does mean that the program will run //
//		   at almost the same speed on different speeds of         //
//		   computers which have similar monitors.  In our          //
//		   SilkyDemo, we used a WaitRetrace, and it therefore      //
//		   runs at the same (fairly fast) speed when Turbo is on   //
//		   or off.                                                 //
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
// SetUpScreen() - This gets our screen ready by setting up the pallette   //
//                 and drawing the lines.                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpScreen() {

  // Clear the entire PALL variable to zero.
  _fmemset(Pall,0,sizeof(Pall));

  // This sets colors 0 to 200 in the PALL variable to values between
  // 0 to 63. the modulus function gives you the remainder of a division,
  // ie. 105 % 10 = 5.
  for (int loop = 0; loop &lt; 201;loop++)
    Pall[loop][0] = loop % 64;

  for (loop = 0; loop &lt; 321; loop++) {

    // These two lines start drawing lines from the left and the right
    // hand sides of the screen, using colors 1 to 199. Look at these
    // two lines and understand them.
    Line(319,199,320-loop,0,(loop % 199) + 1);
    Line(0,0,loop,199,(loop % 199) + 1);

    // This calls the PalPlay function.
    PalPlay();
  }

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// HiddenScreenSetup() - This function sets up the screen while it is      //
//                       blacked out, so that the user can't see what is   //
//                       happening.                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void HiddenScreenSetup() {

  for (int loop1=0; loop1&lt;320; loop1++)
    for (int loop2=0; loop2&lt;200; loop2++)
      Putpixel (loop1,loop2, rand());

}
