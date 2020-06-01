(*****************************************************************************)
(*                                                                           *)
(* TUT7.PAS - VGA Trainer Program 7 (in Pascal)                              *)
(*                                                                           *)
(* "The VGA Trainer Program" is written by Denthor of Asphyxia.  However it  *)
(* was limited to Pascal only in its first run.  All I have done is taken    *)
(* his original release, translated it to C++, and touched up a few things.  *)
(* I take absolutely no credit for the concepts presented in this code, and  *)
(* am NOT the person to ask for help if you are having trouble.  -Snowman    *)
(*                                                                           *)
(* Program Notes : This program demonstrates animation.  Several of the      *)
(*                 functions have been converted to assembler.  This         *)
(*                 tutorial is a whopper, so just take your time.            *)
(*                                                                           *)
(* Author        : Grant Smith (Denthor)  - denthor@beastie.cs.und.ac.za     *)
(*                                                                           *)
(*****************************************************************************)

{$X+}
USES crt;

CONST VGA = $a0000;

Type Toastinfo = Record                 { This is format of of each of our }
                 x,y:integer;              { records for the flying toasters }
                 speed,frame:integer;
                 active:boolean;
               END;

     icon = Array [1..30*48] of byte;  { This is the size of our pictures }

CONST frame1 : icon = (
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
);
      frame2 : icon = (
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
);
      frame3 : icon = (
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
);


VAR
    ourpal : Array [0..255,1..3] of byte; { A virtual pallette }
    toaster : Array [1..10] of toastinfo; { The toaster info }

    Virscr,VirScr2: Array [1..64000] of byte; { our first and second virtual screens}

{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure SetMCGA;  { This procedure gets you into 320x200x256 mode. }
BEGIN
  asm
     mov        ax,0013h
     int        10h
  end;
END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure SetText;  { This procedure returns you to text mode.  }
BEGIN
  asm
     mov        ax,0003h
     int        10h
  end;
END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure Putpixel1 (X,Y : Integer; Col : Byte);
  { This puts a pixel on the VGA screen by writing directly to memory. }
BEGIN
  Mem [VGA+X+(Y*320)]:=Col;
END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}

Procedure PutPixel2 (X,Y : Integer; Col : Byte);
   { This puts a pixel at X,Y using color col, on the Virtual Screen}
BEGIN
  Virscr [X+(Y*320)]:=col;
END;

