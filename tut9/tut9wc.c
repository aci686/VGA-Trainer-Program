//               //
// INCLUDE FILES //
//               //
#include &lt;conio.h&gt;  // clrscr(), getch(), kbhit()
#include &lt;dos.h&gt;    // MK_FP, geninterrupt()
#include &lt;stdio.h&gt;  // fopen(), fread(), fclose(), FILE
#include &lt;string.h&gt; // for the memcpy() function
#include &lt;math.h&gt;       // sin(), cos()
#include &lt;stdlib.h&gt; // exit(), rand()




  #define VGA 0xa0000


#define ABS(x) ( ((x) &gt;= 0) ? (x) : (x * -1) )
#define round(x) floor( (x) + 0.5)


#define MAXPOLYS   5
#define POLYPOINTS 4
#define POLYCORDS  3
//          //
// TYPEDEFS //
//          //

  typedef unsigned char byte;
  typedef unsigned int  word;

//           //
// CONSTANTS //
//           //
  const PIE = 3.1415927;


//             //
// LETTER DATA //
//             //

  // The 3-D coordinates of our object ... stored as {X1,Y1,Z1},
  // {X2,Y2,Z2} ... for the 4 points of a poly.
  const int A[MAXPOLYS][POLYPOINTS][POLYCORDS] =
                                { {{-10, 10, 0},{ -2,-10, 0},{ 0,-10, 0},{ -5,10, 0}},   // 1
                                  {{ 10, 10, 0},{  2,-10, 0},{ 0,-10, 0},{  5,10, 0}},   // 2
                                  {{ -2,-10, 0},{  2,-10, 0},{ 2, -5, 0},{ -2,-5, 0}},   // 3
                                  {{ -6,  0, 0},{  6,  0, 0},{ 7,  5, 0},{ -7, 5, 0}},   // 4
                                  {{  0,  0, 0},{  0,  0, 0},{ 0,  0, 0},{  0, 0, 0}} }; // 5

  //                             1----1    +    2----2
  //                             |....|    |    |....|
  //                             |....|    |    |....|
  //                             `....`    |    '....'
  //                              |....|   |   |....|
  //                              `.4------+------4.'
  //                               ||......|......||
  //                               ``......|......''
  //                                ||.....|.....||
  //                                ||.....|.....||
  //                      -------+---4+----+----+4---+-------
  //                                 |...| | |...|
  //                                 `...` | '...'
  //                                  |...|||...|
  //                                   |...|...|
  //                                   |.3-+-3.|
  //                                   `.|.|.|.'
  //                                    ||.|.||
  //                                    ||.|.||
  //                                    `|.|.|'
  //                                     3-2-3

  const int S[MAXPOLYS][POLYPOINTS][POLYCORDS] =
                                { {{-10,-10, 0},{ 10,-10, 0},{10, -7, 0},{-10, -7, 0}},
                                  {{-10, 10, 0},{ 10, 10, 0},{10,  7, 0},{-10,  7, 0}},
                                  {{-10,  1, 0},{ 10,  1, 0},{10, -2, 0},{-10, -2, 0}},
                                  {{-10, -8, 0},{ -7, -8, 0},{-7,  0, 0},{-10,  0, 0}},
                                  {{ 10,  8, 0},{  7,  8, 0},{ 7,  0, 0},{ 10,  0, 0}} };

  const int P[MAXPOLYS][POLYPOINTS][POLYCORDS] =
                                { {{-10,-10,0},{-7,-10,0},{-7,10,0},{-10,10,0}},
                                  {{10,-10,0},{7,-10,0},{7,0,0},{10,0,0}},
                                  {{-9,-10,0},{9,-10,0},{9,-7,0},{-9,-7,0}},
                                  {{-9,-1,0},{9,-1,0},{9,2,0},{-9,2,0}},
                                  {{0,0,0},{0,0,0},{0,0,0},{0,0,0}} };

  const int H[MAXPOLYS][POLYPOINTS][POLYCORDS] =
                                { {{-10,-10,0},{-7,-10,0},{-7,10,0},{-10,10,0}},
                                  {{10,-10,0},{7,-10,0},{7,10,0},{10,10,0}},
              {{-9,-1,0},{9,-1,0},{9,2,0},{-9,2,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}} };

  const int Y[MAXPOLYS][POLYPOINTS][POLYCORDS] =
            { {{-7,-10,0},{0,-3,0},{0,0,0},{-10,-7,0}},
              {{7,-10,0},{0,-3,0},{0,0,0},{10,-7,0}},
              {{-2,-3,0},{2,-3,0},{2,10,0},{-2,10,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}} };

  const int X[MAXPOLYS][POLYPOINTS][POLYCORDS] =
            { {{-7,-10,0},{10,7,0},{7,10,0},{-10,-7,0}},
              {{7,-10,0},{-10,7,0},{-7,10,0},{10,-7,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}} };

  const int I[MAXPOLYS][POLYPOINTS][POLYCORDS] =
            { {{-10,-10,0},{10,-10,0},{10,-7,0},{-10,-7,0}},
              {{-10,10,0},{10,10,0},{10,7,0},{-10,7,0}},
              {{-2,-9,0},{2,-9,0},{2,9,0},{-2,9,0}},
              {{0,0,0},{0,0,0},{0,0,0},{0,0,0}},
                                  {{0,0,0},{0,0,0},{0,0,0},{0,0,0}} };

