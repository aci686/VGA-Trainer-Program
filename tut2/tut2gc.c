#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
// #include &lt;sys/nearptr.h&gt; // for __djgpp_conventional_base
#include &lt;dpmi.h&gt; // for __dpmi_regs
#include &lt;string.h&gt; // for memset
#include &lt;sys/farptr.h&gt;
#include &lt;go32.h&gt; /* for _dos_ds */
#include &lt;sys/movedata.h&gt; // for dosmemput
#include &lt;math.h&gt; // for the floor() function

#define ABS(x) ( ((x) &gt;= 0) ? (x) : (x * -1) )
#define round(x) floor( (x) + 0.5)

// char *scrptr;
char screen_buffer [64000]; // screen is 320x200
__dpmi_regs regs;

unsigned char Pall[255][3],Pall2[255][3];
     // This declares the PALL variable. 0 to 255 signify the colors of the
     //  pallette, 1 to 3 signifies the Red, Green and Blue values. I am
     //  going to use this as a sort of "virtual pallette", and alter it
     //  as much as I want, then suddenly bang it to screen. Pall2 is used
     //  to "remember" the origional pallette so that we can restore it at
     //  the end of the program. 

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This procedure waits for a key.

void Readkey(void)
{
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x01;
  __dpmi_int(0x16, &amp;regs);

}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This procedure gets you into 320x200x256 mode.

void SetMCGA(void)
{
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x13;
  __dpmi_int(0x10, &amp;regs);
}
        
//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//  This procedure returns you to text mode.

