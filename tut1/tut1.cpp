/*****************************************************************************/
/*                                                                           */
/* TUTPROG1.CPP - VGA Trainer Program 1 (in Turbo C++ 3.0)                   */
/*                                                                           */
/* "The VGA Trainer Program" is written by Denthor of Asphyxia.  However it  */
/* was limited to Pascal only in its first run.  All I have done is taken    */
/* his original release, translated it to C++, and touched up a few things.  */
/* I take absolutely no credit for the concepts presented in this code, and  */
/* am NOT the person to ask for help if you are having trouble.              */
/*                                                                           */
/* Program Notes : This program presents some basic video concepts: kicking  */
/*                 the computer into graphics mode, testing out two differ-  */
/*                 ent methods of putting pixels to the screen, and finally  */
/*                 re-entering text mode.                                    */
/*                                                                           */
/*                 If you are compiling this code command line, be sure to   */
/*                 use the "-ml" parameter (large memory model).  Otherwise, */
/*                 the program will compile and link, but will lock your     */
/*                 system upon execution.                                    */
/*                                                                           */
/* Author        : Grant Smith (Denthor)      - smith9@batis.bis.und.ac.za   */
/* Translated by : Christopher (Snowman) Mann - r3cgm@dax.cc.uakron.edu      */
/*                                                                           */
/* Last Modified : October 21st, 1994                                        */
/*                                                                           */
/*****************************************************************************/

#include &lt;conio.h&gt;    // getch(), clrscr()
#include &lt;dos.h&gt;      // MK_FP, geninterrupt()
#include &lt;mem.h&gt;      // memset()
#include &lt;stdio.h&gt;    // printf()
#include &lt;stdlib.h&gt;   // rand()

void SetMCGA();
void SetText();
void Cls(unsigned char Col);
void TestINTPutpixel();
void TestMEMPutpixel();
void INTPutpixel(int x, int y, unsigned char Col);
void MEMPutpixel(int x, int y, unsigned char Col);

// declare a pointer to the offset of VGA memory
unsigned char *vga = (unsigned char *) MK_FP(0xA000, 0);

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// SetMCGA() - This function gets you into 320x200x256 mode.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void SetMCGA() {
  _AX = 0x0013;
  geninterrupt (0x10);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// SetText() - This function gets you into text mode.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void SetText() {
  _AX = 0x0003;
  geninterrupt (0x10);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Cls() - This clears the screen to the specified color.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Cls(unsigned char Col) {
  memset(vga, Col, 0xffff);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// INTPutpixel() - This puts a pixel on the screen using inturrupts.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void INTPutpixel(int x, int y, unsigned char Col) {
  _AH = 0x0C;
  _AL = Col;
  _CX = x;
  _DX = y;
  _BX = 0x01;
  geninterrupt (0x10);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// TestINTPutpixel() - This tests out the speed of the INTPutpixel function.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void TestINTPutpixel() {

  int loop1,loop2;

  for (loop1=0;loop1&lt;320;loop1++) {
    for (loop2=0;loop2&lt;200;loop2++) {
      INTPutpixel (loop1,loop2,rand());
    }
  }
  getch();
  Cls(0);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// MEMPutpixel() - This puts a pixel on the screen by writing directly to
//                 memory.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void MEMPutpixel (int x, int y, unsigned char Col) {
  memset(vga+x+(y*320),Col,1);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// TestMEMPutpixel() - This tests out the speed of the MEMPutpixel function.
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void TestMEMPutpixel () {

  int loop1,loop2;

  for (loop1=0;loop1&lt;320;loop1++) {
    for (loop2=0;loop2&lt;200;loop2++) {
      MEMPutpixel (loop1,loop2,rand());
    }
  }
  getch();
  Cls(0);
}

//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//                                MAIN FUNCTION
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void main() {
  clrscr();
  printf ("What will happen is that I will clear the screen twice.  After\n");
  printf ("each clear screen you will have to hit a key.  I will then fill\n");
  printf ("the screen twice with randomly colored pixels using 2 different\n");
  printf ("methods, after each of which you will have to hit a key. I will\n");
  printf ("then return you to text mode.\n\n");
  printf ("Hit any key to continue ...\n");
  getch();

  SetMCGA();
  Cls(32);
  getch();
  Cls(90);
  getch();
  TestINTPutpixel();
  TestMEMPutpixel();
  SetText();

  printf ("All done. This concludes the 1st sample program in the ASPHYXIA\n");
  printf ("Training series. You may reach DENTHOR under the name of GRANT\n");
  printf ("SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n");
  printf ("ASPHYXIA BBS. Get the numbers from Roblist, or write to :\n");
  printf ("             Grant Smith\n");
  printf ("             P.O. Box 270\n");
  printf ("             Kloof\n");
  printf ("             3640\n");
  printf ("I hope to hear from you soon!\n\n\n");
  printf ("Hit any key to exit ...");
  getch();
}
