#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
// #include &lt;sys/nearptr.h&gt; // for __djgpp_conventional_base
#include &lt;dpmi.h&gt; // for __dpmi_regs
#include &lt;string.h&gt; // for memset
#include &lt;sys/farptr.h&gt;
#include &lt;go32.h&gt; /* for _dos_ds */
#include &lt;sys/movedata.h&gt; // for dosmemput


// char *scrptr;
char screen_buffer [64000]; // screen is 320x200
__dpmi_regs regs;

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
// This puts a pixel on the screen using interrupts.

void INTPutpixel(unsigned int x,unsigned int y,unsigned char col)
{
  memset(&amp;regs, 0, sizeof regs);
   regs.h.ah=0x0C;
   regs.h.al=col;
   regs.x.bx=0x01;
   regs.x.cx=x;
   regs.x.dx=y;
   __dpmi_int(0x10, &amp;regs);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This puts a pixel on the screen by writing directly to memory.

void MEMPutpixel(unsigned int x, unsigned int y, unsigned char col)
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
// This tests out the speed of the MEMPutpixel procedure.
void TestMEMPutpixel(void)
{

  int loop1,loop2;

  for (loop1=0; loop1&lt;=319; loop1++)
  {
    for (loop2=0; loop2&lt;=199; loop2++)
    {
         MEMPutpixel (loop1,loop2,(double)rand () * (256) / RAND_MAX);
    }
  }
  Readkey();
  Cls (0);
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This tests out the speed of the INTPutpixel procedure.

void TestINTPutpixel(void)
{

  int loop1,loop2;

  for (loop1=0; loop1&lt;=319; loop1++)
  {
    for (loop2=0; loop2&lt;=199; loop2++)
    {
      INTPutpixel (loop1,loop2,(double)rand () * (256) / RAND_MAX);
    }
  }
  Readkey();
  Cls (0);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// BEGIN Of the main program

int main (void)

{
//   scrptr = (char*) (__djgpp_conventional_base + 0xa0000);
//   __djgpp_nearptr_enable();

  printf ("What will happen is that I will clear the screen twice. After\n");
  printf ("each clear screen you will have to hit a key. I will then fill\n");
  printf ("the screen twice with randomlly colored pixels using two different\n");
  printf ("methoods, after each of which you will have to hit a key. I will\n");
  printf ("then return you to text mode.\n");
  printf ("\n\n");
  printf ("Hit any kay to continue ...\n");
  Readkey();

  SetMCGA();
  Cls (32);
  Readkey();
  Cls (90);

  TestINTPutpixel();

  TestMEMPutpixel();

//  __djgpp_nearptr_disable();

  SetText();


  printf ("All done. This concludes the first sample program in the ASPHYXIA\n");
  printf ("Training series. You may reach DENTHOR under the name of GRANT\n");
  printf ("SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n");
  printf ("ASPHYXIA BBS. Get the numbers from Roblist, or write to :\n");
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
