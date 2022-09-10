% task1
/*
parent(boris, alla). boris is parent of alla
parent(bedros, filipp).
parent(edmuntas, kristina).
parent(alla, kristina).
parent(kristina, denis).
parent(vladimir, denis).

spouse(filipp, alla).
spouse(vladimir, kristina).
*/

% task 2
/*
parent(boris, alla).
parent(boris, temel).
parent(bedros, filipp).
parent(edmuntas, kristina).
parent(alla, kristina).
parent(kristina, denis).

grandparent(X,Y) :- parent(X,Z), parent(Z,Y).
sibling(X,Y) :- parent(Z,X), parent(Z,Y), not(X = Y).
uncle(X,Y) :- parent(Z,Y), sibling(X,Z).
*/

% task 3 - recursion (FAILED)
/*
factorial(0,1). 
factorial(N,F) :-  N>0, N1 is N-1, factorial(N1,F1), F is N * F1.
*/

% task 4 - collision
/*
parent(oleg, sergei).
parent(maria, dasha).
spouse(oleg, dasha).
spouse(sergei, maria).

spouse(X,Y) :- spouse(Y,X).
parent(X,Y) :- spouse(X,Z), parent(Z,Y).
grandparent(X,Y) :- parent(X,Z), parent(Z,Y).
*/

% task 5 - recursion and iterations
% type :- readchar(X), write(X), X \= '.', type.

% vim:ft=prolog
