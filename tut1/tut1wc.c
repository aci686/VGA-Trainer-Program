#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt; // for the rand() function


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This procedure gets you into 320x200x256 mode.

void Readkey(void)
{
        #pragma aux Readkey =   \
        "mov ax, 0001h",        \
        "int 16h"               \
        modify [ax];
}


//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This procedure gets you into 320x200x256 mode.

void SetMCGA(void)
{
        #pragma aux SetMCGA =   \
        "mov ax, 0013h",        \
        "int 10h"               \
        modify [ax];
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//  This procedure returns you to text mode.

void SetText(void)
{
        #pragma aux SetText =   \
        "mov ax, 0003h",        \
        "int 10h"               \
        modify [ax];
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This puts a pixel on the screen using interrupts.

void INTPutpixel (unsigned int x, unsigned int y, unsigned char col)
{

        #pragma aux INTPutpixel =       \
        "mov ah, 0Ch",                  \
        "mov bx, [1]"                   \
        "int 10h",                      \
        parm [cx] [dx] [al]             \
        modify [ax bx cx dx];
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This puts a pixel on the screen by writing directly to memory.

void MEMPutpixel(unsigned int x, unsigned int y, unsigned char col)
{
        *((char*) 0xa0000 + (y*320) + x) = col;
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
      MEMPutpixel (loop1,loop2,(int) 256 * rand() / RAND_MAX );
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
      INTPutpixel (loop1,loop2,(int) 256 * rand() / RAND_MAX );
    }
  }
  Readkey();
  Cls (0);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// This clears the screen to the specified color

void Cls(unsigned char col)
{
        memset((char*) 0xa0000, col, 64000);
}

//컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// BEGIN Of the main program

int main (void)

{

//  clrscr();               // This clears the text Screen
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
  Readkey();
  TestINTPutpixel();
  TestMEMPutpixel();
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
