#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
int guess = -1;
int num = 0;
int tries = 0;
int range = 100;

int main(){

	srand((int)time(0)); // seed the random function with system time
	num = rand() % range;
	cout << "I'm thinking of a number between 0 and " << range << "\n";

	while(guess != num){
		cin >> guess;
		tries++;

		if(guess > num){
			cout << "Too high!\n";
		} else if(guess < num){
			cout << "Too low!\n";
		}
	}

	cout << "Congratulations! It only took you " << tries << " tries to guess my number\n";
	return 0;
}