{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
procedure WaitRetrace; assembler;
  {  This waits for a vertical retrace to reduce snow on the screen }
label
  l1, l2;
asm
    mov dx,3DAh
@l1:
    in al,dx
    and al,08h
    jnz @l1
@l2:
    in al,dx
    and al,08h
    jz  @l2
end;

{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure GetPal(ColorNo : Byte; Var R,G,B : Byte);
  { This reads the values of the Red, Green and Blue values of a certain
    color and returns them to you. }
Begin
   Port[$3c7] := ColorNo;
   R := Port[$3c9];
   G := Port[$3c9];
   B := Port[$3c9];
End;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure Pal(ColorNo : Byte; R,G,B : Byte);
  { This sets the Red, Green and Blue values of a certain color }
Begin
   Port[$3c8] := ColorNo;
   Port[$3c9] := R;
   Port[$3c9] := G;
   Port[$3c9] := B;
End;



{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure Flip;
   { This flips the virtual screen to the VGA screen. }

BEGIN

  move(VirScr,mem[VGA],64000);

END;

{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}

Procedure Flip2;
   { This flips the virtual screen to the second virtual screen}

BEGIN

  move(VirScr,VirScr2,64000);

END;

{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure putico(X,Y:Word;VAR sprt : icon); { ASSEMBLER;
  (* This puts an icon, EXCEPT it's color 0 (black) pixels, onto the screen
    "where", at position X,Y *)
label
  _Redraw, _DrawLoop, _Exit, _LineLoop, _NextLine, _Store, _NoPaint;

asm
    push  ds
    push  es
    lds   si,Sprt
    mov   ax,X     (* ax = x *)
    mov   bx,Y     (* bx = y *)
@_Redraw:
    push    ax
    mov     ax,[where]
    mov     es,ax

    mov     ax, bx                  (* ; ax = bx  x = y *)
    mov     bh, bl                  (* ; y = y * 256  bx = bx * 256 *)
    xor     bl, bl
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1
    shl     ax, 1                   (* ; y = y * 64   ax = ax * 64 *)
    add     bx, ax                  (* ; y = (y*256) + (Y*64)  bx = bx + ax (ie y*320) *)

    pop     ax                      (* ; get back our x *)


    add     ax, bx                  (* ; finalise location *)
    mov     di, ax

    mov   dl,30    (* dl = height of sprite *)
    xor   ch,ch
    mov   cl,48     (* cx = width of sprite *)
    cld
    push  ax
    mov   ax,cx
@_DrawLoop:
    push  di            (* store y adr. for later *)
    mov   cx,ax         (* store width *)
@_LineLoop:
    mov   bl,byte ptr [si]
    or    bl,bl
    jnz   @_Store
@_NoPaint:
    inc    si
    inc    di
    loop   @_LineLoop
    jmp    @_NextLine
@_Store:
    movsb
    loop  @_LineLoop
@_NextLine:
    pop   di
    dec   dl
    jz    @_Exit
    add   di,320        (* di = next line of sprite *)
    jmp   @_DrawLoop
@_Exit:
    pop   ax
    pop   es
    pop   ds
}
  var
  i,j:integer;

  begin

  for i:=0 to 48-1 do begin
       for j:=0 to 30-1 do begin
            if (sprt[i+(j*48)]&lt;&gt;0) then
               virscr[X+i+(Y+j)*320]:=sprt[i+(j*48)];
       end;
  end;

end;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure Funny_line(a,b,c,d:integer);
  { This procedure draws a line from a,b to c,d on screen "where". After
    each pixel it plots, it increments a color counter for the next pixel.
    you may easily alter this to be a normal line procedure, and it will
    be quite a bit faster than the origional one I gave you. This is
    because I replaced all the reals with integers. }

  function sgn(a:real):integer;
  begin
       if a&gt;0 then sgn:=+1;
       if a&lt;0 then sgn:=-1;
       if a=0 then sgn:=0;
  end;
var i,s,d1x,d1y,d2x,d2y,u,v,m,n:integer;
    count:integer;
begin
     count:=50;
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
          putpixel2(a,b,count);
          inc (count);
          if count=101 then count:=50;
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



{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure SetUpScreen;
  { This procedure sets up the static background to be used in the program }

CONST circ : Array [1..5,1..5] of byte =
        ((0,10,10,10,0),
         (10,13,12,11,10),
         (10,12,12,11,10),
         (10,11,11,11,10),
         (0,10,10,10,0));

VAR x,y:integer;
    loop1,loop2,loop3:integer;

BEGIN
  pal (1,22,22,22);
  pal (2,45,45,45);
  pal (3,59,59,59);
  pal (4,63,63,27);
  pal (5,39,63,3);
  pal (6,51,39,3);
  pal (7,3,27,3);
  pal (8,15,39,15);
  pal (9,35,35,35);
  pal (10, 0, 0,40);
  pal (11,10,10,50);
  pal (12,20,20,60);
  pal (13,30,30,63);

  For loop1:=50 to 100 do
    pal (loop1,0,0,loop1-36);

  For loop1:=0 to 255 do
     getpal (loop1,OurPal[loop1,1],OurPal[loop1,2],OurPal[loop1,3]);

  For loop1:=0 to 319 do
    Funny_line (0,199,loop1,0);
  For loop1:=0 to 199 do
    Funny_line (0,199,319,loop1);

  For loop1:=1 to 200 do BEGIN
    x:=random (315);
    y:=random (195);
    For loop2:=1 to 5 do
      For loop3:=1 to 5 do
        if circ [loop2,loop3]&lt;&gt;0 then
          putpixel2 (x+loop2,y+loop3,circ [loop2,loop3]);
  END;

  flip;  (* Copy the entire screen at vaddr, our virtual screen *)
                     (* on which we have done all our graphics, onto the    *)
                     (* screen you see, VGA *)

  flip2;
END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure rotatepal;
  { This procedure rotates the colors between 50 and 100 }
VAR temp : Array [1..3] of byte;
    loop1:integer;
BEGIN
  Move(OurPal[100],Temp,3);
  Move(OurPal[50],OurPal[51],50*3);
  Move(Temp,OurPal[50],3);
  For loop1:=50 to 100 do
    pal (loop1,OurPal[loop1,1],OurPal[loop1,2],OurPal[loop1,3]);
END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure ScreenTrans (x,y:word);
{
  (* This is a small procedure to copy a 30x30 pixel block from coordinates
    x,y on the virtual screen to coordinates x,y on the true vga screen *)
BEGIN

  asm
    push    ds
    push    es
    mov     ax,virscr
    mov     es,ax
    mov     ax,virscr2
    mov     ds,ax
    mov     bx,[X]
    mov     dx,[Y]
    push    bx                      (* ; and this again for later *)
    mov     bx, dx                  (* ; bx = dx *)
    mov     dh, dl                  (* ; dx = dx * 256 *)
    xor     dl, dl
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1
    shl     bx, 1                   (* ; bx = bx * 64 *)
    add     dx, bx                  (* ; dx = dx + bx (ie y*320) *)
    pop     bx                      (* ; get back our x *)
    add     bx, dx                  (* ; finalise location *)
    mov     di, bx                  (* ; es:di = where to go *)
    mov     si, di
    mov     al,60
    mov     bx, 30         (* Hight of block to copy *)
@@1 :
    mov     cx, 24         (* Width of block to copy divided by 2 *)
    rep     movsw
    add     di,110h        (* 320 - 48 = 272 .. or 110 in hex *)
    add     si,110h
    dec     bx
    jnz     @@1

    pop     es
    pop     ds
  end;
  (* I wrote this procedure late last night, so it may not be in it's
    most optimised state. Sorry :-) *)
}
  var
  i,j:integer;

  begin

  for i:=0 to 48-1 do begin
       for j:=0 to 30-1 do begin
            virscr[X+i+(Y+j)*320]:=virscr2[X+i+(Y+j)*320];
       end;
  end;

END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure NewToaster;
  { This adds a new toaster to the screen }
VAR loop1:integer;
BEGIN
  loop1:=0;
  repeat
    inc (loop1);
    if not (toaster[loop1].active) then BEGIN
      toaster[loop1].x:=random (200)+70;
      toaster[loop1].y:=0;
      toaster[loop1].active:=true;
      toaster[loop1].frame:=1;
      toaster[loop1].speed:=Random (3)+1;
      loop1:=10;
    END;
  until loop1=10;
END;


{\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4\C4}
Procedure Fly;
  { This is the procedure where we move and put the toasters }
VAR loop1,loop2:integer;
    ch:char;
BEGIN
  For loop1:=1 to 10 do
    toaster[loop1].active:=FALSE;
  ch:=#0;
  NewToaster;
  Repeat
    if keypressed then BEGIN
      ch:=readkey;
      if ch='+' then NewToaster;      { If '+' is pressed, add a toaster }
      if ch='-' then BEGIN            { if '-' is pressed, remove a toaster }
        loop1:=0;
        repeat
          inc (loop1);
          if toaster[loop1].active then BEGIN
            screentrans (toaster[loop1].x,toaster[loop1].y);
            toaster [loop1].active:=FALSE;
            loop1:=10;
          END;
        until loop1=10;
      END;
    END;
    for loop1:=1 to 10 do
      if toaster[loop1].active then BEGIN
        screentrans (toaster[loop1].x,toaster[loop1].y);
          { Restore the backgrond the toaster was over }
        dec (toaster[loop1].x,toaster[loop1].speed);
        inc (toaster[loop1].y,toaster[loop1].speed);
          { Move the toaster }
        if (toaster[loop1].x&lt;1) or (toaster[loop1].y&gt;170) then BEGIN
          toaster[loop1].active:=FALSE;
          NewToaster;
        END;
          { When toaster reaches the edge of the screen, render it inactive
            and bring a new one into existance. }
      END;
    for loop1:=1 to 10 do
      if toaster[loop1].active then BEGIN
        CASE toaster [loop1].frame of
           1   : putico (toaster[loop1].x,toaster[loop1].y,frame1);
           3   : putico (toaster[loop1].x,toaster[loop1].y,frame2);
           2,4 : putico (toaster[loop1].x,toaster[loop1].y,frame3);
        END;
        toaster[loop1].frame:=toaster[loop1].frame+1;
        if toaster [loop1].frame=5 then toaster[loop1].frame:=1;
          { Draw all the toasters on the VGA screen }
      END;
    waitretrace;
    flip;
    rotatepal;
  Until ch=#27;
END;


BEGIN
  Randomize;       { Make sure that the RANDOM funcion really is random }
  ClrScr;
  writeln ('Hello! This program will demonstrate the principals of animation.');
  writeln ('The program will firstly generate an arb background screen to a');
  writeln ('virtual page, then flip it to the VGA. A toaster will then start');
  writeln ('to move across the screen. Note that the background will be restored');
  writeln ('after the toaster has passed over it. You may add or remove toasters');
  writeln ('by hitting "+" or "-" respectively. Note that the more frames you');
  writeln ('use, usually the better the routine looks. Because of space');
  writeln ('restrictions, we only had room for three frames.');
  writeln;
  writeln ('The toasters were drawn by Fubar (Pieter Buys) in Autodesk Animator.');
  writeln ('I wrote a small little program to convert them into CONSTANTS. See');
  writeln ('the main text to find out how to load up AA CEL files directly.');
  writeln;
  writeln;
  Write ('  Hit any key to contine ...');
  Readkey;
  SetMCGA;

  SetupScreen;     (* Draw the background screen to VADDR, then flip it to
                     the VGA screen *)
  readln;

  Fly;             (* Make the toasters fly around the screen *)

  SetText;
  Writeln ('All done. This concludes the seventh sample program in the ASPHYXIA');
  Writeln ('Training series. You may reach DENTHOR under the names of GRANT');
  Writeln ('SMITH/DENTHOR/ASPHYXIA on the ASPHYXIA BBS. I am also an avid');
  Writeln ('Connectix BBS user, which is unfortunatly offline for the moment.');
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
