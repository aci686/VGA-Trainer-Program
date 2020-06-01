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


  #define VGA 0xa0000
  #define MAXLINES 12     // the number of lines in our cube

#define ABS(x) ( ((x) &gt;= 0) ? (x) : (x * -1) )
#define round(x) floor( (x) + 0.5)

//          //
// TYPEDEFS //
//          //

  typedef unsigned char byte;
  typedef unsigned int  word;

  // The data on every point we rotate
  typedef struct {
	 float    x;
	 float    y;
	 float    z;
  } Point;

//           //
// CONSTANTS //
//           //
  const PIE = 3.1415927;

  // The 3-D coordinates of our object ... stored as {X1,Y1,Z1},
  // {X2,Y2,Z2} ... for the two ends of a line
  const int Obj[MAXLINES][2][3] =
    { {{-10,-10,-10}, { 10,-10,-10}}, //  0        .-----2----.
      {{-10,-10,-10}, {-10, 10,-10}}, //  1       /|         /|
      {{-10, 10,-10}, { 10, 10,-10}}, //  2      9 |        A |
      {{ 10,-10,-10}, { 10, 10,-10}}, //  3     /  |       /  |
      {{-10,-10, 10}, { 10,-10, 10}}, //  4    .------6---.   3
      {{-10,-10, 10}, {-10, 10, 10}}, //  5    |   |      |   |
      {{-10, 10, 10}, { 10, 10, 10}}, //  6    |   1      7   |
      {{ 10,-10, 10}, { 10, 10, 10}}, //  7    |   |      |   |
      {{-10,-10, 10}, {-10,-10,-10}}, //  8    5   '----0-|---'
      {{-10, 10, 10}, {-10, 10,-10}}, //  9    |  /       |  /
      {{ 10, 10, 10}, { 10, 10,-10}}, //  A    | 8        | B
      {{ 10,-10, 10}, { 10,-10,-10}}  //  B    |/         |/
    };                                //       `-----4----'



 Point Translated[MAXLINES][2];  	// The rotated object
 Point Lines[MAXLINES][2];    			// The base object rotated

//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  float Lookup[360][2];            // Our sin and cos lookup table
  int Xoff, Yoff, Zoff;            // Used for movement of the objects

  __dpmi_regs regs;

  char screen_buffer [64000]; // screen is 320x200


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


//ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ
// This puts a pixel on the virtual screen by writing directly to memory.
void Putpixel2(unsigned int x, unsigned int y, unsigned char col)
{
    screen_buffer[y * 320 + x] = col;

}