void SetText(void)
{
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x03;
  __dpmi_int(0x10, &amp;regs);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This puts a pixel on the screen by writing directly to memory.
void putpixel(unsigned int x, unsigned int y, unsigned char col)
{
    //   *(scrptr + x + y * 320) = col;
    _farpokeb(_dos_ds, 0xa0000 + x + y * 320, col);

}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This clears the screen to the specified color

void Cls(unsigned char col)
{
        memset(&amp;screen_buffer, col, 64000);
        dosmemput(screen_buffer,64000,0xa0000);
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
void GetPal(unsigned char ColorNo, unsigned char *R,unsigned char *G,unsigned char *B)
{
  // This reads the values of the Red, Green and Blue values of a certain
  //  color and returns them to you. 


   outp(0x3C7,ColorNo);
   *R = inp(0x3C9);
   *G = inp(0x3C9); 
   *B = inp(0x3C9);
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
void Pal(unsigned char ColorNo, unsigned char R,unsigned char G,unsigned char B)
{
  // This sets the Red, Green and Blue values of a certain color 

   outp(0x3C8,ColorNo);
   outp(0x3C9,R);
   outp(0x3C9,G);
   outp(0x3C9,B);

}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
int sgn(float a)
   {

        if (a&gt;0) return (+1);
        if (a&lt;0) return (-1);
        if (a==0) return (0);
        return 0;

   }

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void Line(int a,int b,int c,int d,int col)
{

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
          putpixel(a,b,col);
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

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
void PalPlay(void)
{
  // This procedure mucks about with our "virtual pallette", then shoves it
  //  to screen. 
   unsigned char Tmp[3]; // This is used as a "temporary color" in our pallette
   int loop1;

   memcpy(Tmp,Pall[199],3); // This copies color 200 from our virtual pallette to the Tmp variable

   memcpy(Pall[1],Pall[0],200*3); // This moves the entire virtual pallette up one color

   memcpy(Pall[0],Tmp,3); // This copies the Tmp variable to the bottom of the virtual pallette

   WaitRetrace();

   for(loop1=1;loop1&lt;=255;loop1++)
     Pal (loop1,Pall[loop1][0],Pall[loop1][1],Pall[loop1][2]);

}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void SetUpScreen(void)
{
  // This gets our screen ready but setting up the pallette and drawing
  //  the lines.
   int Loop;

   memset(Pall,sizeof(Pall),0); // Clear the entire PALL variable to zero.
   for (Loop=0;Loop&lt;=200;Loop++)
      Pall[Loop][0] = Loop%64;

       // This sets colors 0 to 200 in the PALL variable to values between
       //  0 to 63. the MOD function gives you the remainder of a division,
       //  ie. 105 mod 10 = 5 

   for (Loop=1;Loop&lt;=320;Loop++)
   {
      Line(319,199,320-Loop,0,(Loop%199)+1);
      Line(0,0,Loop,199,(Loop%199)+1);
       // These two lines start drawing lines from the left and the right
       //  hand sides of the screen, using colors 1 to 199. Look at these
       //  two lines and understand them.

      PalPlay();
        // This calls the PalPlay procedure 
   }


}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void GrabPallette(void) 
{
int loop1;

  for (loop1=0;loop1&lt;=255;loop1++)
    GetPal (loop1,&amp;Pall2[loop1][0],&amp;Pall2[loop1][1],&amp;Pall2[loop1][2]);
}



//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void Blackout(void)
{
  // This procedure blackens the screen by setting the pallette values of
  // all the colors to zero. 
int loop1;

  WaitRetrace;
  for (loop1=0;loop1&lt;=255;loop1++)
    Pal (loop1,0,0,0);
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void HiddenScreenSetup(void) 
{
 // This procedure sets up the screen while it is blacked out, so that the
 // user can't see what is happening. 
int loop1,loop2;

  for (loop1=0;loop1&lt;=319;loop1++)
    for(loop2=0;loop2&lt;=199;loop2++)
      putpixel (loop1,loop2,(double)rand() * (256) / RAND_MAX);
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void FadeUp(void) {
  // This procedure slowly fades up the new screen
int loop1,loop2;
unsigned char Tmp[3];
      // This is temporary storage for the values of a color 

  for (loop1=1;loop1&lt;64;loop1++)
  {
      // A color value for Red, green or blue is 0 to 63, so this loop only
      // need be executed a maximum of 64 times 
    WaitRetrace();
    for(loop2=0;loop2&lt;255;loop2++)
    {
      GetPal (loop2,&amp;Tmp[0],&amp;Tmp[1],&amp;Tmp[2]);

      if (Tmp[0]&lt;Pall2[loop2][0]) Tmp[0]++;
      if (Tmp[1]&lt;Pall2[loop2][1]) Tmp[1]++;
      if (Tmp[2]&lt;Pall2[loop2][2]) Tmp[2]++;

        // If the Red, Green or Blue values of color loop2 are less then they
        //  should be, increase them by one. 
      Pal (loop2,Tmp[0],Tmp[1],Tmp[2]);
        // Set the new, altered pallette color.

    }
  }
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void FadeDown(void) {
  // This procedure fades the screen out to black. 
int loop1,loop2;
unsigned char Tmp[3];
      // This is temporary storage for the values of a color 

  for (loop1=1;loop1&lt;64;loop1++)
  {
    WaitRetrace();
    for (loop2=0;loop2&lt;255;loop2++)
    {
      GetPal (loop2,&amp;Tmp[0],&amp;Tmp[1],&amp;Tmp[2]);
      if (Tmp[0]&gt;0) Tmp[0]--;
      if (Tmp[1]&gt;0) Tmp[1]--;
      if (Tmp[2]&gt;0) Tmp[2]--;
        // If the Red, Green or Blue values of color loop2 are not yet zero,
        // then, decrease them by one. }
      Pal (loop2,Tmp[0],Tmp[1],Tmp[2]);
        // Set the new, altered pallette color. 
    }
  }
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void RestorePallette(void)
{
  // This procedure restores the origional pallette 
int loop1;

  WaitRetrace;
  for (loop1=0;loop1&lt;=255;loop1++)
    Pal (loop1,Pall2[loop1][0],Pall2[loop1][1],Pall2[loop1][2]);
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// BEGIN Of the main program

int main (void)

{
//   scrptr = (char*) (__djgpp_conventional_base + 0xa0000);
//   __djgpp_nearptr_enable();

  printf("This program will draw lines of different colors across the\n");
  printf("screen and change them only by changing their pallette values.\n");
  printf("The nice thing about using the pallette is that one pallette\n");
  printf("change changes the same color over the whole screen, without\n");
  printf("you having to redraw it. Because I am using a WaitRetrace\n");
  printf("command, turning on and off your turbo during the demonstration\n");
  printf("should have no effect.\n");
  printf("\n");
  printf("The second part of the demo blacks out the screen using the\n");
  printf("pallette, fades in the screen, waits for a keypress, then fades\n");
  printf("it out again. I haven''t put in any delays for the fadein/out,\n");
  printf("so you will have to put ''em in yourself to get it to the speed you\n");
  printf("like. Have fun and enjoy! ;-)\n");
  printf("\n\n");
  printf("Hit any key to continue ...\n");
  Readkey();
  SetMCGA();
  GrabPallette();

  SetUpScreen();
  Readkey();

  while(!kbhit())
     PalPlay();
       // Call the PalPlay procedure repeatedly until a key is pressed.

  Readkey();
    // Read in the key pressed otherwise it is left in the keyboard buffer



  Blackout();
  HiddenScreenSetup();

  FadeUp();

  Readkey();
  FadeDown();
  Readkey();
  RestorePallette();

//  __djgpp_nearptr_disable();
  SetText();
  printf("All done. This concludes the second sample program in the ASPHYXIA\n");
  printf("Training series. You may reach DENTHOR under the name of GRANT\n");
  printf("SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n");
  printf("ASPHYXIA BBS. Get the numbers from Roblist, or write to :\n");
  printf("             Grant Smith\n");
  printf("             P.O. Box 270\n");
  printf("             Kloof\n");
  printf("             3640\n");
  printf("I hope to hear from you soon!\n");
  printf("\n\n");
  printf("Hit any key to exit ...\n");
  Readkey();


  return(0);

} // END Of the main program
