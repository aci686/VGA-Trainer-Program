/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// TUTPROG5.CPP - VGA Trainer Program 5 (in Turbo C++ 3.0)                 //
//                                                                         //
// "The VGA Trainer Program" is written by Denthor of Asphyxia. However it //
// was limited to Pascal only in its first run.  All I have done is taken  //
// his original release, translated it to C++ and touched up a few things. //
// I take absolutely no credit for the concepts presented in this code and //
// am NOT the person to ask for help if you are having trouble.            //
//                                                                         //
// Program Notes : This program presents the basic demo scroller.          //
//                                                                         //
//                 If you are compiling this code command line, be sure to //
//                 use the "-ml" parameter (large memory model).           //
//                 Otherwise, the program will compile and link, but will  //
//                 lock up your system.                                    //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : December 27, 1994                                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

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

//           //
// CONSTANTS //
//           //

  #define XSize 16
  #define YSize 16

  #define VGA 0xa0000

  const NumLetters = 62;   // the number of characters in our font

//                     //
// FUNCTION PROTOTYPES //
//                     //

  // MODE SETTING FUNCTIONS
  void SetMCGA();
  void SetText();

  // UTILITY FUNCTIONS
  void Pal(unsigned char ColorNo, unsigned char R,
		unsigned char G,       unsigned char B);
  void LoadPal(char *FileName);
  void Putpixel(int x, int y, unsigned char col);
  void WaitRetrace();
  int  Exist(char *FileName);

  // MID-LEVEL FUNCTIONS
  void Setup();
  void ScrollMsg (char *Msg, int SizeMsg);

//                 //
// GLOBAL TYPEDEFs //
//                 //

  typedef unsigned char Letter[XSize][YSize];

