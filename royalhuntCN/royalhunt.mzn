include "globals.mzn";

int: n; % number of court members
set of int: COURT = 1..n;
int: emperor = 1;
array[COURT] of int: rank;
array[COURT] of int: ability;

int: m; % number of horses
set of int: HORSE = 1..m;
array[HORSE] of int: beauty;
array[HORSE] of int: speed;

array[COURT,HORSE] of int: enjoy;

array[COURT] of var 0..m: horse; % 0->nochoice;
array[HORSE] of var 0..n: court; % 0->nochoice;

constraint forall(i in COURT where horse[i] in HORSE)(
  court[horse[i]] = i
);

constraint forall(i in HORSE where court[i] in COURT)(
  horse[court[i]] = i
);

constraint horse[emperor] > 0;


constraint forall(i in 2..n where horse[i] in HORSE)(
  enjoy[emperor,horse[emperor]] > enjoy[i,horse[i]]
);

constraint alldifferent_except_0(horse);
constraint alldifferent_except_0(court);

constraint sum(i in COURT)(horse[i]>0) = min(m,n);

constraint forall(i in COURT where horse[i] in HORSE)(enjoy[i,horse[i]] > 0);

constraint forall(i in COURT where horse[i] in HORSE)(
  forall(j in COURT where horse[j] in HORSE /\ rank[i] > rank[j])(
    beauty[horse[i]] >= beauty[horse[j]]
  )
);

constraint forall(i in COURT where horse[i] in HORSE)(
  forall(j in COURT where horse[j] ==0)(
    rank[i] >= rank[j]
  )
);


var int: breakrules;
constraint breakrules=sum(i in HORSE where court[i] in COURT)(
  sum(j in HORSE where speed[i] > speed[j])(
  court[j] == 0 \/ ability[court[i]] < ability[court[j]]
  )
);

var int: obj;
constraint obj=sum(i in COURT where horse[i] in HORSE)
  (enjoy[i,horse[i]]) - 100 * breakrules;

solve maximize obj;

output["horse = ",show(horse),";\nobj = ",show(obj)];
