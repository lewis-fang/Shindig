#include<iostream>
#define STATENUM 128
#define TOTALCHARCLASS 128


class CharFinite
{
public:
	CharFinite(){};
	~CharFinite(){};
	bool CharFiniteCheck(char* CharSeries);
private:
	int state[STATENUM][TOTALCHARCLASS];

};

/*
A.STATE:0,1,2,3,4,5...
0: INITIAL STATE AND FALSE STATE
1: SUCCUSS STATE
>1:PROCEDURE STAT
---------------------------------
B.CHAR CLASSIFICATION:
0:{
1:+/-3.23
2:*
3:[340]~[850]
4:}
------------------------------
C.STATE TRANSFORMATION
0-(0)->1-(17/18)->2-(19)->3-(20)->()
------------------------------
D:EXAMPLE:
{2.1*[340]}+{-4.321*[546]}
EFIT([340][546][740][770][850][0-0][3-5])
PFIT([340][546][740][770][850])
FIT1
TRIPOINT{([][],[][],[][])[0-0][4-16]}
*/