# Genetic Sequence Formulizer
This program will take a sequence entered by the user and tries to find a formula producing this sequence. I made this to play around with genetic programming. Inspired by the following two talks:

* [Reducing Wasted Evaluations in Cartesian Genetic Programming, by Brian Goldman](https://www.youtube.com/watch?v=3HsgVHv1ho8)
* [Automatic Algorithm Invention with a GPU, by Wes Faler](https://www.youtube.com/watch?v=xQDazGrKsuM)

I used a technique similar to the one described in the first presentation (the one called *single*), as it seemed to be the easiest one. The nice thing is that there is always just one parent, but there are inactive genes which are allowed to mutate freely.

## Examples
Some actual output of the program (truncated):

	$ ./main 0 0 0 0 0 0 0
	your input:	0, 0, 0, 0, 0, 0, 0, 
	formula:	(x-x)*x
	continuation:	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ...
	
	$ ./main 1 2 3
	your input:	1, 2, 3, 
	formula:	((x-18)/(x-18))+x
	continuation:	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, ...
	
	$ ./main 1 2 4 8
	your input:	1, 2, 4, 8, 
	formula:	2^x
	continuation:	1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, ...
	
	$ ./main 37
	your input:	37, 
	formula:	(10+12)+15
	continuation:	37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 

## Notes

* The algorithm does not yet simplify the formula (I should take this into account when scoring the genes).
* The sequences are 0-based.
* The constants used in the genes are bound to 20 (so in the last example, it needed multiple constants added).
* It treats ```1/0``` as ```INT_MAX``` (we need some safety as we evaluate randomly generated expressions).
* I may be using some c++14 features. Install a new clang or gcc to get this awesomeness.