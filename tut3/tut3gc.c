#include &lt;stdio.h&gt;
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
    _farpokeb(_dos_ds, 0xa0000 + x + y * 320, col);

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
void Circle (int X,int Y,int rad,unsigned char Col)
{
  // This draws a circle with centre X,Y, with Rad as it's radius
float deg;

  deg=0;

  do
  {
    X=round( rad * cos(deg) );
    Y=round( rad * sin(deg) );
    putpixel (X+160,Y+100,Col);
    deg=deg+0.005;
  } while (!(deg&gt;6.4));
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
void Line2 (int x1,int y1,int x2,int y2, unsigned char col)
{
  // This draws a line from x1,y1 to x2,y2 using the first method
int x,y,dx,dy;
float xlength, ylength, xslope,yslope;

  xlength=ABS(x1-x2);
  if ( (x1-x2)&lt;0 )  dx=-1;
  if ( (x1-x2)==0 ) dx= 0;
  if ( (x1-x2)&gt;0 )  dx=+1;
  ylength=ABS(y1-y2);
  if ( (y1-y2)&lt;0 )  dy=-1;
  if ( (y1-y2)==0 ) dy= 0;
  if ( (y1-y2)&gt;0 )  dy=+1;
  if (dy==0)
  {
    if (dx&lt;0)
       for (x=x1;x&lt;x2;x++)
           putpixel (x,y1,col);
           
    if (dx&gt;0)
       for (x=x2;x&lt;x1;x++)
           putpixel (x,y1,col);
    return;
  }
  if (dx==0)
  {
    if (dy&lt;0)
       for (y=y1;y&lt;y2;y++)
           putpixel (x1,y,col);
           
    if (dy&gt;0)
       for (y=y2;y&lt;y1;y++)
           putpixel (x1,y,col);

    return;
  }

  xslope=xlength / ylength;
  yslope=ylength / xlength;

  if (   ( (yslope/xslope) &lt; 1 ) &amp;&amp; ( (yslope/xslope) &gt; -1 )   )
  {
    if (dx&lt;0)
       for (x=x1;x&lt;x2;x++)
       {
            y= round (yslope*x);
            putpixel (x,y,col);
       };
       
    if (dx&gt;0)
       for (x=x2;x&lt;x1;x++)
       {
            y= round (yslope*x);
            putpixel (x,y,col);
       };
  }
  else
  {

    if (dy&lt;0)
       for (y=y1;y&lt;y2;y++)
       {
            x= round (xslope*y);
            putpixel (x,y,col);
       };
    if (dy&gt;0)
       for (y=y2;y&lt;y1;y++)
       {
            x= round (xslope*y);
            putpixel (x,y,col);
       };
  };
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
void line(int a,int b,int c,int d,int col)
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

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void PalPlay(void)

{
  // This procedure mucks about with our "virtual pallette", then shoves it
  //  to screen. 
   unsigned char Tmp[3]; // This is used as a "temporary color" in our pallette
   int loop1;

   memcpy(Tmp,Pall[198],3); // This copies color 200 from our virtual pallette to the Tmp variable

   memcpy(Pall[2],Pall[1],198*3); // This moves the entire virtual pallette up one color

   memcpy(Pall[1],Tmp,3); // This copies the Tmp variable to the bottom of the virtual pallette

   WaitRetrace();

   for(loop1=1;loop1&lt;199;loop1++)
     Pal (loop1,Pall[loop1][0],Pall[loop1][1],Pall[loop1][2]);

}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// BEGIN Of the main program

int main (void)

{



int loop1;

  printf ("This sample program will test out our line and circle algorithms.\n");
  printf ("In the first part, many circles will be draw creating (hopefully)\n");
  printf ("a \"tunnel\" effect. I will the rotate the pallete to make it look\n");
  printf ("nice. I will then draw some lines and rotate the pallette on them\n");
  printf ("too. Note : I am using the slower (first) line algorithm (in\n");
  printf ("procedure line2). Change it to Procedure Line and it will be using\n");
  printf ("the second line routine. NB : For descriptions on how pallette works\n");
  printf ("have a look at part two of this series; I won''t re-explain it here.\n");
  printf ("\n");
  printf ("Remember to send me any work you have done, I am most eager to help.\n");
  printf ("\n\n");
  printf ("Hit any key to continue ...\n");
  Readkey();
  SetMCGA();

  for (loop1=1;loop1&lt;199;loop1++)
  {
    Pall[loop1][0] = loop1 % 30+33;
    Pall[loop1][1] = 0;
    Pall[loop1][2] = 0;
  }
       // This sets colors 1 to 199 to values between 33 to 63. The %
       // operator gives you the remainder of a division, ie. 105 % 10 = 5

   WaitRetrace();
   for (loop1=1;loop1&lt;199;loop1++)
       Pal (loop1,Pall[loop1][0],Pall[loop1][1],Pall[loop1][2]);
        // This sets the true pallette to variable Pall

   for (loop1=1;loop1&lt;90;loop1++)
       Circle (160,100,loop1,loop1);
       // This draws 90 circles all with centres at 160,100; with increasing
       //  radii and colors.

  while(!kbhit())
  {
    PalPlay();
  }
  Readkey();

  for (loop1=1;loop1&lt;199;loop1++)
      Line2 (0,1,319,loop1,loop1); // *** Replace Line2 with Line to use the
                                   //      second line algorithm ***
       // This draws 199 lines, all starting at 0,1 }

  while (!kbhit())
  {
      PalPlay();
  }

  Readkey();
  SetText();
  
  printf ("All done. Okay, so maybe it wasn''t a tunnel effect, but you get the\n");
  printf ("general idea ;-) This concludes the third sample program in the ASPHYXIA\n");
  printf ("Training series. You may reach DENTHOR under the name of GRANT SMITH\n");
  printf ("on the MailBox BBS, or leave a message to ASPHYXIA on the ASPHYXIA BBS.\n");
  printf ("Get the numbers from Roblist, or write to :\n");
  printf ("             Grant Smith\n");
  printf ("             P.O. Box 270\n");
  printf ("             Kloof\n");
  printf ("             3640\n");
  printf ("I hope to hear from you soon!\n");
  printf ("\n\n");
  printf ("Hit any key to exit ...\n");

  Readkey();


  return(0);

} // END Of the main program