//            //
// STRUCTURES //
//            //

  // The data on every point we rotate
  typedef struct {
         float    x;
         float    y;
         float    z;
  } Point;


//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  float Lookup[360][2];            // Our sin and cos lookup tables
  int Xoff, Yoff, Zoff;            // Used for movement of the object
  Point Lines[MAXPOLYS][4];        // The base object being rotated
  Point Translated[MAXPOLYS][4];   // The rotated object

  char screen_buffer [64000]; // screen is 320x200

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
// SetMCGA() - This function gets you into 320x200x256 mode.               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


void SetMCGA(void)
{
                  #pragma aux SetMCGA =   \
                  "mov ax, 0013h",        \
                  "int 10h"               \
                  modify [ax];
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetText() - This function gets you into text mode.                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetText(void)
{
                  #pragma aux SetText =   \
                  "mov ax, 0003h",        \
                  "int 10h"               \
                  modify [ax];
}




//?????????????????????????????????????
// This puts a pixel on the virtual screen by writing directly to memory.

void Putpixel2(unsigned int x, unsigned int y, unsigned char col)
{
        screen_buffer[y*320 + x] = col;
}

///////////////////////////////////////////////////////////
// This clears the virtual screen to the specified color

void Cls(unsigned char col)
{
        memset((char *) screen_buffer, col, 64000);
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This clears the virtual screen to the specified color

void Cls2(unsigned char col)
{
        memset((char *) screen_buffer, col, 64000);
}

////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// WaitRetrace() - This waits until you are in a Verticle Retrace.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

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




/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Pal() - This sets the Red, Green, and Blue values of a certain color.   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal(unsigned char ColorNo, unsigned char R,unsigned char G,unsigned char B)
{
  // This sets the Red, Green and Blue values of a certain color 

   outp(0x3C8,ColorNo);
   outp(0x3C9,R);
   outp(0x3C9,G);
        outp(0x3C9,B);

}


////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Flip() - This copies the entire screen at "source" to destination.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////



void Flip(void)
{
   // This flips the virtual screen to the VGA screen.

  memcpy((char *) VGA,screen_buffer,64000);

}


/////////////////////////////////////////////////////////////////////////
// This draws a horizontal line from x1 to x2 on line y in color col   //
/////////////////////////////////////////////////////////////////////////
void Hline(int x1,int x2,int y,unsigned char col) {

        int i;
        for (i=x1; i&lt;x2; i++)
         screen_buffer[y * 320 + i] = col;

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// DrawPoly() - This draws a polygon with 4 points at x1,y1, x2,y2, x3,y3, //
//              x4,y4 in color Col at location Where                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void DrawPoly(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,
                                  byte Col) {

  int x, mny, mxy, mnx, mxx, yc;
  int mul1, div1, mul2, div2, mul3, div3, mul4, div4;

  // find the maximum y (mny) and minimum y (mny)
                                  mny = y1;
                                  mxy = y1;
  if (y2&lt;mny) mny = y2;
  if (y2&gt;mxy) mxy = y2;
  if (y3&lt;mny) mny = y3;
  if (y3&gt;mxy) mxy = y3;
  if (y4&lt;mny) mny = y4;
  if (y4&gt;mxy) mxy = y4;

  // if the mimimum or maximum is out of bounds, bring it back in
  if (mny&lt;  0) mny =   0;
  if (mxy&gt;199) mxy = 199;

  // verticle range checking
  if (mny&gt;199) return;
  if (mxy&lt;  0) return;

  // constants needed for intersection calculations
  mul1 = x1-x4;  div1 = y1-y4;
  mul2 = x2-x1;  div2 = y2-y1;
  mul3 = x3-x2;  div3 = y3-y2;
  mul4 = x4-x3;  div4 = y4-y3;

  for (yc=mny; yc&lt;mxy; yc++) {
         mnx = 320;
         mxx =  -1;

         if ((y4 &gt;= yc) || (y1 &gt;= yc))
                if ((y4 &lt;= yc) || (y1 &lt;= yc))
                  if (y4 != y1) {
                         x = ((yc-y4) * mul1 / div1) + x4;
                         if (x&lt;mnx) mnx = x;
                         if (x&gt;mxx) mxx = x;
                  }

         if ((y1 &gt;= yc) || (y2 &gt;= yc))
                if ((y1 &lt;= yc) || (y2 &lt;= yc))
                  if (y1 != y2) {
                         x = ((yc-y1) * mul2 / div2) + x1;
                         if (x&lt;mnx) mnx = x;
                         if (x&gt;mxx) mxx = x;
                  }

         if ((y2 &gt;= yc) || (y3 &gt;= yc))
                if ((y2 &lt;= yc) || (y3 &lt;= yc))
                  if (y2 != y3) {
                         x = ((yc-y2) * mul3 / div3) + x2;
                         if (x&lt;mnx) mnx = x;
                         if (x&gt;mxx) mxx = x;
                  }

         if ((y3 &gt;= yc) || (y4 &gt;= yc))
                if ((y3 &lt;= yc) || (y4 &lt;= yc))
                  if (y3 != y4) {
                         x = ((yc-y3) * mul4 / div4) + x3;
                         if (x&lt;mnx) mnx = x;
                         if (x&gt;mxx) mxx = x;
                  }

         // horizontal range checking
         if (mnx&lt;  0)  mnx =   0;
         if (mxx&gt;319)  mxx = 319;

         if (mnx&lt;=mxx)
                // draw the horizontal line
                Hline(mnx,mxx,yc,Col);

  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetUpPoints() - This creates the lookup table.                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpPoints() {

  int loop1;

  // generate the sin() and cos() tables
  for (loop1=0; loop1&lt;360; loop1++) {
         Lookup [loop1][0] = sin(rad(loop1));
         Lookup [loop1][1] = cos(rad(loop1));
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// RotatePoints() - This rotates object lines by X,Y and Z, then places    //
//                  the result in Translated[]                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void RotatePoints(int X, int Y, int Z) {

  int loop1, loop2;
  Point temp;

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++) {
    for (loop2=0; loop2&lt;4; loop2++) {
      temp.x = Lines[loop1][loop2].x;
      temp.y = Lookup[X][1] * Lines[loop1][loop2].y - Lookup[X][0] * Lines[loop1][loop2].z;
      temp.z = Lookup[X][0] * Lines[loop1][loop2].y + Lookup[X][1] * Lines[loop1][loop2].z;
      Translated[loop1][loop2] = temp;

                if (Y&gt;0) {
        temp.x = Lookup[Y][1] * Translated[loop1][loop2].x - Lookup[Y][0] * Translated[loop1][loop2].y;
        temp.y = Lookup[Y][0] * Translated[loop1][loop2].x + Lookup[Y][1] * Translated[loop1][loop2].y;
        temp.z = Translated[loop1][loop2].z;
        Translated[loop1][loop2] = temp;
      }

      if (Z&gt;0) {
        temp.x = Lookup[Z][1] * Translated[loop1][loop2].x + Lookup[Z][0] * Translated[loop1][loop2].z;
        temp.y = Translated[loop1][loop2].y;
        temp.z = Lookup[Z][0] * Translated[loop1][loop2].x + Lookup[Z][1] * Translated[loop1][loop2].z;
        Translated[loop1][loop2] = temp;
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// DrawPoints() - This draws the translated object to the virtual screen.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void DrawPoints() {

  int nx, ny, nx2, ny2, nx3, ny3, nx4, ny4, temp, loop1;

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++) {
    if ((Translated[loop1][0].z+Zoff&lt;0) &amp;&amp; (Translated[loop1][1].z+Zoff&lt;0) &amp;&amp;
        (Translated[loop1][2].z+Zoff&lt;0) &amp;&amp; (Translated[loop1][4].z+Zoff&lt;0)) {

      temp = Translated[loop1][0].z + Zoff;
      nx   = ((256*Translated[loop1][0].x) / temp) + Xoff;
      ny   = ((256*Translated[loop1][0].y) / temp) + Yoff;

      temp = Translated[loop1][1].z + Zoff;
      nx2  = ((256*Translated[loop1][1].x) / temp) + Xoff;
      ny2  = ((256*Translated[loop1][1].y) / temp) + Yoff;

                temp = Translated[loop1][2].z + Zoff;
      nx3  = ((256*Translated[loop1][2].x) / temp) + Xoff;
      ny3  = ((256*Translated[loop1][2].y) / temp) + Yoff;

      temp = Translated[loop1][3].z + Zoff;
      nx4  = ((256*Translated[loop1][3].x) / temp) + Xoff;
      ny4  = ((256*Translated[loop1][3].y) / temp) + Yoff;

                DrawPoly(nx,ny,nx2,ny2,nx3,ny3,nx4,ny4,13);
         }
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Whizz() - This function moves the letters from one side of the screen   //
//           to the other and also zooms them closer as they move.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Whizz(int sub, int *deg) {

  int loop1;

  for (loop1=(-64); loop1&lt;(-4); loop1++) {

         Zoff = (loop1 * 8) - 15;
         if (sub == 1) Xoff -= 7; else Xoff += 7;
         RotatePoints(*deg,*deg,*deg);
         DrawPoints();
         WaitRetrace();
         Flip();
         Cls2(0);
         *deg = (*deg + 5) % 360;
  }
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// MoveAround() - This is the main display function.  First it brings the  //
//                object towards the viewer by increasing the Zoff, then   //
//                it passes control to the user.                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void MoveAround(void) {

  int deg=0, loop1, loop2;
  byte ch=1; // assign a dummy value to ch

  Yoff = 100;
  Xoff = 350;
  Cls2(0);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = A[loop1][loop2][0];
                Lines[loop1][loop2].y = A[loop1][loop2][1];
                Lines[loop1][loop2].z = A[loop1][loop2][2];
         }
  Whizz(1,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = S[loop1][loop2][0];
                Lines[loop1][loop2].y = S[loop1][loop2][1];
                Lines[loop1][loop2].z = S[loop1][loop2][2];
         }
  Whizz(0,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = P[loop1][loop2][0];
                Lines[loop1][loop2].y = P[loop1][loop2][1];
                Lines[loop1][loop2].z = P[loop1][loop2][2];
         }
  Whizz(1,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = H[loop1][loop2][0];
                Lines[loop1][loop2].y = H[loop1][loop2][1];
                Lines[loop1][loop2].z = H[loop1][loop2][2];
         }
  Whizz(0,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = Y[loop1][loop2][0];
                Lines[loop1][loop2].y = Y[loop1][loop2][1];
                Lines[loop1][loop2].z = Y[loop1][loop2][2];
         }
  Whizz(1,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = X[loop1][loop2][0];
                Lines[loop1][loop2].y = X[loop1][loop2][1];
                Lines[loop1][loop2].z = X[loop1][loop2][2];
         }
  Whizz(0,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = I[loop1][loop2][0];
                Lines[loop1][loop2].y = I[loop1][loop2][1];
                Lines[loop1][loop2].z = I[loop1][loop2][2];
         }
  Whizz(1,&amp;deg);

  for (loop1=0; loop1&lt;MAXPOLYS; loop1++)
         for (loop2=0; loop2&lt;POLYPOINTS; loop2++) {
                Lines[loop1][loop2].x = A[loop1][loop2][0];
                Lines[loop1][loop2].y = A[loop1][loop2][1];
                Lines[loop1][loop2].z = A[loop1][loop2][2];
         }
  Whizz(0,&amp;deg);

  Cls2(0);
  Cls(0);

  Xoff = 160;

  do {
         if (kbhit()) {
                ch = getch();
                switch (ch) {
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
         Cls2(0);
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
         "Hello there! Varsity has begun once again, so it is once again\n"
         "back to the grindstone ;-) ... anyway, this tutorial is, by\n"
         "popular demand, on poly-filling, in relation to 3-D solids.\n\n"
         "In this program, the letters of ASPHYXIA will fly past you. As you\n"
         "will see, they are solid, not wireframe. After the last letter has\n"
         "flown by, a large A will be left in the middle of the screen.\n\n"
         "You will be able to move it around the screen, and you will notice\n"
         "that it may have bits only half on the screen, i.e. clipping is\n"
         "perfomed. To control it use the following : ""A"" and ""Z"" control the Z\n"
         "movement, "","" and ""."" control the X movement, and ""S"" and ""X""\n"
         "control the Y movement. I have not included rotation control, but\n"
         "it should be easy enough to put in yourself ... if you have any\n"
         "hassles, leave me mail.\n\n");
  printf ("Hit any key to continue ...\n");
  getch();
  SetMCGA();

  SetUpPoints();

  MoveAround();
  SetText();

  printf(
         "All done. This concludes the ninth sample program in the ASPHYXIA\n"
         "Training series. You may reach DENTHOR under the names of GRANT\n"
         "SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid\n"
         "Connectix BBS user, and occasionally read RSAProg.\n"
         "The numbers are available in the main text. You may also write to me at:\n"
         "             Grant Smith\n"
         "             P.O. Box 270\n"
         "             Kloof\n"
         "             3640\n"
         "I hope to hear from you soon!\n\n");
  printf ("Hit any key to exit ...\n");
  getch();

  return (0);
}
