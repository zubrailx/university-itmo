% Fact - male
male(grigoriy).
male(vasiliy).
male(alexei).
male(oleg).
male(anatoliy).
male(nikita).
male(evgeniy).
male(iliya).
male(maksim).
male(dmitriy).
male(alexander).
male(vadim).
male(vladimir).
% Fact - female
female(elena).
female(nataliya).
female(marina).
female(irina).
female(svetlana).
female(katerina).
female(anastasiya).
female(larisa).
female(anna).
female(dariya).
female(polina).
% Fact - parent
parent(irina, nikita).
parent(vasiliy, nikita).
parent(elena, irina).
parent(grigoriy, irina).
parent(elena, marina).
parent(grigoriy, marina).
parent(nataliya, vasiliy).
parent(alexei, vasiliy).
parent(nataliya, oleg).
parent(alexei, oleg).
parent(nataliya, anatoliy).
parent(alexei, anatoliy).
parent(svetlana, evgeniy).
parent(anatoliy, evgeniy).
parent(katerina, grigoriy).
parent(iliya, grigoriy).
parent(maksim, elena).
parent(anastasiya, elena).
parent(larisa, dmitriy).
parent(evgeniy, dmitriy).
parent(anna, alexei).
parent(alexander, alexei).
parent(svetlana, vadim).
parent(anatoliy, vadim).
parent(svetlana, dariya).
parent(anatoliy, dariya).
parent(marina, polina).
parent(vladimir, polina).
% Fact - spouse
spouse(anastasiya, maksim).
spouse(katerina, iliya).
spouse(elena, grigoriy).
spouse(marina, vladimir).
spouse(irina, vasiliy).
spouse(anna, alexander).
spouse(nataliya, alexei).
spouse(svetlana, anatoliy).
spouse(larisa, evgeniy).
spouse(X, Y) :- spouse(Y, X), X \= Y.

% Rules
son(X, Y) :- parent(Y, X), male(X).
daughter(X, Y) :- parent(Y, X), female(X).
sibling(X, Y) :- parent(Z, X), parent(Z, Y), X \= Y.
brother(X, Y) :- sibling(X, Y), male(X).
sister(X, Y) :- sibling(X, Y), female(X).
grandparent(X, Y) :- parent(Z, Y), parent(X, Z).
grandfather(X, Y) :- grandparent(X, Y), male(X).
grandmother(X, Y) :- grandparent(X, Y), female(X).
granddaughter(X, Y) :- grandparent(Y, X), female(X).
grandson(X, Y) :- grandparent(Y, X), male(X).
uncle(X, Y) :- parent(Z, Y), brother(Z, X).
aunt(X, Y) :- parent(Z, Y), sister(Z, X).

% niece(X, Y) :- uncle(Y, X), female(X).
% niece(X, Y) :- aunt(Y, X), female(X).
niece(X, Y) :- (uncle(Y, X); aunt(Y, X)), female(X).

% nephew(X, Y) :- uncle(Y, X), male(X).
% nephew(X, Y) :- aunt(Y, X), male(X).
nephew(X, Y) :- (uncle(Y, X); aunt(Y, X)), male(X).

% vim:ft=prolog
