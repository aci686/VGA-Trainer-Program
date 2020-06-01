/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// TUTPROG7.CPP - VGA Trainer Program 7 (in Turbo C++ 3.0)                 //
//                                                                         //
// "The VGA Trainer Program" is written by Denthor of Asphyxia. However it //
// was limited to only Pascal in its first run.  All I have done is taken  //
// his original release, translated it to C++ and touched up a few things. //
// I take absolutely no credit for the concepts presented in this code and //
// am NOT the person to ask for help if you are having trouble.            //
//                                                                         //
// Program Notes : This program demonstrates animation.  Several of the    //
//                 functions have been converted to assembler.  This       //
//                 tutorial is a whopper, so just take your time.          //
//                                                                         //
//                 If you are compiling this program from within the       //
//                 Turbo C++ environment, you must go under Options,       //
//                 Debugger, and change the "Program Heap Size" to a value //
//                 132 or greater.  If you are going to be fooling around  //
//                 with the code a bit, I suggest raising this to about    //
//                 150 just to be on the safe side.  You don't have to     //
//                 worry about this if you are compiling command line.     //
//                                                                         //
//                 Just for reference, this is what I use:                 //
//                                                                         //
//                    tcc tut7 -mc -a -G -2 -O                             //
//                                                                         //
//                 The Compact memory model (-mc) seems to provide the     //
//                 best results for this tutorial.  Remember, use this     //
//                 memory model when you have little code (less than 64k)  //
//                 and lots of data.                                       //
//                                                                         //
// Author        : Grant Smith (Denthor) - denthor@beastie.cs.und.ac.za    //
// Translator    : Christopher G. Mann   - r3cgm@dax.cc.uakron.edu         //
//                                                                         //
// Last Modified : January 6, 1995                                         //
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
                           // cout, memmove()
  #include &lt;stdlib.h&gt;
                           // abs(), exit(), randomize(), random()

//          //
// TYPEDEFS //
//          //

  typedef unsigned char byte;
  typedef unsigned int  word;

//              //
// TOASTER DATA //
//              //

