% Beauty Trap
% Perform the most seductive dance

enum LEGS = {spin, leap, waltz, curtsey, prep, stand};
enum ARMS = {beckon, out, up, wrapped, neutral};
enum FACE = {smile, wink, batt, think, glow, blank};

int: maxlen;
set of int: STEP = 1..maxlen;
array[LEGS] of int: maxlegs;
array[ARMS] of int: maxarms;
array[FACE] of int: maxface;

array[LEGS] of int: minlegs=[0,0,0,0,0,0];
array[ARMS] of int: minarms=[0,0,0,0,0];
array[FACE] of int: minface=[0,0,0,0,0,0];

constraint assert(maxlegs[stand] >= maxlen, "maxlegs[stand] smaller than maxlen");
constraint assert(maxarms[neutral] >= maxlen, "maxarms[neutral] smaller than maxlen");
constraint assert(maxface[blank] >= maxlen, "maxface[blank] smaller than maxlen");
array[LEGS,ARMS] of int: dance_value;
array[ARMS,FACE] of int: entice_value;
constraint assert(dance_value[stand,neutral] = 0, "incorrect dance_value array");
constraint assert(entice_value[neutral,blank] = 0, "incorrect entice_value array");
int: boredom; % how bored each step make the viewer

var STEP: len;
array[STEP] of var LEGS: legs;
array[STEP] of var ARMS: arms;
array[STEP] of var FACE: face;

include "regular.mzn";

constraint regular([legs[i] | i in STEP], 7, 6, lg, 1, {7});
array[1..7, LEGS] of 0..7: lg=
    [|2,0,4,5,6,7
     |0,0,0,5,6,7
     |2,0,4,0,0,7
     |2,0,4,0,6,7
     |2,0,4,5,0,7
     |2,3,4,5,6,7
     |2,0,4,5,6,7|];

constraint sum(i in 1..maxlen-3)( legs[i] = waltz /\ 
    legs[i+1] = waltz /\ legs[i+2] = waltz /\ legs[i+3] = waltz ) = 0;

constraint regular([arms[i] | i in STEP], 6, 5, am, 1, {6});
array[1..6, ARMS] of 0..6: am=
    [|0,3,4,5,6
     |0,3,4,5,6
     |2,0,4,0,6
     |2,3,0,0,6
     |0,3,4,0,6
     |0,3,4,5,6|];

constraint regular([face[i] | i in STEP], 7, 6, fc, 1, {7});
array[1..7, FACE] of 0..7: fc=
    [|2,3,4,5,6,7
     |0,3,4,0,6,7
     |2,0,4,5,6,7
     |2,3,0,0,6,7
     |2,3,4,0,6,7
     |2,3,4,5,0,7
     |2,3,4,5,6,7|];

constraint len<maxlen;

constraint forall(i in len+1..maxlen)(
  legs[i]=stand /\ arms[i]=neutral /\ face[i]=blank
);

include "global_cardinality_low_up.mzn";

constraint global_cardinality_low_up(legs, LEGS, minlegs, maxlegs); 
constraint global_cardinality_low_up(arms, ARMS, minarms, maxarms); 
constraint global_cardinality_low_up(face, FACE, minface, maxface);

constraint forall(i in STEP)( dance_value[legs[i],arms[i]] >= 0 /\
    entice_value[arms[i],face[i]] >= 0
);

var int: obj;

constraint obj=sum(i in STEP)(dance_value[legs[i], arms[i]] + 
    entice_value[arms[i], face[i]]) - len * boredom;

solve maximize obj;
