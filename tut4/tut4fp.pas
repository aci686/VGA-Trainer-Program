{$I386_INTEL}
USES CRT, GO32;



CONST VGA = $a000;  (* This sets the constant VGA to the segment of the
                       VGA screen.                                      *)

VAR
Virscr : Array [1..64000] of byte;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure SetMCGA;  { This procedure gets you into 320x200x256 mode. }
BEGIN
  asm
     mov        ax,0013h
     int        10h
  end;
END;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure SetText;  { This procedure returns you to text mode.  }
BEGIN
  asm
     mov        ax,0003h
     int        10h
  end;
END;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure Cls1 (Col : Byte);
   { This clears the screen to the specified color, on the VGA }

BEGIN
   DosMemFillchar (VGA,0,64000,chr(Col) );

END;

{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure Cls2 (Col : Byte);
   { This clears the screen to the specified color on the virtual screen }

BEGIN
   FillChar (Virscr,64000,Col );

END;

{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
procedure WaitRetrace; assembler;
  { This waits until you are in a Verticle Retrace ... this means that all
    screen manipulation you do only appears on screen in the next verticle
    retrace ... this removes most of the "fuzz" that you see on the screen
    when changing the pallette. It unfortunately slows down your program
    by "synching" your program with your monitor card ... it does mean
    that the program will run at almost the same speed on different
    speeds of computers which have similar monitors. In our SilkyDemo,
    we used a WaitRetrace, and it therefore runs at the same (fairly
    fast) speed when Turbo is on or off. }

label
  l1, l2;
asm
    mov dx,3DAh
l1:
    in al,dx
    and al,08h
    jnz l1
l2:
    in al,dx
    and al,08h
    jz  l2
end;

{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}

Procedure PutPixel1 (X,Y : Integer; Col : Byte);
   { This puts a pixel at X,Y using color col, on the VGA }
BEGIN
  Mem [VGA:X+(Y*320)]:=col;
END;

{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}

Procedure PutPixel2 (X,Y : Integer; Col : Byte);
   { This puts a pixel at X,Y using color col, on the Virtual Screen}
BEGIN
  VirScr [X+(Y*320)]:=col;
END;

{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure Flip;
   { This flips the virtual screen to the VGA screen. }

BEGIN

  DosMemPut(VGA,0,VirScr,64000);

END;

{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure BlockMove;
   { This tests various ways of moving a block around the screen }
VAR loop1,loop2,loop3:Integer;
BEGIN
  For loop1:=1 to 50 do BEGIN                     { This draw a block    }
    for loop2:=1 to 50 do                         {  directly to VGA, no }
      for loop3:=1 to 50 do                       {  flipping            }
        putpixel1 (loop1+loop2,loop3,32);
    cls1 (0);
  END;

  For loop1:=1 to 50 do BEGIN                     { This draws a block     }
    for loop2:=1 to 50 do                         { to the virtual screen, }
      for loop3:=1 to 50 do                       { then flips it to VGA   }
        putpixel2 (loop1+loop2,loop3,32);
    flip;
    cls2 (0);
  END;

  For loop1:=1 to 50 do BEGIN                     { This draws a block     }
    for loop2:=1 to 50 do                         { to the virtual screen, }
      for loop3:=1 to 50 do                       { waits for a retrace,   }
        putpixel2 (loop1+loop2,loop3,32);   { then flips it to VGA   }
    waitretrace;
    flip;
    cls2 (0);
  END;
END;


{컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴}
Procedure PatternDraw;
   { This test the speed of flipping by drawing two patterns and flipping
     them }
VAR loop1,loop2:integer;
BEGIN
  for loop1:=1 to 100 do                        { This draws pattern one }
    for loop2:=1 to 100 do                      { to the virtual screen  }
      putpixel2 (loop1,loop2,loop1);       { then flips it to VGA   }
  flip;

  for loop1:=1 to 100 do                        { This draws pattern two }
    for loop2:=1 to 100 do                      { to the virtual screen  }
      putpixel2 (loop1,loop2,loop2);      { then flips it to VGA   }
  flip;
END;


BEGIN
  ClrScr;
  Writeln ('This program will demonstrate the power of virtual screens.');
  Writeln ('A block will firstly move across the screen, being drawn and');
  Writeln ('erased totally on the VGA. Then the same block will move');
  Writeln ('across, but will be drawn on the virtual screen and flipped');
  Writeln ('to the VGA screen without a retrace (see part 2). The the');
  Writeln ('block will go again, with flipping and a retrace.');
  Writeln;
  Writeln ('I will then draw a pattern, flip it to VGA, draw another');
  Writeln ('pattern, flip it to VGA, and repeat that until a key is pressed.');
  Writeln ('This will demonstrate that even when I put down 10000 pixels,');
  Writeln ('then flip them to the VGA, it is still relatively fast.      ');
  Writeln; Writeln;
  Writeln ('Hit any key to continue ...');
  readkey;
  setmcga;

  cls2 (0);           { After you have got the memory for the virtual screen,
                      it is usually filled with random garbage. It is always
                      wise to clear the virtual screen directly afterwards }
  BlockMove;


  Repeat
    PatternDraw;
  Until keypressed;

  Readkey;

  settext;

  Writeln ('All done. This concludes the fourth sample program in the ASPHYXIA');
  Writeln ('Training series. You may reach DENTHOR under the name of GRANT');
  Writeln ('SMITH on the MailBox BBS, or leave a message to ASPHYXIA on the');
  Writeln ('ASPHYXIA BBS. Get the numbers from Roblist, or write to :');
  Writeln ('             Grant Smith');
  Writeln ('             P.O. Box 270');
  Writeln ('             Kloof');
  Writeln ('             3640');
  Writeln ('I hope to hear from you soon!');
  Writeln; Writeln;
  Write   ('Hit any key to exit ...');
  Readkey;
END.\
