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

#include &lt;stdlib.h&gt; // calloc(), free(), exit()


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

  #define VGA  0xa0000        // The address of our VGA screen
  #define ICON  30*48         // This is the size of our pictures
  #define NUMTOASTERS  10     // The maximum number of toasters we have
  #define NUMCIRCLES  200     // The number of little circles on the screen
  #define TRUE  1
  #define FALSE  0


//                              //
// GLOBAL VARIABLE DECLARATIONS //
//                              //


  __dpmi_regs regs;

  char virscr1 [64000], virscr2 [64000]; // virtual screens are 320x200

  byte ourpal[256][3];                    // A virtual pallette

  struct toastinfostrct {
	 int      x;
	 int      y;
	 word     speed;
	 word     frame;
	 byte     active;
  } toaster[NUMTOASTERS];         // The toaster info;


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetMCGA() - This function gets you into 320x200x256 mode.               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetMCGA(void) {
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x13;
  __dpmi_int(0x10, &amp;regs);
}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// SetText() - This function gets you into text mode.                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void SetText(void) {
  memset(&amp;regs, 0, sizeof regs);
  regs.x.ax = 0x03;
  __dpmi_int(0x10, &amp;regs);
}



//\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4
// This puts a pixel on the virtual screen by writing directly to memory.
void Putpixel2(unsigned int x, unsigned int y, unsigned char col)
{
	 virscr1[y * 320 + x] = col;

}

//\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4



/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// WaitRetrace() - This waits until you are in a Verticle Retrace.         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void WaitRetrace()

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

void GetPal(unsigned char ColorNo, unsigned char *R,unsigned char *G,unsigned char *B)
{
  // This reads the values of the Red, Green and Blue values of a certain
  //  color and returns them to you.


	outp(0x3C7,ColorNo);
	*R = inp(0x3C9);
	*G = inp(0x3C9);
	*B = inp(0x3C9);
}


////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Flip() - This copies the entire screen at "source" to destination.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void flip1(void)
{
	// This flips the first virtual screen to the VGA screen.

		  dosmemput(virscr1,64000,0xa0000);

}

void flip2(void)
{
	// This flips the virtual screen to the second virtual screen

  memcpy ((char *)(virscr2),(char *)(virscr1),64000);

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Putico() - This puts an icon, EXCEPT it's color 0 (black) pixels, onto  //
//            the screen "where", at position X,Y                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Putico(word X, word Y, const byte *sprt) {

  int i,j;

  for (i=0; i&lt;48; i++) {
		 for (j=0;j&lt;30;j++) {
				if (sprt[i+(j*48)]!=0)
					virscr1[X+i+(Y+j)*320]=sprt[i+(j*48)];
		 };
  };

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

void Funny_line (int a, int b, int c, int d) {

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
	 Putpixel2(a,b,count);   // plot the original x1 and y1
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

void SetUpScreen(void) {

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
	 GetPal (loop1,&amp;ourpal[loop1][0],&amp;ourpal[loop1][1],&amp;ourpal[loop1][2]);

  // draw funny lines
  for (loop1 = 0; loop1 &lt; 320; loop1++)
	 Funny_line (0,199,loop1,0);
  for (loop1 = 0; loop1 &lt; 200; loop1++)
	 Funny_line (0,199,319,loop1);

  // draw circles
  for (loop1 = 0; loop1 &lt; 200; loop1++) {
	 x = (double)rand () * (315) / RAND_MAX;
	 y = (double)rand () * (195) / RAND_MAX;
	 for (loop2 = 0; loop2 &lt; 5; loop2++)
		for (loop3 = 0; loop3 &lt; 5; loop3++)
		// don't plot a pixel if its color is 0
		  if (circ[loop2][loop3] != 0)
			 Putpixel2(x+loop2,y+loop3,circ[loop2][loop3]);
  }

  // Copy the entire screen at Vaddr1, our virtual screen on which we have
  // done all our graphics, onto the screen you see, VGA.
  flip1();

  // also make a copy of the virtual screen to Vaddr2
  flip2();

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Rotatepal() - This function rotates the colors between 50 and 100.      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void Rotatepal(void) {

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

  int i,j;

  for (i=0;i&lt;48;i++) {
		 for (j=0;j&lt;30;j++) {
				virscr1[x+i+(y+j)*320]=virscr2[x+i+(y+j)*320];
		 };
  };

}


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// NewToaster() - This adds a new toaster to the screen.                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void NewToaster(void) {

  int loop1 = 0;

  do {
	 if (toaster[loop1].active == FALSE) {
		toaster[loop1].x = ((double)rand () * (200) / RAND_MAX ) +70;
		toaster[loop1].y = 0;
		toaster[loop1].active = TRUE;
		toaster[loop1].frame = 1;
		toaster[loop1].speed = ((double)rand () * (3) / RAND_MAX) +1;
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

void Fly(void) {

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
		  case 1: Putico(toaster[loop1].x,toaster[loop1].y,FRAME1); break;
		  case 2: Putico(toaster[loop1].x,toaster[loop1].y,FRAME3); break;
		  case 3: Putico(toaster[loop1].x,toaster[loop1].y,FRAME2); break;
		  case 4: Putico(toaster[loop1].x,toaster[loop1].y,FRAME3); break;
		  }
		// increment the current frame of the toaster
		toaster[loop1].frame += 1;
		// reset the frame if it gets to 5
		if (toaster[loop1].frame == 5) toaster[loop1].frame = 1;
	 }

	 WaitRetrace();
	 flip1();
	 Rotatepal();

	 // if the key pressed above was 0 (escape key) then read the escape code
	 if (ch == 0) ch = getch();

  } while (ch != 27); // if the escape code was 27 (escape key) then exit

}




///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                MAIN FUNCTION                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
int main(void) {

  printf (
	 "Hello! This program will demonstrate the principals of animation.\n"
	 "The program will firstly generate an arb background screen to a\n"
	 "virtual page, then flip it to the VGA. A toaster will then start\n"
	 "to move across the screen. Note that the background will be restored\n"
	 "after the toaster has passed over it. You may add or remove toasters\n"
	 "by hitting ""+"" or ""-"" respectively. Note that the more frames you\n"
	 "use, usually the better the routine looks. Because of space\n"
	 "restrictions, we only had room for three frames.\n\n");
  printf (
	 "The toasters were drawn by Fubar (Pieter Buys) in Autodesk Animator.\n"
	 "I wrote a small little program to convert them into CONSTANTS. See\n"
	 "the main text to find out how to load up AA CEL files directly.\n\n\n");
  printf ("Hit any key to continue...");
  getch();

  SetMCGA();
  SetUpScreen();  // Draw background screen to Vaddr1, then flip to VGA
  Fly();          // Make the toasters fly around the screen.
  SetText();

  printf (
	 "All done. This concludes the seventh sample program in the ASPHYXIA\n"
	 "Training series. You may reach DENTHOR under the names of GRANT\n"
	 "SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid\n"
	 "Connectix BBS user, which is unfortunatly offline for the moment.\n"
	 "For discussion purposes, I am also the moderator of the Programming\n"
	 "newsgroup on the For Your Eyes Only BBS.\n"
	 "The numbers are available in the main text. You may also write to me at:\n"
	 "             Grant Smith\n"
	 "             P.O. Box 270\n"
	 "             Kloof\n"
	 "             3640\n"
	 "I hope to hear from you soon!\n\n\n");
  printf ("Hit any key to exit...");
  getch();

  return (0);
}