//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  // pointer to our index of font data
  Letter *Font;

  __dpmi_regs regs;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int main() {

  clrscr();
  printf (
	 "This program will give you an example of a scrolly. If the file\n"
	 "SOFTROCK.FNT is in the current directory, this program will scroll\n"
	 "letters, otherwise it will only scroll bars. It also searches for\n"
	 "PALLETTE.COL, which it uses for it''s pallette. Both SOFTROCK.FNT\n"
	 "and PALLETTE.COL come with TEXTER5.ZIP, at a BBS near you.\n\n");
  printf (
	 "You will note that you can change what the scrolly says merely by\n"
	 "changing the string in the program.\n\n");

  Setup();

  do ScrollMsg ("ASPHYXIA RULZ!!!   ",19); while (!kbhit());
  getch();

  SetText();
  free(Font);

  printf (
	 "All done. This concludes the fifth sample program in the ASPHYXIA\n"
	 "Training series. You may reach DENTHOR under the name of GRANT\n"
	 "SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the\n"
	 "ASPHYXIA BBS. Get the numbers from Roblist, or write to :\n"
	 "             Grant Smith\n"
	 "             P.O. Box 270\n"
	 "             Kloof\n"
	 "             3640\n"
	 "I hope to hear from you soon!\n\n");
  printf (
	 "Hit any key to exit ...");
  getch();

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetMCGA() - This function gets you into 320x200x256 mode.               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// This procedure gets you into 320x200x256 mode.

void SetMCGA(void)
{
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x13;
  __dpmi_int(0x10, &amp;regs);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetText() - This function gets you into text mode.                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//  This procedure returns you to text mode.

void SetText(void)
{
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x03;
  __dpmi_int(0x10, &amp;regs);
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


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// LoadPal() - This loads the Pallette file and puts it on screen.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void LoadPal(char *FileName) {

  typedef unsigned char DACType[256][3]; // [256] colors, [3] types (R,G,B)
  DACType DAC;
  FILE *fp;
  int loop1;

  fp = fopen(FileName,"rb");
  fread(DAC,sizeof(DACType),1,fp);
  fclose(fp);

  for (loop1=0; loop1&lt;256; loop1++)
	 Pal(loop1, DAC[loop1][0], DAC[loop1][1], DAC[loop1][2]);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Putpixel() - This puts a pixel at X,Y using color Col, on VGA or the    //
//              Virtual Screen;                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// void Putpixel (int x, int y, unsigned char Col, unsigned char *Where) {
//  memset(Where+(x+(y*320)),Col,1);
// }
	void Putpixel (int x, int y, unsigned char col)
	{
	 _farpokeb(_dos_ds, 0xa0000 + x + y * 320, col);
	}

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
// Exist() - Checks to see if file exists or not.                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

int Exist (char *FileName) {

  FILE *fp;

  if ((fp = fopen(FileName,"rb")) != NULL) {
	 fclose(fp);
    return 1;
  }
  else
    return 0;

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Setup() - This loads the font and the pallette.                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Setup() {

  FILE *fp;
  int loop1, loop2, loop3;

  Font = (Letter *) calloc(NumLetters,sizeof(Letter));

    if (Font == NULL) { // always check to make sure you have enough memory!
      SetText();
      printf ("Not enough memory available, exiting program...");
      exit(1);
    }

  if (Exist("SOFTROCK.FNT\0")) { // don't forget the "\0" null terminator
	 fp = fopen("SOFTROCK.FNT","rb");
	 fread(Font,sizeof(Letter),NumLetters,fp);
	 printf ("SoftRock.FNT from TEXTER5 found in current directory. Using.\n");
	 fclose(fp);
  }
  else {
	 printf ("SoftRock.FNT from TEXTER5 not found in current directory.\n");
	 for     (loop1=0; loop1&lt;NumLetters; loop1++)
		for   (loop2=0; loop2&lt;XSize;      loop2++)
	for (loop3=0; loop3&lt;YSize;      loop3++)
	  Font[loop1][loop2][loop3] = loop2;
  }

  if (Exist("PALLETTE.COL\0")) // don't forget the \0 null terminator
	 printf ("Pallette.COL from TEXTER5 found in current directory. Using.\n");
  else
	 printf ("Pallette.COL from TEXTER5 not found in current directory.\n");

  printf ("\n\nHit any key to continue ...");
  getch();

  SetMCGA();

  if (Exist("PALLETTE.COL\0")) LoadPal("PALLETTE.COL\0");
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// ScrollMsg() - This scrolls the string in Msg across the screen.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void ScrollMsg (char *Msg, int SizeMsg) {

  short video = __dpmi_segment_to_descriptor(0xa000);

  int loop1, loop2, loop3;
  int Location = 100; // specify how far from the top of the screen we want
				// the scroller to appear

  // MAIN LOOP - do every letter in Msg
  for (loop1 = 0; loop1 &lt; SizeMsg; loop1++) {

	 // Do this loop &lt;n&gt; number of times where &lt;n&gt; = the width of a letter
	 for (loop2 = 0; loop2 &lt; XSize; loop2++) {

	 // There are two loops here.
	 //
	 // 1. Copy YSize rows to the left by 1.  In effect, this frees up a
	 //    blank column of vertical pixels on the right side of the screen.
	 // 2. Replace that new blank column with pixels from our Font[] table.

	 WaitRetrace();

	 for (loop3=Location; loop3&lt;Location+YSize; loop3++)
		// move each row to the left by 1
		movedata(video,1+(loop3*320),video,0+(loop3*320), 320);

	 for (loop3=Location; loop3&lt;Location+YSize; loop3++)
		// Replace the new column on the right with information from Font[].
		// In the following statement:
		//   (320,         - y location (the rightmost column)
		//   loop3,        - x location (the current row we are working with)
		//   Font[...      - Get information from our Font table.  All of this
		//                   complex indexing really just gives us the pallette
		//                   number we are putting to the screen at a given
		//                   location.
		//    [(Msg[loop1]-32)] - Access the correct letter from our Font
		//                        table.  The (Msg[loop1]) would give us the
		//                        ASCII value of the letter we want to display,
		//                        but we need to subtract 32 to coordinate the
		//                        ASCII value with our Font table.
		//    [loop2]           - the column index of the letter we are using
		//    [loop3-Location]  - the row    index of the letter we are using
		Putpixel (319,loop3,Font[(Msg[loop1]-32)][loop2][loop3-Location]);

	 } // we have now inserted a new letter on the right side of the screen

  // This next bit scrolls by one pixel after each letter so that there
  // are gaps between the letters

  WaitRetrace();

  for (loop3=Location; loop3&lt;Location+YSize; loop3++)
	 // move each row to the left by 1 (same as above)
	 movedata(video,1+(loop3*320),video,loop3*320, 320);

  for (loop3 = 100; loop3 &lt; 100 + YSize; loop3++)
	 // put a blank column of pixels
	 Putpixel(319,loop3,0);

  } // we have now scrolled the entire message once

}
