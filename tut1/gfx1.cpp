/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// GFX1.CPP - VGA Trainer Program secondary module containing graphics     //
//            functions.  Note: This module does not follow a lot of good  //
//            programming practices.  It was built to be used with the     //
//            VGA tutorial series.  If you are planning on using this      //
//            module with a different source file, some modifications may  //
//            be necessary.                                                //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : January 13, 1995                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//               //
// INCLUDE FILES //
//               //

  #include &lt;dos.h&gt;
                           // geninterrupt()
  #include &lt;math.h&gt;
                           // abs()

//         //
// DEFINES //
//         //

  #if !defined(PI)
    #define PI 3.1415927
  #endif

  #if !defined(VGA)
    #define VGA 0xA000
  #endif

//          //
// TYPEDEFS //
//          //

  typedef unsigned char byte;
  typedef unsigned int  word;

//                     //
// FUNCTION PROTOTYPES //
//                     //

  // MODE SETTING FUNCTIONS
  void  SetMCGA     ();
  void  SetText     ();

  // PALLETTE FUNCTIONS
  void  Pal         (byte Col, byte  R, byte  G, byte  B);
  void  GetPal      (byte Col, byte &amp;R, byte &amp;G, byte &amp;B);

  // MATH-LIKE FUNCTIONS
  float rad         (float theta);
  int   sgn         (int a);

  // DRAWING FUNCTIONS
  void  Putpixel    (word X, word Y, byte Col, word Where);
  void  Line        (int a, int b, int c, int  d, int col, word Where);

  // VIDEO MEMORY FUNCTIONS
  void  Cls         (byte Col, word Where);
  void  Flip        (word source, word dest);


//--------------------------MODE SETTING FUNCTIONS-------------------------//

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


//----------------------------PALLETTE FUNCTIONS---------------------------//

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Pal() - This sets the Red, Green, and Blue values of a certain color.   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal (byte Col, byte  R, byte  G, byte  B) {
  asm {
    mov     dx, 0x3C8    // load DX with 3C8 (write pallette function)
    mov     al, [Col]    // move color to AL
    out     dx, al       // write DX to the VGA (tell VGA that we want to
                         //   work with the color in AL
    inc     dx           // load DX with 3C9 (write RGB colors)
    mov     al, [R]      // move Red   to AL
    out     dx, al       // write DX to VGA (tell VGA that we want to use
                         //   the Red value in AL
    mov     al, [G]      // move Green to AL
    out     dx, al       // write DX to VGA
    mov     al, [B]      // move Blue  to AL
    out     dx, al       // write DX to VGA
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// GetPal() - This reads the values of the Red, Green, and Blue values of  //
//            a certain color.  This function uses pass-by-reference.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void GetPal (byte Col, byte &amp;R, byte &amp;G, byte &amp;B) {

  byte rr,gg,bb;

  asm {
    mov     dx, 0x03C7   // load DX with 3C7 (read pallette function)
    mov     al, [Col]    // move color to AL
    out     dx, al       // write DX to the VGA (tell VGA that we want to
                         //   work with the color in AL
    add     dx, 2        // load DX with 3C9 (read RGB colors)
    in      al, dx       // read Red   to AL
    mov     [rr],al      // copy AL to rr
    in      al, dx       // read Green to AL
    mov     [gg],al      // copy AL to gg
    in      al, dx       // read Blue  to AL
    mov     [bb],al      // copy AL to bb
  }

  R = rr;
  G = gg;
  B = bb;

}


//----------------------------MATH-LIKE FUNCTIONS--------------------------//

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
// sgn() - This checks the sign of an integer and returns a 1, -1, or 0.   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

int sgn (int a) {

  if (a &gt; 0)  return +1;
  if (a &lt; 0)  return -1;
  return 0;
}


//-----------------------------DRAWING FUNCTIONS---------------------------//

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Putpixel() - This puts a pixel on the screen by writing directly to     //
//              memory.                                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Putpixel (word X, word Y, byte Col, word Where) {
  asm {
    push    ds           // save DS
    push    es           // save ES
    mov     ax, [Where]  // move segment of Where to AX
    mov     es, ax       // set ES to segment of Where
    mov     bx, [X]      // set BX to X
    mov     dx, [Y]      // set DX to Y
    push    bx           // save BX (our X value)
    mov     bx, dx       // now BX and DX are equal to Y
    mov     dh, dl       // copy DL to DH (multiply Y by 256)
    xor     dl, dl       // zero out DL
    shl     bx, 6        // shift BX left 6 places (multiply Y by 64).
    add     dx, bx       // add BX to DX (Y*64 + Y*256 = Y*320)
    pop     bx           // restore BX (X coordinate)
    add     bx, dx       // add BX to DX (Y*320 + X).  this gives you
                         //   the offset in memory you want
    mov     di, bx       // move the offset to DI
    xor     al, al       // zero out AL
    mov     ah, [Col]    // move value of Col into AH
    mov     es:[di], ah  // move Col to the offset in memory (DI)
    pop     es           // restore ES
    pop     ds           // restore DS
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Line() - This draws a line from a,b to c,d of color col on screne Where //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Line(int a, int b, int c, int d, int col, word Where) {

  int i,u,s,v,d1x,d1y,d2x,d2y,m,n;

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

  s = m / 2; // s is the m distance (either x or y) divided by 2

  for (i=0;i&lt;m+1;i++) { // repeat this loop until it
                 // is = to m (y or x distance)
    Putpixel(a,b,col,Where); // plot a pixel at the original x1, y1
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


//--------------------------VIDEO MEMORY FUNCTIONS-------------------------//

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Cls() - This clears the screen at location Where to color Col           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Cls(byte Col, word Where) {
  asm {
    push    es           // save ES
    mov     cx, 32000    // this is our loop counter.  we want to clear
                         //   64000 bytes of memory, so why do we use 32000?
                         //   1 word = 2 bytes, and we are moving a word at
                         //   a time
    mov     es, [Where]  // move address in Where to ES
    xor     di, di       // zero out DI
    mov     al, [Col]    // move color to AL
    mov     ah, al       // move color to AH (Remember, will be moving
                         //   a WORDS, so we need two copies
    rep     stosw        // copy AX to Where and drecrement CX by 1
                         //   until CX equals 0
    pop     es           // restore ES
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Flip() - This copies the entire screen at "source" to destination.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Flip(word source, word dest) {
  asm {
    push    ds           // save DS
    mov     ax, [dest]   // copy segment of destination to AX
    mov     es, ax       // set ES to point to destination
    mov     ax, [source] // copy segment of source to AX
    mov     ds, ax       // set DS to point to source
    xor     si, si       // zero out SI
    xor     di, di       // zero out DI
    mov     cx, 32000    // set our counter to 32000
    rep     movsw        // move source to destination by words.  decrement
                         //   CX by 1 each time until CX is 0
    pop     ds           // restore DS
  }
}
