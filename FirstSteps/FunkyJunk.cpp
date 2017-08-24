#include <iostream>
using namespace std;

int main(){
	int a = 4;
	int* b = &a;
	int c = *b;

	cout << c << "\n";
	cout << *b << "\n";
	cout << &a << "\n";

	int arr[5];
	for(int i = 0; i < 5; i++){
		arr[i] = i*2;
	}

	int* pointer = arr;

	cout << "Array pointer's address: " << &pointer << "\n";
	cout << "Dereferenced pointer: " << *pointer << "\n";

	for(int i = 0; i < 5; i++){
		cout << pointer[i] << "\n";
	}

	cout << "Array pointer's address: " << &pointer << "\n";
}