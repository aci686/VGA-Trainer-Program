/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// GFX2.CPP - VGA Trainer Program secondary module containing graphics     //
//            functions.  Note: This module does not follow a lot of good  //
//            programming practices.  It was built to be used with the     //
//            VGA tutorial series.  If you are planning on using this      //
//            module with a different source file, some modifications may  //
//            be necessary.                                                //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : January 21, 1995                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//               //
// INCLUDE FILES //
//               //

  #include &lt;alloc.h&gt;
                           // farcalloc(), farfree()
  #include &lt;dos.h&gt;
                           // geninterrupt(), FP_SEG

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

  // VIRTUAL SCREEN FUNCTIONS
  void  SetUpVirtual(byte far *&amp;Virscr, word &amp;Vaddr);
  void  ShutDown    (byte far *&amp;Virscr);
  void  Cls         (byte Col, word Where);
  void  Flip        (word source, word dest);

  // MODE SETTING FUNCTIONS
  void  SetMCGA     ();
  void  SetText     ();

  // PALLETTE CLASS (DATA OBJECT AND RELATED FUNCTIONS)
  class Pal {
    public:
      Pal();
      void PalSet   (byte Rset, byte Gset, byte Bset);
      void PalGet   (byte Col);
      void PalPut   (byte Col);
      void PalInc   ();
      void PalDec   ();
    private:
      byte R;   // 0-63
      byte G;   // 0-63
      byte B;   // 0-63
  };

  // MATH-LIKE FUNCTIONS
  float rad         (float theta);
  int   sgn         (int a);

  template&lt;class T&gt;
  T abso(T value) {  if (value &gt;= 0) return value;  else return -value; }

  // DRAWING FUNCTIONS
  void  Putpixel    (word X, word Y, byte Col, word Where);
  void  PutpixelVGA (word X, word Y, byte Col);
  void  Line        (int a, int b, int c, int  d, int col, word Where);
  void  Hline       (word X1, word X2, word Y, byte Col, word Where);


