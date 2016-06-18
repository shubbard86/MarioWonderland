/*
This file is to performa Monte Carlo Simulation for a "Game" one of my friends
came up with. The game is a modificaiton of a random walk. Mario has the ability
to draw any of 4 different types of cards. Jump left, Jump right,
Grow (double size), and shrink (halve size).

The question was posed, what is the probability that Mario will return
to center with his original size.

His size is indicated by an int though for calculations it is used as
2^size. This is because when size was the actual value for Mario's size
mario would become size 0 about 30% of the time at which point he would never
grow any larger.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
// initialization step for the game method, the prng and the global size and location)

int Marios_bounce(int hops);
int randint(int n);
long int size;
long double location;

int main(int argc, char *argv[])
{

	srand(time(NULL)); //seed the PRNG with system time. 

	// Checks of input arguments and ability to open the output file.
	if (atoi(argv[1]) < 1) {
		printf("Please put a positive, non-zero integer for hops\n format: Mario_wonderland <hops in each trial> <trials>");
		exit(1);
	}
	if (atoi(argv[2]) < 1) {
		printf("Please put a positive, non-zero integer for trials\n format: Mario_wonderland <hops in each trial> <trials>");
		exit(1);
	}
	FILE *f = fopen("MarioCarlo.txt", "w");
	if (f == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	//initalize counters
	int hops = atoi(argv[1]);
	int total_trials = atoi(argv[2]);
	int trial = 0;
	int success = 0;

	fprintf(f, "Number of turns, size, location, victory\n"); //print file heading
	
	//Loop to run the Simulation and print results to file
	while (trial < total_trials){
		int outcome = Marios_bounce(hops);
		if (outcome != -1){
			fprintf(f, "%d,%li,%Lf,1\n", outcome, size, location);
			success++;
		} else{
			fprintf(f, "%d,%li,%Lf,0\n", outcome, size, location);
		}
		trial++;
	} 
	double percent_success = (double)success / (double)total_trials;
	printf("%f%% Success Rate in %d trials in %d turns\n", percent_success, total_trials, hops);
	fclose(f);
	return 0;
}


// The Game Method
int Marios_bounce(int hops)
{
	//initalize starting variables and counters
	size = 0;
	location = 0;
	int i = 0;

	while(i < hops) {
		i++;
		int r = randint(4); // produces a random int between 0 and 3
		if (r == 0){
			location = location + pow(2, size);
		} else if(r == 1){
			location = location - pow(2, size);
		} else if(r == 2){
			size++;
		} else if(r == 3){
			size--;
		}
		if (location == 0 && size == 0) {
			break;
		}
	}
	if (i < hops) {
		return i;
	} else {
		return -1;
	}
}

/* Returns an integer in the range [0, n).
Uses rand(), and so is affected-by/affects the same seed.
Method copied from Laurence Gonsalves 
(stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c)
*/
int randint(int n) {
	if ((n - 1) == RAND_MAX) {
		return rand();
	} else {
	// chop off all of the values that would cause skew...
	long end = RAND_MAX / n; // truncate skew
	assert (end > 0L);
	end *= n;

	// ... and ignore results from rand() that fall above that limit.
	// Worst case the loop condition should succeed 50% of the time,
	// so we can expect to bail out of this loop pretty quickly.)
	int r;
	while ((r = rand()) >= end);

	return r % n;
	}
}