/////////////////////////////////////////////////////////////////////////////
void Flip(void)
{
	// This flips the virtual screen to the VGA screen.

		  dosmemput(screen_buffer,64000,0xa0000);

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


void Line(int a,int b,int c,int d,int col)
{

//ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ
int sgn(float a)
	{

		  if (a&gt;0) return (+1);
		  if (a&lt;0) return (-1);
		  if (a==0) return (0);
		  return 0;

	}


  // This draws a line from a,b to c,d of color col.

  float u,s,v,d1x,d1y,d2x,d2y,m,n;
  int i;

  float a1=a, b1=b, c1=c, d1=d;

     u = c1 - a1;
     v = d1 - b1;

     d1x= sgn(u);
     d1y= sgn(v);
     d2x= sgn(u);
     d2y= 0;
     m= ABS(u);
     n = ABS(v);

     if ( !(m&gt;n) )
     {
          d2x =0;
          d2y = sgn(v);
          m = ABS(v);
          n = ABS(u);
     }
     s = (int)(m / 2);


     
     for(i=0;i&lt;=round(m);i++)
	  {
			 Putpixel2(a,b,col);
			 s = s + n;
			 if (!(s&lt;m))
			 {
					s = s - m;
					a= a +round(d1x);
					b = b + round(d1y);
			 }
			 else
			 {
					a = a + round(d2x);
					b = b + round(d2y);
			 }
	  }

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
// DrawLogo() - This draws 'SNOWMAN' at the top of the screen in little    //
//              balls.                                                     //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void DrawLogo() {

  const byte ball[5][5] = { 0,1,1,1,0,
									 1,4,3,2,1,
									 1,3,3,2,1,
									 1,2,2,2,1,
									 0,1,1,1,0
								  };

  const char *Logo[5] =      {
									  "OOO OOO OOO O O O  O O  OOO OOO",
									  "O   O O O O O O O O O O O O O O",
									  "OOO O O O O O O O O O O OOO O O",
									  "  O O O O O O O O O O O O O O O",
									  "OOO O O OOO  O O  O O O O O O O",
									  };

  int loop1, loop2, loop3, loop4;

  Pal(13, 0,63, 0); // set the color for the cube lines
  Pal( 1, 0, 0,40); // set the colors for the dots
  Pal( 2, 0, 0,45);
  Pal( 3, 0, 0,50);
  Pal( 4, 0, 0,60);

  for (loop1=0; loop1&lt;5; loop1++)        // for each line...
	 for (loop2=0; loop2&lt;31; loop2++)       // is it active?
		if (Logo[loop1][loop2] == 'O')
		for (loop3=0; loop3&lt;5; loop3++)        // y coordinate of the ball
		  for (loop4=0; loop4&lt;5; loop4++)        // x coordinate of the ball
			 Putpixel2 ((loop2+1)*10+loop3, (loop1+1)*4+loop4,
						  ball[loop3][loop4]);

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetUpPoints() - This sets the basic offsets of the object, creates the  //
//                 lookup table, and moves the object from a constant to a //
//                 variable.                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpPoints() {

  int loop1;

  // set the starting offsets of the cube
  Xoff =  160;
  Yoff =  100;
  Zoff = -256;

  // generate the sin() and cos() tables
  for (loop1=0; loop1&lt;361; loop1++) {
	 Lookup [loop1][0] = sin(rad(loop1));
	 Lookup [loop1][1] = cos(rad(loop1));
  }

  // move the Obj constant array into the Lines array
  for (loop1=0; loop1&lt;MAXLINES; loop1++) {
	 Lines[loop1][0].x = Obj[loop1][0][0];
	 Lines[loop1][0].y = Obj[loop1][0][1];
	 Lines[loop1][0].z = Obj[loop1][0][2];
	 Lines[loop1][1].x = Obj[loop1][1][0];
	 Lines[loop1][1].y = Obj[loop1][1][1];
	 Lines[loop1][1].z = Obj[loop1][1][2];
  }

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// RotatePoints() - This rotates object lines by X, Y, and Z.  Then it     //
//                  places the result in Translated.                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void RotatePoints (int X,int Y,int Z) {

  int loop1;

  Point temp;


  // for each line...
  for (loop1=0; loop1&lt;MAXLINES; loop1++) {

    // start point of line

    temp.x = Lines[loop1][0].x;
    temp.y = Lookup[X][1]*Lines[loop1][0].y - Lookup[X][0]*Lines[loop1][0].z;
    temp.z = Lookup[X][0]*Lines[loop1][0].y + Lookup[X][1]*Lines[loop1][0].z;
    Translated[loop1][0] = temp;

	 if (Y &gt; 0) {
      temp.x = Lookup[Y][1]*Translated[loop1][0].x - Lookup[Y][0]*Translated[loop1][0].y;
      temp.y = Lookup[Y][0]*Translated[loop1][0].x + Lookup[Y][1]*Translated[loop1][0].y;
      temp.z = Translated[loop1][0].z;
      Translated[loop1][0] =temp;
    }

    if (Z &gt; 0) {
      temp.x = Lookup[Z][1] * Translated[loop1][0].x + Lookup[Z][0]*Translated[loop1][0].z;
      temp.y = Translated[loop1][0].y;
      temp.z = (-Lookup[Z][0])*Translated[loop1][0].x + Lookup[Z][1]*Translated[loop1][0].z;
      Translated[loop1][0] = temp;
    }

	 // end point of line

    temp.x = Lines[loop1][1].x;
    temp.y = cos(rad(X))*Lines[loop1][1].y - sin(rad(X))*Lines[loop1][1].z;
    temp.z = sin(rad(X))*Lines[loop1][1].y + cos(rad(X))*Lines[loop1][1].z;
    Translated[loop1][1] = temp;

    if (Y &gt; 0) {
      temp.x = cos(rad(X))*Translated[loop1][1].x - sin(rad(Y))*Translated[loop1][1].y;
      temp.y = sin(rad(Y))*Translated[loop1][1].x + cos(rad(Y))*Translated[loop1][1].y;
		temp.z = Translated[loop1][1].z;
      Translated[loop1][1] = temp;
    }

    if (Z &gt; 0) {
      temp.x = cos(rad(Z))*Translated[loop1][1].x + sin(rad(Z))*Translated[loop1][1].z;
      temp.y = Translated[loop1][1].y;
      temp.z = (-sin(rad(Z)))*Translated[loop1][1].x + cos(rad(Z))*Translated[loop1][1].z;
      Translated[loop1][1] = temp;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// DrawPoints() - This draws the translated object to the virtual screen.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void DrawPoints() {

  int loop1, nx, ny, nx2, ny2, temp;

  for (loop1=0; loop1&lt;MAXLINES; loop1++) {
	 if ((Translated[loop1][0].z+Zoff&lt;0) &amp;&amp; (Translated[loop1][1].z+Zoff&lt;0)) {

      // start point of line
      temp = Translated[loop1][0].z + Zoff;
      nx   = ((256*Translated[loop1][0].x) / temp) + Xoff;
      ny   = ((256*Translated[loop1][0].y) / temp) + Yoff;

      // end point of line
      temp = Translated[loop1][1].z + Zoff;
      nx2  = ((256*Translated[loop1][1].x) / temp) + Xoff;
      ny2  = ((256*Translated[loop1][1].y) / temp) + Yoff;

      // check to make sure the line is within bounds
      if ((nx &gt;-1) &amp;&amp; (nx &lt;320) &amp;&amp; (ny &gt;25) &amp;&amp; (ny &lt;200) &amp;&amp;
			 (nx2&gt;-1) &amp;&amp; (nx2&lt;320) &amp;&amp; (ny2&gt;25) &amp;&amp; (ny2&lt;200))
		  Line(nx,ny,nx2,ny2,13);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// ClearPoints() - This clears the translated object from the virtual      //
//                 screen ... believe it or not, this is faster than a     //
//                 straight cls(0,vaddr);                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void ClearPoints() {

  int loop1, nx, ny, nx2, ny2, temp;

  for (loop1=0; loop1&lt;MAXLINES; loop1++) {
    if ((Translated[loop1][0].z+Zoff&lt;0) &amp;&amp; (Translated[loop1][1].z+Zoff&lt;0)) {

      // start point of line
      temp = Translated[loop1][0].z + Zoff;
      nx   = ((256*Translated[loop1][0].x) / temp) + Xoff;
		ny   = ((256*Translated[loop1][0].y) / temp) + Yoff;

      // end point of line
      temp = Translated[loop1][1].z + Zoff;
      nx2  = ((256*Translated[loop1][1].x) / temp) + Xoff;
      ny2  = ((256*Translated[loop1][1].y) / temp) + Yoff;

      // check to make sure the line is within bounds
      if ((nx &gt;-1) &amp;&amp; (nx &lt;320) &amp;&amp; (ny &gt;25) &amp;&amp; (ny &lt;200) &amp;&amp;
          (nx2&gt;-1) &amp;&amp; (nx2&lt;320) &amp;&amp; (ny2&gt;25) &amp;&amp; (ny2&lt;200))
		  Line(nx,ny,nx2,ny2,0);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// MoveAround() - This is the main display function.  First it brings the  //
//                object towards the viewer by increasing the Zoff, then   //
//                it passes control to the user.                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void MoveAround() {

  // For some reason, the values we defined Xoff, Yoff, and Zoff to be in
  // the function SetUpPoints() won't hold until this point.  If you know
  // the reason, please send it to r3cgm@dax.cc.uakron.edu
  Xoff =  160;  // redefined
  Yoff =  100;  // redefined
  Zoff = -256;  // redefined

  int deg=0, loop1;
  byte ch=1; // assign a dummy value to ch

  Cls(0);

  DrawLogo();

  for (loop1=(-256); loop1&lt;(-39); loop1++) {
	 Zoff = loop1 * 2;
	 RotatePoints(deg,deg,deg);
	 DrawPoints();
	 WaitRetrace();
	 Flip();
	 ClearPoints();
	 deg = (deg + 5) % 360;
  }

  do {

	 if (kbhit()) {
		ch = getch();
		switch (ch) {
		  // We are not going to use toupper() because if we did, we'd have
		  // to include the whole ctype.h file.  This might take a little more
		  // time, but the program will be smaller.  We already have 7 include
		  // files, and its getting a bit rediculous.
		  case 'A': case 'a': Zoff += 5; break;  // away
		  case 'Z': case 'z': Zoff -= 5; break;  // toward
		  case ',':           Xoff -= 5; break;  // left
		  case '.':           Xoff += 5; break;  // right
		  case 'S': case 's': Yoff -= 5; break;  // down
		  case 'X': case 'x': Yoff += 5; break;  // up
		}
	 }
	 DrawPoints();
	 WaitRetrace();
	 Flip();
	 ClearPoints();
	 RotatePoints(deg,deg,deg);
	 deg = (deg + 5) % 360;

	 // if the key pressed above was 0 (i.e. a control character) then
	 // read the character code
	 if (ch == 0) ch = getch();

  } while (ch != 27); // if the escape code was 27 (escape key) then exit
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int main(void) {

  printf(
	 "Greetings and salutations! Hope you had a great Christmas and New\n"
	 "year! ;-) ... Anyway, this tutorial is on 3-D, so this is what is\n"
	 "going to happen ... a wireframe square will come towards you.\n"
	 "When it gets close, you get control. ""A"" and ""Z"" control the Z\n"
	 "movement, "","" and ""."" control the X movement, and ""S"" and ""X""\n"
	 "control the Y movement. I have not included rotation control, but\n"
	 "it should be easy enough to put in yourself ... if you have any\n"
	 "hassles, leave me mail.\n\n");
  printf(
	 "Read the main text file for ideas on improving this code ... and\n"
	 "welcome to the world of 3-D!\n\n");
  printf("Hit any key to contine ...\n");
  getch();

  SetMCGA();

  SetUpPoints();
  MoveAround();

  SetText();

  printf(
	 "All done. This concludes the eigth sample program in the ASPHYXIA\n"
	 "Training series. You may reach DENTHOR under the names of GRANT\n"
	 "SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid\n"
	 "Connectix BBS user, and occasionally read RSAProg.\n"
	 "For discussion purposes, I am also the moderator of the Programming\n"
	 "newsgroup on the For Your Eyes Only BBS.\n"
	 "The numbers are available in the main text. You may also write to me at:\n"
	 "             Grant Smith\n"
	 "             P.O. Box 270\n"
	 "             Kloof\n"
	 "             3640\n"
	 "I hope to hear from you soon!\n\n\n");
  printf("Hit any key to exit ...\n");
  getch();
  return (0);
}