const byte FRAME1[] = {
0,0,0,0,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,
7,7,7,7,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
5,7,7,7,7,7,7,7,8,8,7,7,7,7,7,7,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,
0,0,0,0,0,5,5,7,7,7,7,7,8,8,7,8,8,7,8,7,8,7,7,7,5,8,8,8,8,5,5,5,5,5,5,5,5,5,5,5,
5,0,0,0,0,0,0,0,0,0,0,0,5,7,7,7,7,7,7,8,7,7,7,8,7,7,7,7,7,7,0,0,0,0,0,0,8,5,5,5,
5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,5,7,7,8,8,7,7,8,7,7,8,7,7,7,7,7,0,0,0,0,0,
0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,5,7,8,8,8,7,7,8,7,7,8,7,7,7,
7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,5,7,8,8,8,7,7,
8,8,8,8,8,8,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,5,7,8,8,8,8,8,7,7,8,8,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,5,7,7,8,8,8,8,7,7,8,8,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
1,1,0,0,0,1,1,1,1,1,1,1,9,9,9,5,7,8,8,7,7,8,8,7,8,8,8,7,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,5,7,8,8,7,7,7,7,8,8,7,7,7,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,7,8,8,8,8,8,8,8,7,
7,7,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,7,
7,7,7,7,7,7,7,7,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,
4,6,6,6,6,6,6,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const byte FRAME2[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,0,0,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,5,
5,5,5,5,5,5,5,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,5,5,5,5,5,5,5,5,5,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,5,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,5,5,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,5,5,5,5,
5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,2,2,2,2,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,7,1,4,
4,6,6,6,6,6,6,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,
0,0,0,5,5,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,5,5,5,5,0,0,
0,0,0,0,0,0,0,0,0,0,0,5,5,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,9,9,9,9,9,9,9,9,9,9,5,5,
5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,
1,7,7,1,7,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,5,5,1,7,7,7,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,1,1,5,5,5,5,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,
5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,
0,0,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const byte FRAME3[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,7,1,1,1,1,1,
1,1,0,0,0,1,1,1,1,1,1,1,9,9,9,9,9,9,9,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0,
0,7,1,1,7,7,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,1,7,7,7,7,5,5,5,5,5,5,
5,0,0,0,0,0,0,0,7,1,7,7,7,1,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,5,5,1,1,1,7,7,
1,1,7,5,5,5,5,5,5,5,0,0,0,0,0,0,1,1,7,1,1,7,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,1,7,7,7,1,7,7,7,7,7,5,5,5,5,5,5,5,5,0,0,0,0,0,1,7,7,7,7,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,1,7,7,7,7,7,7,7,1,1,5,5,5,5,5,5,5,5,5,0,0,0,0,7,7,1,7,1,7,1,1,
1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,5,5,5,5,5,5,5,5,5,5,5,0,0,0,
7,7,7,7,7,1,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,5,5,5,5,5,5,
5,5,5,5,5,0,0,0,7,7,0,0,7,7,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,5,5,0,0,5,5,0,5,5,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,
1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,
4,6,6,6,6,6,6,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//           //
// CONSTANTS //
//           //

  const VGA = 0xa000;         // The address of our VGA screen
  const ICON = 30*48;         // This is the size of our pictures
  const NUMTOASTERS = 10;     // The maximum number of toasters we have
  const NUMCIRCLES = 200;     // The number of little circles on the screen
  const TRUE = 1;
  const FALSE = 0;

//                     //
// FUNCTION PROTOTYPES //
//                     //

  // GENERAL VIDEO TOOLS
  void SetMCGA     ();
  void SetText     ();
  void Cls         (byte Col, word Where);
  void Putpixel    (word X, word Y, byte Col, word Where);
  void WaitRetrace ();
  void Pal         (byte Col, byte  R, byte  G, byte  B);
  void GetPal      (byte Col, byte &amp;R, byte &amp;G, byte &amp;B);

  // MID-LEVEL FUNCTIONS
  void SetUpVirtual();
  void ShutDown    ();
  void Flip        (word source, word dest);
  void Putico      (word X, word Y, const byte *sprt, word Where);
  void Funny_line  (int a, int b, int c, int d, word where);
   int sgn         (int a);
  void SetUpScreen ();
  void Rotatepal   ();
  void ScreenTrans (word x, word y);
  void NewToaster  ();
  void Fly         ();

//            //
// STRUCTURES //
//        //

  struct Toastinfo {
    int      x;
    int      y;
    word     speed;
    word     frame;
    byte     active;
  };

//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //

  byte far *Virscr1=NULL, *Virscr2=NULL;  // Pointers to both virtual screens
  word Vaddr1, Vaddr2;                    // Segments of both virtual screens
  byte ourpal[256][3];                    // A virtual pallette
  Toastinfo toaster[NUMTOASTERS];         // The toaster info


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void main() {

  randomize();    // Make sure that the random() function really is random
  SetUpVirtual(); // Set up the virtual screens and get their segments

  clrscr();
  cout
    &lt;&lt; "Hello! This program will demonstrate the principals of animation.\n"
    &lt;&lt; "The program will firstly generate an arb background screen to a\n"
    &lt;&lt; "virtual page, then flip it to the VGA. A toaster will then start\n"
    &lt;&lt; "to move across the screen. Note that the background will be restored\n"
    &lt;&lt; "after the toaster has passed over it. You may add or remove toasters\n"
    &lt;&lt; "by hitting ""+"" or ""-"" respectively. Note that the more frames you\n"
    &lt;&lt; "use, usually the better the routine looks. Because of space\n"
    &lt;&lt; "restrictions, we only had room for three frames.\n\n";
  cout
    &lt;&lt; "The toasters were drawn by Fubar (Pieter Buys) in Autodesk Animator.\n"
    &lt;&lt; "I wrote a small little program to convert them into CONSTANTS. See\n"
    &lt;&lt; "the main text to find out how to load up AA CEL files directly.\n\n\n";
  cout &lt;&lt; "Hit any key to continue...";
  getch();

  SetMCGA();
  SetUpScreen();  // Draw background screen to Vaddr1, then flip to VGA
  Fly();          // Make the toasters fly around the screen.
  SetText();
  ShutDown();     // Free the memory taken up by virtual pages

  cout
    &lt;&lt; "All done. This concludes the seventh sample program in the ASPHYXIA\n"
    &lt;&lt; "Training series. You may reach DENTHOR under the names of GRANT\n"
    &lt;&lt; "SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid\n"
    &lt;&lt; "Connectix BBS user, which is unfortunatly offline for the moment.\n"
    &lt;&lt; "For discussion purposes, I am also the moderator of the Programming\n"
    &lt;&lt; "newsgroup on the For Your Eyes Only BBS.\n"
    &lt;&lt; "The numbers are available in the main text. You may also write to me at:\n"
    &lt;&lt; "             Grant Smith\n"
    &lt;&lt; "             P.O. Box 270\n"
    &lt;&lt; "             Kloof\n"
    &lt;&lt; "             3640\n"
    &lt;&lt; "I hope to hear from you soon!\n\n\n";
  cout &lt;&lt; "Hit any key to exit...";
  getch();
}


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
// WaitRetrace() - This waits until you are in a Verticle Retrace.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void WaitRetrace() {

  asm mov dx,0x03DA

  l1: asm {
    in      al,dx;
    and     al,0x08;
    jnz     l1;
  }
  l2: asm {
    in      al,dx;
    and     al,0x08;
    jz      l2;
  }

}


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


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetUpVirtual() - This sets up the memory needed for the virtual screen. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpVirtual() {

  Virscr1 = (byte far *) farcalloc(64000,1);
  Virscr2 = (byte far *) farcalloc(64000,1);

  // always check to see if enough memory was allocated
  if ((Virscr1 == NULL) || (Virscr2 == NULL)) {
    SetText();
    cout &lt;&lt; "Insufficient memory for virtual screens, exiting...";
    free(Virscr1);  // on the off chance that it allocated 1 but not 2,
            // deallocate the first.
    exit(1);
  }

  Vaddr1 = FP_SEG(Virscr1);
  Vaddr2 = FP_SEG(Virscr2);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// ShutDown() - This frees the memory used by the virtual screen.          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void ShutDown() {

//  free(Virscr1);
//  free(Virscr2);

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


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Putico() - This puts an icon, EXCEPT it's color 0 (black) pixels, onto  //
//            the screen "where", at position X,Y                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Putico(word X, word Y, const byte *sprt, word Where) {
  asm {
    push    ds           // save DS
    push    es           // save ES
    lds     si, [sprt]   // set SI to offset of sprite
    mov     ax, X        // set AX equal to X
    mov     bx, Y        // set BX equal to Y
  }
  _Redraw: asm {
    push    ax           // save AX (X coordinate)
    mov     ax, [Where]  // copy segment of Where to AX
    mov     es, ax       // set ES to segment of Where
    mov     ax, bx       // set AX to BX (X)
    mov     bh, bl       // copy BL to BH (multiply Y by 256)
    xor     bl, bl       // zero out BL
    shl     ax, 6        // multiply AX by 64 (Y * 64)
    add     bx, ax       // add AX to BX (Y*64 + Y*256 = Y*320)
    pop     ax           // restore BX (X coordinate)
    add     ax, bx       // add BX to AX (Y*320 + X).  this gives you
                         //   the offset in memory you want
    mov     di, ax       // move the offset to DI
    mov     dl, 30       // set DL to height of sprite
    xor     ch, ch       // zero out CH
    mov     cl, 48       // set CL to width of sprite
    cld                  // clear direction flag.  if you did not do this,
                         //   the loop would go backward instead of forward
    push    ax           // save AX (offset in memory where you draw)
    mov     ax, cx       // set AX to CX (width)
  }
  _DrawLoop: asm {
    push    di           // save DI (offset in memory where you draw)
    mov     cx, ax       // set CX to AX (width)
  }
  _LineLoop: asm {       // DRAW THE DATA
    mov     bl,byte ptr [si]
                         // load byte of data from sprite to BL
    or      bl, bl       // no effect, but triggers zero flag
    jnz     _Store       // if previous statement did not trigger zero flag
                         //   (BL was not color 0) then jump to _Store
                         //   (write it to the screen).  in the description
                         //   of this function, it said that we would not be
                         //   putting color 0 on the screen, so the next
  }                      //   section skips over the pixel if it was 0.

  _NoPaint: asm {        // SKIP OVER COLOR 0
    inc     si           // increment SI (skip to next pixel)
    inc     di           // increment DI (skip to next pixel)
    loop    _LineLoop    // decrement CX and jump to _LineLoop if CX is
                         //   not 0 (end of line)
    jmp     _NextLine    // if CX is 0 (end of line) jump to _NextLine
  }
  _Store: asm {          // COPY THE DATA
    movsb                // move the byte that DS:SI is pointing to into
                         //   ES:DI and increment SI and DI.
    loop    _LineLoop    // decrement CX and jump to _LineLoop if CX is
  }                      //   not 0 (end of line)

  _NextLine: asm {       // START A NEW LINE
    pop     di           // restore address of line just drawn
    dec     dl           // decrement DL (height)
    jz      _Exit        // when DL equals 0, you are done drawing the sprite
    add     di, 320      // if not end of sprite, add 320 to DI so that we
                         //   start drawing the sprite on the next line
    jmp     _DrawLoop    // go back to _DrawLoop and start drawing the next
  }                      //   line

  _Exit: asm {           // WE ARE DONE DRAWING THE SPRITE
    pop     ax           // restore AX
    pop     es           // restore ES
    pop     ds           // restore DS
  }
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// sgn() - This function is used by Line() to determine the sign of a long //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

int sgn (int a) {

  if (a &gt; 0)  return +1;
  if (a &lt; 0)  return -1;
  return 0;
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Funny_line() - This function draws a line from a,b, to c,d on screen    //
//                "where".  After it plots each pixel, it increments a     //
//                color counter for the next pixel.  You may easily alter  //
//                this to be a normal line function, and it will be quite  //
//                a bit faster than the original one I gave you.  This is  //
//                because I replaced all the floats with integers.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Funny_line (int a, int b, int c, int d, word where) {

int i,s,d1x,d1y,d2x,d2y,u,v,m,n,count;

  count = 50;
  u = c - a;       // u = x2 - x1  (change in x)
  v = d - b;       // v = y2 - y1  (change in y)

  d1x = sgn(u);    // d1x = sign of the change in x
  d1y = sgn(v);    // d1y = sign of the change in y

  d2x = sgn(u);    // d2x = sign of the change in x
  d2y = 0;         // d2y = 0
  m = abs(u);      // m = positive change in x
  n = abs(v);      // n = positive change in y

  if (m &lt;= n) {
    d2x = 0;       // d2x = 0
    d2y = sgn(v);  // d2y = sign of the change in y
    m = abs(v);    // m = positive change in y
    n = abs(u);    // n = positive change in x
  }
  s = m / 2;       // s = half of the change in x/y

  for (i=0;i&lt;m+1;i++) {
    Putpixel(a,b,count,where);   // plot the original x1 and y1
    count++;                     // increment the color
    if (count &gt; 100) count = 50; // if color is out of bounds, reset
    s += n;
    if (s &gt;= m) {
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
// SetUpScreen() - This procedure sets up the static background to be used //
//                 in the program.                                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetUpScreen() {

  int x,y,loop1,loop2,loop3;
  const byte circ[5][5] = {{ 0,10,10,10, 0},
               {10,13,12,11,10},
               {10,12,12,11,10},
               {10,11,11,11,10},
               { 0,10,10,10, 0}};

  // set the pallette for the toasters and some other things
  Pal (1 ,22,22,22);
  Pal (2 ,45,45,45);
  Pal (3 ,59,59,59);
  Pal (4 ,63,63,27);
  Pal (5 ,39,63, 3);
  Pal (6 ,51,39, 3);
  Pal (7 , 3,27, 3);
  Pal (8 ,15,39,15);
  Pal (9 ,35,35,35);
  Pal (10, 0, 0,40);
  Pal (11,10,10,50);
  Pal (12,20,20,60);
  Pal (13,30,30,63);

  // set pallette colors 50-100 to blue values between 13 and 63
  for (loop1 = 50; loop1 &lt; 101; loop1++)
    Pal (loop1,0,0,loop1-37);

  // put a copy of the pallette into ourpal[]
  for (loop1 = 0; loop1 &lt; 256; loop1++)
    GetPal (loop1,ourpal[loop1][0],ourpal[loop1][1],ourpal[loop1][2]);

  // draw funny lines
  for (loop1 = 0; loop1 &lt; 320; loop1++)
    Funny_line (0,199,loop1,0,Vaddr1);
  for (loop1 = 0; loop1 &lt; 200; loop1++)
    Funny_line (0,199,319,loop1,Vaddr1);

  // draw circles
  for (loop1 = 0; loop1 &lt; 200; loop1++) {
    x = random (315);
    y = random (195);
    for (loop2 = 0; loop2 &lt; 5; loop2++)
      for (loop3 = 0; loop3 &lt; 5; loop3++)
      // don't plot a pixel if its color is 0
        if (circ[loop2][loop3] != 0)
          Putpixel(x+loop2,y+loop3,circ[loop2][loop3],Vaddr1);
  }

  // Copy the entire screen at Vaddr1, our virtual screen on which we have
  // done all our graphics, onto the screen you see, VGA.
  Flip (Vaddr1,VGA);
  // also make a copy of the virtual screen to Vaddr2
  Flip (Vaddr1,Vaddr2);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Rotatepal() - This function rotates the colors between 50 and 100.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Rotatepal() {

  byte temp[3];
  int loop1;

  memmove(temp,ourpal[100],3);
  memmove(ourpal[51],ourpal[50],50*3);
  memmove(ourpal[50],temp,3);
  for (loop1 = 50; loop1 &lt; 101; loop1++)
    Pal (loop1,ourpal[loop1][0],ourpal[loop1][1],ourpal[loop1][2]);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// ScreenTrans() - This is a small function to copy a 30x30 pixel block    //
//                 from coordinates x,y on the Vaddr1 to           //
//                 coordinates x,y on the true vga screen.                 //
//                                                                         //
// ERROR, from Vaddr1 to Vaddr2
/////////////////////////////////////////////////////////////////////////////

void ScreenTrans (word x, word y) {
  asm {
    push    ds           // save DS
    push    es           // save ES
    mov     ax,Vaddr1    // set AX to the segment of Virtual Screen 1
    mov     es,ax        // set ES to segment of Vaddr1
    mov     ax,Vaddr2    // set AX to the segment of Virtual Screen 2
    mov     ds,ax        // set DS to segment of Vaddr2
    mov     bx,[x]       // set BX to x
    mov     dx,[y]       // set DX to y
    push    bx           // save x coordinate
    mov     bx, dx       // copy y coordinate to BX
    mov     dh, dl       // multiply y*256
    xor     dl, dl       // zero out DL
    shl     bx, 6        // multiply y*64
    add     dx, bx       // add y*64 to y*256 to get y*320
    pop     bx           // restore x coordinate
    add     bx, dx       // finalize the location
    mov     di, bx       // set DI to offset of Virtual Screen 1
    mov     si, di       // set SI to offset of Virtual Screen 2
    mov     al,60        // I have no idea why this is here.
    mov     bx, 30       // set BX to height of block
  }
  _loop1: asm {
    mov     cx, 24       // set CX to width of block divided by 2
    rep     movsw        // copy the screens and decrement CX by 1 until
                         //   CX is 0
    add     di,0x110     // 320 - 48 = 272 .. or 110 in hex
    add     si,0x110
    dec     bx           // decrease number lines left to draw
    jnz     _loop1       // if not done go back to _loop1
    pop     es           // restore ES
    pop     ds           // restore DS
  }

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// NewToaster() - This adds a new toaster to the screen.                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void NewToaster() {

  int loop1 = 0;

  do {
    if (toaster[loop1].active == FALSE) {
      toaster[loop1].x = random(200)+70;
      toaster[loop1].y = 0;
      toaster[loop1].active = TRUE;
      toaster[loop1].frame = 1;
      toaster[loop1].speed = random(3)+1;
      loop1 = 10;
    }
    loop1++;
  } while (loop1 &lt; NUMTOASTERS);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Fly() - This is the function where we move and put the toasters.        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Fly() {

  int loop1,loop2;
  char ch;

  for (loop1 = 0; loop1 &lt; NUMTOASTERS; loop1++)
    toaster[loop1].active = FALSE;

  ch = '0'; // assign a dummy value to ch
  NewToaster();

  do {
    // if a key is pressed...

    if (kbhit()) {
      ch = getch();
      // if '+' is pressed, add a toaster
      if (ch == '+') NewToaster();
      // if '-' is pressed, remove a toaster
      if (ch == '-') {
        loop1 = 0; // start with toaster 0
        do {
          // if the loop1 toaster is active
          if (toaster[loop1].active == TRUE) {
            // draw the toaster on the screen
            ScreenTrans(toaster[loop1].x,toaster[loop1].y);
            // then make it false
            toaster[loop1].active = FALSE;
            // break out of the loop
            loop1 = 10;
          }
          // check the next toaster
          loop1++;
        } while (loop1 &lt; NUMTOASTERS);
      }
    }

    // if no key was pressed...

    for (loop1 = 0; loop1 &lt; NUMTOASTERS; loop1++) {
      if (toaster[loop1].active == TRUE) {
        // Restore the background the toaster was over
        ScreenTrans(toaster[loop1].x,toaster[loop1].y);
        // Move the toaster
        toaster[loop1].x -= toaster[loop1].speed;
        toaster[loop1].y += toaster[loop1].speed;
        // When toaster reaches the edge of the screen, render it inactive
        // and bring a new one into existance.
        if ((toaster[loop1].x &lt; 0) || (toaster[loop1].y &gt; 170)) {
          toaster[loop1].active = FALSE;
          NewToaster();
        }
      }
    }

    // Draw all of the toasters
    for (loop1 = 0; loop1 &lt; NUMTOASTERS; loop1++) {
      // ... but only if they are active
      if (toaster[loop1].active == TRUE)
        switch (toaster[loop1].frame) {
        case 1: Putico(toaster[loop1].x,toaster[loop1].y,FRAME1,Vaddr1); break;
        case 2: Putico(toaster[loop1].x,toaster[loop1].y,FRAME3,Vaddr1); break;
        case 3: Putico(toaster[loop1].x,toaster[loop1].y,FRAME2,Vaddr1); break;
        case 4: Putico(toaster[loop1].x,toaster[loop1].y,FRAME3,Vaddr1); break;
        }
      // increment the current frame of the toaster
      toaster[loop1].frame += 1;
      // reset the frame if it gets to 5
      if (toaster[loop1].frame == 5) toaster[loop1].frame = 1;
    }

    WaitRetrace();
    Flip(Vaddr1,VGA);
    Rotatepal();

    // if the key pressed above was 0 (escape key) then read the escape code
    if (ch == 0) ch = getch();

  } while (ch != 27); // if the escape code was 27 (escape key) then exit

}
