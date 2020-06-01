/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// TUTPROG8.CPP - VGA Trainer Program 8 (in Turbo C++ 3.0)                 //
//                                                                         //
// "The VGA Trainer Program" is written by Denthor of Asphyxia. However it //
// was limited to only Pascal in its first run.  All I have done is taken  //
// his original release, translated it to C++ and touched up a few things. //
// I take absolutely no credit for the concepts presented in this code and //
// am NOT the person to ask for help if you are having trouble.            //
//                                                                         //
// Program Notes : This program presents the basics of 3D.  Please note    //
//                 that the compiled C++ version of this program runs      //
//                 much faster than the Pascal version.  If you are        //
//                 a 486DX/33 or higher, you may wish to turn turbo off.   //
//                                                                         //
//                 If you are compiling this program from within the       //
//                 Turbo C++ environment, you must go under Options,       //
//                 Debugger, and change the "Program Heap Size" to a value //
//                 80 or greater.  If you are going to be fooling around   //
//                 with the code a bit, I suggest raising this to about    //
//                 100 just to be on the safe side.  You don't have to     //
//                 worry about this if you are compiling command line.     //
//                                                                         //
//                 Just for reference, this is what I use:                 //
//                                                                         //
//                    tcc -mc -a -G -2 -O tut8.cpp                         //
//                                                                         //
//                 The way things are set up, there is no need to compile  //
//                 or link tut8.cpp and gfx1.cpp seperately.               //
//                                                                         //
//                 The Compact memory model (-mc) seems to provide the     //
//                 best results for this tutorial.  Remember, use this     //
//                 memory model when you have little code (less than 64k)  //
//                 and lots of data.                                       //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : January 14, 1995                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//               //
// INCLUDE FILES //
//               //

  #include &lt;alloc.h&gt;
                           // farcalloc()
  #include &lt;conio.h&gt;
                           // clrscr(), getch(), kbhit()
  #include &lt;dos.h&gt;
                           // FP_SEG, geninterrupt()
  #include &lt;iostream.h&gt;
                           // cout
  #include &lt;math.h&gt;
                           // sin(), cos()
  #include &lt;stdlib.h&gt;
                           // exit()
  #include "gfx1.cpp"

//          //
// TYPEDEFS //
//          //

  typedef unsigned char byte;
  typedef unsigned int  word;

//           //
// CONSTANTS //
//           //

  const MAXLINES = 12;     // the number of lines in our cube

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

//                     //
// FUNCTION PROTOTYPES //
//                     //

  // MEMORY ALLOCATION FUNCTIONS
  void SetUpVirtual ();
  void ShutDown     ();

  // LOGO-FUNCTION
  void DrawLogo     ();

  // 3D POINTS FUNCTIONS
  void SetUpPoints  ();
  void RotatePoints (int X, int Y, int Z);
  void DrawPoints   ();
  void ClearPoints  ();

  // MID-LEVEL FUNCTION
  void MoveAround   ();

//            //
// STRUCTURES //
//            //

  // The data on every point we rotate
  struct Point {
    float    x;
    float    y;
    float    z;
  };

//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  byte far *Virscr=NULL;           // Pointer to our virtual screen
  word Vaddr;                      // Segment of our virtual screen
  float Lookup[360][2];            // Our sin and cos lookup table
  int Xoff, Yoff, Zoff;            // Used for movement of the objects
  Point Lines[MAXLINES][2];        // The base object rotated
  Point Translated[MAXLINES][2];   // The rotated object


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void main() {

  SetUpVirtual();
  clrscr();
  cout
    &lt;&lt; "Greetings and salutations! Hope you had a great Christmas and New\n"
    &lt;&lt; "year! ;-) ... Anyway, this tutorial is on 3-D, so this is what is\n"
    &lt;&lt; "going to happen ... a wireframe square will come towards you.\n"
    &lt;&lt; "When it gets close, you get control. ""A"" and ""Z"" control the Z\n"
    &lt;&lt; "movement, "","" and ""."" control the X movement, and ""S"" and ""X""\n"
    &lt;&lt; "control the Y movement. I have not included rotation control, but\n"
    &lt;&lt; "it should be easy enough to put in yourself ... if you have any\n"
    &lt;&lt; "hassles, leave me mail.\n\n";
  cout
    &lt;&lt; "Read the main text file for ideas on improving this code ... and\n"
    &lt;&lt; "welcome to the world of 3-D!\n\n";
  cout &lt;&lt; "Hit any key to contine ...\n";
  getch();

  SetMCGA();

  SetUpPoints();
  MoveAround();

  ShutDown();
  SetText();

  cout
    &lt;&lt; "All done. This concludes the eigth sample program in the ASPHYXIA\n"
    &lt;&lt; "Training series. You may reach DENTHOR under the names of GRANT\n"
    &lt;&lt; "SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid\n"
    &lt;&lt; "Connectix BBS user, and occasionally read RSAProg.\n"
    &lt;&lt; "For discussion purposes, I am also the moderator of the Programming\n"
    &lt;&lt; "newsgroup on the For Your Eyes Only BBS.\n"
    &lt;&lt; "The numbers are available in the main text. You may also write to me at:\n"
    &lt;&lt; "             Grant Smith\n"
    &lt;&lt; "             P.O. Box 270\n"
    &lt;&lt; "             Kloof\n"
    &lt;&lt; "             3640\n"
    &lt;&lt; "I hope to hear from you soon!\n\n\n";
  cout &lt;&lt; "Hit any key to exit ...\n";
  getch();

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetUpVirtual() - This sets up the memory needed for the virtual screen. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpVirtual() {

  Virscr = (byte far *) farcalloc(64000,1);

  // always check to see if enough memory was allocated
  if (Virscr == NULL) {
    SetText();
    cout &lt;&lt; "Insufficient memory for virtual screens, exiting...";
    exit(1);
  }

  Vaddr = FP_SEG(Virscr);

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// ShutDown() - This frees the memory used by the virtual screen.          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void ShutDown() {
  free(Virscr);
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

  const char *Logo[5] = { {"OOO OOO OOO O O O  O O  OOO OOO"},
                          {"O   O O O O O O O O O O O O O O"},
                          {"OOO O O O O O O O O O O OOO O O"},
                          {"  O O O O O O O O O O O O O O O"},
                          {"OOO O O OOO  O O  O O O O O O O"}
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
          Putpixel ((loop2+1)*10+loop3, (loop1+1)*4+loop4,
                    ball[loop3][loop4],Vaddr);

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
      temp.x = Lookup[Z][1]*Translated[loop1][0].x + Lookup[Z][0]*Translated[loop1][0].z;
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
        Line(nx,ny,nx2,ny2,13,Vaddr);
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
        Line(nx,ny,nx2,ny2,0,Vaddr);
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

  Cls(0,Vaddr);

  DrawLogo();

  for (loop1=(-256); loop1&lt;(-39); loop1++) {
    Zoff = loop1 * 2;
    RotatePoints(deg,deg,deg);
    DrawPoints();
    Flip(Vaddr,VGA);
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
    Flip(Vaddr,VGA);
    ClearPoints();
    RotatePoints(deg,deg,deg);
    deg = (deg + 5) % 360;

    // if the key pressed above was 0 (i.e. a control character) then
    // read the character code
    if (ch == 0) ch = getch();

  } while (ch != 27); // if the escape code was 27 (escape key) then exit
}
