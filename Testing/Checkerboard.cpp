#include <iostream>
using namespace std;

int main(){
	bool a = false;
	for(int i = 0; i < 120; i++){
		if(a){cout<<"*";}else{cout<<" ";}
		if(i%15==0){cout<<"\n";}
		a = !a;
	}
	cout<<"\n\n";
	return 0;
}