//-------------------------VIRTUAL SCREEN FUNCTIONS------------------------//

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetUpVirtual() - This sets up the memory needed for a virtual screen.   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpVirtual(byte far *&amp;Virscr, word &amp;Vaddr) {
  Virscr = (byte far *) farcalloc(64000,1);
  Vaddr = FP_SEG(Virscr);
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// ShutDown() - This frees the memory used by a virtual screen.            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void ShutDown(byte far *&amp;Virscr) {
  farfree(Virscr);
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Cls() - This clears the screen at Where to color Col.                   //
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
// Flip() - This copies 64000 bytes from "source" to "destination".        //
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
// Pal() - This constructor initializes all Pal variables (R, G, and B) to //
//         zero.  This ensures that all Pal objects start in a consistent  //
//         state.                                                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

Pal::Pal() { R = B = G = 0; }

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PalSet() - This sets the Red, Green, and Blue values of a given color.  //
//            Set invalid colors ( &gt;63 ) equal to 0.                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal::PalSet(byte Rset, byte Gset, byte Bset) {

   R = (Rset &lt; 64) ? Rset : 0;
   G = (Gset &lt; 64) ? Gset : 0;
   B = (Bset &lt; 64) ? Bset : 0;

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PalGet() - This reads the values of the Red, Green, and Blue values of  //
//            a certain color.                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal::PalGet(byte Col) {

  byte Rtemp, Gtemp, Btemp;

  asm {
    mov     dx, 0x03C7   // load DX with 3C7 (read pallette function)
    mov     al, [Col]    // move color to AL
    out     dx, al       // write DX to the VGA (tell VGA that we want to
                         //   work with the color in AL
    add     dx, 2        // load DX with 3C9 (read RGB colors)
    in      al, dx       // read Red   to AL
    mov     [Rtemp],al   // copy AL to rr
    in      al, dx       // read Green to AL
    mov     [Gtemp],al   // copy AL to gg
    in      al, dx       // read Blue  to AL
    mov     [Btemp],al   // copy AL to bb
  }

  R = Rtemp;
  G = Gtemp;
  B = Btemp;

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PalPut() - This sets the Red, Green, and Blue values of a color.        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal::PalPut (byte Col) {

  byte Rtemp = R, Gtemp = G, Btemp = B;

  asm {
    mov     dx, 0x3C8    // load DX with 3C8 (write pallette function)
    mov     al, [Col]    // move color to AL
    out     dx, al       // write DX to the VGA (tell VGA that we want to
                         //   work with the color in AL
    inc     dx           // load DX with 3C9 (write RGB colors)
    mov     al, [Rtemp]  // move Red   to AL
    out     dx, al       // write DX to VGA (tell VGA that we want to use
                         //   the Red value in AL
    mov     al, [Gtemp]  // move Green to AL
    out     dx, al       // write DX to VGA
    mov     al, [Btemp]  // move Blue  to AL
    out     dx, al       // write DX to VGA
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PalInc() - This increments the R, G, and B values of a given Pal        //
//            variable, keeping all values less than 64.                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal::PalInc() {

  // if (R &lt; 63) R = R + 1; else R = 63;
  R = (R &lt; 63) ? (R + 1) : 63;
  G = (G &lt; 63) ? (G + 1) : 63;
  B = (B &lt; 63) ? (B + 1) : 63;

}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PalDec() - This decrements the R, G, and B values of a given Pal        //
//            variable, keeping all values greater than or equal to zero.  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Pal::PalDec() {

  // if (R &gt; 0) R = R - 1; else R = 0;
  R = (R &gt; 0) ? (R - 1) : 0;
  G = (G &gt; 0) ? (G - 1) : 0;
  B = (B &gt; 0) ? (B - 1) : 0;

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
    mov     ax, [Where]  // move segment of Where to AX
    mov     es, ax       // ES = VGA
    mov     bx, [X]      // BX = X
    mov     dx, [Y]      // DX = Y
    mov     ah, dl       // AH = Y*256
    xor     al, al       // AX = Y*256
    shl     dx, 6        // DX = Y*64
    add     dx, ax       // DX = Y*320
    add     bx, dx       // BX = Y*320 + X
    mov     ah, [Col]    // move value of Col into AH
    mov     byte ptr es:[bx], ah  // move Col to the offset in memory (DI)
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// PutpixelVGA() - This puts a pixel on the screen by writing directly to  //
//                 VGA memory.                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void PutpixelVGA (word X, word Y, byte Col) {
  asm {
    mov     ax, 0xA000   // AX = VGA Segment
    mov     es, ax       // ES = VGA Segment
    mov     bx, [X]      // BX = X
    mov     dx, [Y]      // DX = Y
    mov     ah, dl       // AX = Y*256 (AL is already 0 from A000 address)
    shl     dx, 6        // DX = Y*64
    add     dx, ax       // DX = Y*320
    add     bx, dx       // BX = Y*320 + X
    mov     ah, [Col]    // move value of Col into AH
    mov     byte ptr es:[bx], ah  // move Col to the offset in memory (DI)
  }
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Line() - This draws a line from a,b to c,d of color col on screne Where //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Line(int a, int b, int c, int d, int col, word Where) {

  int i,u,s,v,d1x,d1y,d2x,d2y,m,n;

  u   = c-a;       // x2-x1
  v   = d-b;       // y2-y1
  d1x = sgn(u);    // d1x is the sign of u (x2-x1) (VALUE -1,0,1)
  d1y = sgn(v);    // d1y is the sign of v (y2-y1) (VALUE -1,0,1)
  d2x = sgn(u);    // d2x is the sign of u (x2-x1) (VALUE -1,0,1)
  d2y = 0;
  m   = abso(u);   // m is the distance between x1 and x2
  n   = abso(v);   // n is the distance between y1 and y2

  if (m&lt;=n) {      // if the x distance is greater than the y distance
    d2x = 0;
    d2y = sgn(v);  // d2y is the sign of v (x2-x1) (VALUE -1,0,1)
    m   = abso(v); // m is the distance between y1 and y2
    n   = abso(u); // n is the distance between x1 and x2
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

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Hline() - This draws a horizontal line from X1 to X2 on line Y in color //
//           Col at memory location Where.                                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Hline (word X1, word X2, word Y, byte Col, word Where) {
  asm {
    mov     ax, [Where]  // move segment of Where to AX
    mov     es, ax       // set ES to segment of Where
    mov     ax, [Y]      // set AX to Y
    mov     di, ax       // set DI to Y
    shl     ax, 8        // shift AX left 8 places (multiply Y by 256)
    shl     di, 6        // shift DI left 6 places (multiply Y by 64)
    add     di, ax       // add AX to DI (Y*64 + Y*256 = Y*320)
    add     di, [X1]     // add the X1 offset to DI
    mov     al, [Col]    // move Col to AL
    mov     ah, al       // move Col to AH (we want 2 copies for word moving)
    mov     cx, [X2]     // move X2 to CX
    sub     cx, [X1]     // move the change in X to CX
    shr     cx, 1        // divide change in X by 2 (for word moving)
    jnc     Start        // if we have an even number of moves, go to Start
    stosb                // otherwise, move one byte more
  }
  Start: asm {
    rep     stosw        // do it!
  }
}
