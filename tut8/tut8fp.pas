{$I386_INTEL}
USES CRT, GO32;

CONST VGA = $a000;  (* This sets the constant VGA to the segment of the
                       VGA screen.                                      *)

      MaxLines = 12;
      Obj : Array [1..MaxLines,1..2,1..3] of integer =
        (
        ((-10,-10,-10),(10,-10,-10)),((-10,-10,-10),(-10,10,-10)),
        ((-10,10,-10),(10,10,-10)),((10,-10,-10),(10,10,-10)),
        ((-10,-10,10),(10,-10,10)),((-10,-10,10),(-10,10,10)),
        ((-10,10,10),(10,10,10)),((10,-10,10),(10,10,10)),
        ((-10,-10,10),(-10,-10,-10)),((-10,10,10),(-10,10,-10)),
        ((10,10,10),(10,10,-10)),((10,-10,10),(10,-10,-10))
        );  { The 3-D coordinates of our object ... stored as (X1,Y1,Z1), }
            { (X2,Y2,Z2) ... for the two ends of a line }


Type Point = Record
               x,y,z:real;                { The data on every point we rotate}
             END;

VAR Lines : Array [1..MaxLines,1..2] of Point;  { The base object rotated }
    Translated : Array [1..MaxLines,1..2] of Point; { The rotated object }
    Xoff,Yoff,Zoff:Integer;               { Used for movement of the object }
    lookup : Array [0..360,1..2] of real; { Our sin and cos lookup table }
    Virscr : Array [1..64000] of byte;

