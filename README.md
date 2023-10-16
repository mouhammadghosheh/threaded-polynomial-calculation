# threaded-polynomial-calculation
azrieli college


==Description==
This program calculates a polynom of a max degree of 3, it substitutes the parameter entered by the use for 'x' and calculates the polynom according to the coefficients there are.


Note: the program is divided into to two parts:
ex4a.c: calculates the polynom using threads
ex4b.c: calculates the polynom using proccesses and shared memory

functions:
1.print(): this function gets the input from the user which is the polynom and splits it into multiple strings and mini polynoms so that they can be easily calculated and then sends these polynoms accordingly to threads to calculate the outcome of them.

2.identifier(char* str): this function returns an integer which is the degree of the polynom(str).

3.X_Finder(char* str): this function checks if there is an 'x' in the polynom or not to identify if the polynom is a free member or not.

4.space_Finder(char* str): this function checks the string if there are spaces and if the string is not from the asked format.

(5).an added function in ex4b.c which is calcAns(char* str,int param): this function substitutes the int param for 'x' and calculates the polynom's outcom.





==Program Files==
ex4a.c: the part which was done using threads
ex4b.c: the part which was done using proccesses and shared memory
Makefile-to compile the program.
==How to compile?==
compile: gcc -o ex4a ex4a.c
	 gcc -o ex4b ex4b.c
run: ./ex4a
     ./ex4b		

==Input:==
the user inputs a polynom and a parameter in the format "POLYNOM, VAL"

==Output:==
"the final answer is: (the polynom's answer).