{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure SetMCGA;  { This procedure gets you into 320x200x256 mode. }
BEGIN
  asm
     mov        ax,0013h
     int        10h
  end;
END;


{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure SetText;  { This procedure returns you to text mode.  }
BEGIN
  asm
     mov        ax,0003h
     int        10h
  end;
END;

{***************************************************************************}
Procedure Cls (Col : Byte);
   { This clears the screen to the specified color, on the VGA }

BEGIN
   DosMemFillchar (VGA,0,64000,chr(Col) );

END;



{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure Flip;
   { This flips the virtual screen to the VGA screen. }

BEGIN

  DosMemPut(VGA,0,VirScr,64000);

END;



{__________________________________________________________________________}
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




{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure Pal(ColorNo : Byte; R,G,B : Byte);
  { This sets the Red, Green and Blue values of a certain color }
Begin
   outportb($3c8,ColorNo);
   outportb($3c9,R);
   outportb($3c9,G);
   outportb($3c9,B);

End;





{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Function rad (theta : real) : real;
  {  This calculates the degrees of an angle }
BEGIN
  rad := theta * pi / 180
END;


{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure SetUpPoints;
  { This sets the basic offsets of the object, creates the lookup table and
    moves the object from a constant to a variable }
VAR loop1:integer;
BEGIN
  Xoff:=160;
  Yoff:=100;
  Zoff:=-256;
  For loop1:=0 to 360 do BEGIN
    lookup [loop1,1]:=sin (rad (loop1));
    lookup [loop1,2]:=cos (rad (loop1));
  END;
  For loop1:=1 to MaxLines do BEGIN
    Lines [loop1,1].x:=Obj [loop1,1,1];
    Lines [loop1,1].y:=Obj [loop1,1,2];
    Lines [loop1,1].z:=Obj [loop1,1,3];
    Lines [loop1,2].x:=Obj [loop1,2,1];
    Lines [loop1,2].y:=Obj [loop1,2,2];
    Lines [loop1,2].z:=Obj [loop1,2,3];
  END;
END;


{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure PutPixel2 (X,Y : Integer; Col : Byte);
   { This puts a pixel at X,Y using color col, on the Virtual Screen}
BEGIN
  Virscr [X+(Y*320)]:=col;
END;



{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure Line(a,b,c,d:integer;col:byte);
  { This draws a solid line from a,b to c,d in colour col }
  function sgn(a:real):integer;
  begin
       if a&gt;0 then sgn:=+1;
       if a&lt;0 then sgn:=-1;
       if a=0 then sgn:=0;
  end;
var i,s,d1x,d1y,d2x,d2y,u,v,m,n:integer;
begin
     u:= c - a;
     v:= d - b;
     d1x:= SGN(u);
     d1y:= SGN(v);
     d2x:= SGN(u);
     d2y:= 0;
     m:= ABS(u);
     n := ABS(v);
     IF NOT (M&gt;N) then
     BEGIN
          d2x := 0 ;
          d2y := SGN(v);
          m := ABS(v);
          n := ABS(u);
     END;
     s := m shr 1;
     FOR i := 0 TO m DO
     BEGIN
          putpixel2(a,b,col);
          s := s + n;
          IF not (s&lt;m) THEN
          BEGIN
               s := s - m;
               a:= a + d1x;
               b := b + d1y;
          END
          ELSE
          BEGIN
               a := a + d2x;
               b := b + d2y;
          END;
     end;
END;


{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure DrawLogo;
  { This draws 'ASPHYXIA' at the top of the screen in little balls }
CONST ball : Array [1..5,1..5] of byte =
         ((0,1,1,1,0),
          (1,4,3,2,1),
          (1,3,3,2,1),
          (1,2,2,2,1),
          (0,1,1,1,0));

VAR Logo : Array [1..5] of String;
    loop1,loop2,loop3,loop4:integer;
BEGIN
  pal (13,0,63,0);
  pal (1,0,0,40);
  pal (2,0,0,45);
  pal (3,0,0,50);
  pal (4,0,0,60);
  Logo[1]:=' O  OOO OOO O O O O O O OOO  O ';
  Logo[2]:='O O O   O O O O O O O O  O  O O';
  Logo[3]:='OOO OOO OOO OOO  O   O   O  OOO';
  Logo[4]:='O O   O O   O O  O  O O  O  O O';
  Logo[5]:='O O OOO O   O O  O  O O OOO O O';
  For loop1:=1 to 5 do
    For loop2:=1 to 31 do
      if logo[loop1][loop2]='O' then
        For loop3:=1 to 5 do
          For loop4:=1 to 5 do
            putpixel2 (loop2*10+loop3,loop1*4+loop4,ball[loop3,loop4]);
END;



{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure RotatePoints (X,Y,Z:Integer);
  { This rotates object lines by X,Y and Z; then places the result in
    TRANSLATED }
VAR loop1:integer;
    temp:point;
BEGIN
  For loop1:=1 to maxlines do BEGIN
    temp.x:=lines[loop1,1].x;
    temp.y:=lookup[x,2]*lines[loop1,1].y - lookup[x,1]*lines[loop1,1].z;
    temp.z:=lookup[x,1]*lines[loop1,1].y + lookup[x,2]*lines[loop1,1].z;

    translated[loop1,1]:=temp;

    If y&gt;0 then BEGIN
      temp.x:=lookup[y,2]*translated[loop1,1].x - lookup[y,1]*translated[loop1,1].y;
      temp.y:=lookup[y,1]*translated[loop1,1].x + lookup[y,2]*translated[loop1,1].y;
      temp.z:=translated[loop1,1].z;
      translated[loop1,1]:=temp;
    END;

    If z&gt;0 then BEGIN
      temp.x:=lookup[z,2]*translated[loop1,1].x + lookup[z,1]*translated[loop1,1].z;
      temp.y:=translated[loop1,1].y;
      temp.z:=-lookup[z,1]*translated[loop1,1].x + lookup[z,2]*translated[loop1,1].z;
      translated[loop1,1]:=temp;
    END;

    temp.x:=lines[loop1,2].x;
    temp.y:=cos (rad(X))*lines[loop1,2].y - sin (rad(X))*lines[loop1,2].z;
    temp.z:=sin (rad(X))*lines[loop1,2].y + cos (rad(X))*lines[loop1,2].z;

    translated[loop1,2]:=temp;

    If y&gt;0 then BEGIN
      temp.x:=cos (rad(Y))*translated[loop1,2].x - sin (rad(Y))*translated[loop1,2].y;
      temp.y:=sin (rad(Y))*translated[loop1,2].x + cos (rad(Y))*translated[loop1,2].y;
      temp.z:=translated[loop1,2].z;
      translated[loop1,2]:=temp;
    END;

    If z&gt;0 then BEGIN
      temp.x:=cos (rad(Z))*translated[loop1,2].x + sin (rad(Z))*translated[loop1,2].z;
      temp.y:=translated[loop1,2].y;
      temp.z:=-sin (rad(Z))*translated[loop1,2].x + cos (rad(Z))*translated[loop1,2].z;
      translated[loop1,2]:=temp;
    END;
  END;
END;



{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure DrawPoints;
  { This draws the translated object to the virtual screen }
VAR loop1:Integer;
    nx,ny,nx2,ny2:integer;
    temp:integer;
BEGIN
  For loop1:=1 to MaxLines do BEGIN
    If (translated[loop1,1].z+zoff&lt;0) and (translated[loop1,2].z+zoff&lt;0) then BEGIN
      temp:=round (translated[loop1,1].z+zoff);
      nx :=round (256*translated[loop1,1].X) div temp+xoff;
      ny :=round (256*translated[loop1,1].Y) div temp+yoff;
      temp:=round (translated[loop1,2].z+zoff);
      nx2:=round (256*translated[loop1,2].X) div temp+xoff;
      ny2:=round (256*translated[loop1,2].Y) div temp+yoff;
      If (NX &gt; 0) and (NX &lt; 320) and (NY &gt; 25) and (NY &lt; 200) and
         (NX2&gt; 0) and (NX2&lt; 320) and (NY2&gt; 25) and (NY2&lt; 200) then
           line (nx,ny,nx2,ny2,13);
    END;
  END;
END;

{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure ClearPoints;
  { This clears the translated object from the virtual screen ... believe it
    or not, this is faster then a straight "cls (vaddr,0)" }
VAR loop1:Integer;
    nx,ny,nx2,ny2:Integer;
    temp:integer;
BEGIN
  For loop1:=1 to MaxLines do BEGIN
    If (translated[loop1,1].z+zoff&lt;0) and (translated[loop1,2].z+zoff&lt;0) then BEGIN
      temp:=round (translated[loop1,1].z+zoff);
      nx :=round (256*translated[loop1,1].X) div temp+xoff;
      ny :=round (256*translated[loop1,1].Y) div temp+yoff;
      temp:=round (translated[loop1,2].z+zoff);
      nx2:=round (256*translated[loop1,2].X) div temp+xoff;
      ny2:=round (256*translated[loop1,2].Y) div temp+yoff;
      If (NX &gt; 0) and (NX &lt; 320) and (NY &gt; 25) and (NY &lt; 200) and
         (NX2&gt; 0) and (NX2&lt; 320) and (NY2&gt; 25) and (NY2&lt; 200) then
           line (nx,ny,nx2,ny2,0);
    END;
  END;
END;


{ĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄĄ}
Procedure MoveAround;
  { This is the main display procedure. Firstly it brings the object towards
    the viewer by increasing the Zoff, then passes control to the user }
VAR deg,loop1:integer;
    ch:char;
BEGIN
  deg:=0;
  ch:=#0;
  Cls(0);
  DrawLogo;
  For loop1:=-256 to -40 do BEGIN
    zoff:=loop1*2;
    RotatePoints (deg,deg,deg);
    DrawPoints;
    WaitRetrace;
    flip;
    ClearPoints;
    deg:=(deg+5) mod 360;
  END;

  Repeat
    if keypressed then BEGIN
      ch:=upcase (Readkey);
      Case ch of 'A' : zoff:=zoff+5;
                 'Z' : zoff:=zoff-5;
                 ',' : xoff:=xoff-5;
                 '.' : xoff:=xoff+5;
                 'S' : yoff:=yoff-5;
                 'X' : yoff:=yoff+5;
      END;
    END;
    DrawPoints;
    WaitRetrace;
    flip;
    ClearPoints;
    RotatePoints (deg,deg,deg);
    deg:=(deg+5) mod 360;
  Until ch=#27;
END;


BEGIN
  Writeln ('Greetings and salutations! Hope you had a great Christmas and New');
  Writeln ('year! ;-) ... Anyway, this tutorial is on 3-D, so this is what is');
  Writeln ('going to happen ... a wireframe square will come towards you.');
  Writeln ('When it gets close, you get control. "A" and "Z" control the Z');
  Writeln ('movement, "," and "." control the X movement, and "S" and "X"');
  Writeln ('control the Y movement. I have not included rotation control, but');
  Writeln ('it should be easy enough to put in yourself ... if you have any');
  Writeln ('hassles, leave me mail.');
  Writeln;
  Writeln ('Read the main text file for ideas on improving this code ... and');
  Writeln ('welcome to the world of 3-D!');
  writeln;
  writeln;
  Write ('  Hit any key to contine ...');
  Readkey;
  SetMCGA;
  SetUpPoints;
  MoveAround;
  SetText;
  Writeln ('All done. This concludes the eigth sample program in the ASPHYXIA');
  Writeln ('Training series. You may reach DENTHOR under the names of GRANT');
  Writeln ('SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid');
  Writeln ('Connectix BBS user, and occasionally read RSAProg.');
  Writeln ('For discussion purposes, I am also the moderator of the Programming');
  Writeln ('newsgroup on the For Your Eyes Only BBS.');
  Writeln ('The numbers are available in the main text. You may also write to me at:');
  Writeln ('             Grant Smith');
  Writeln ('             P.O. Box 270');
  Writeln ('             Kloof');
  Writeln ('             3640');
  Writeln ('I hope to hear from you soon!');
  Writeln; Writeln;
  Write   ('Hit any key to exit ...');
  Readkey;
END.
