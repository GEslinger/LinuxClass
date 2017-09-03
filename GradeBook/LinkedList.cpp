#include <iostream>
#include <cstring>
using namespace std;

struct Node{
	Node* next;
	int data;
};

void printLL(Node* n){
	Node* current = n;

	while(current != NULL){
		cout << current->data; 
		if(current->next != NULL){
			cout << " -> ";
		}
		current = current->next;
	}
	cout << "\n";
}

void deleteLL(Node* n){
	Node* current = n;
	Node* next;

	while(current != NULL){
		next = current->next;
		cout << "deleting node with " << current->data << "\n";
		delete current;
		current = next;
	}
}

int main(){
	int k;

	Node* head = NULL;

	cout << "Insert a number into the linked list: ";

	while(true){
		cin >> k;
		if(k == 0){break;}

		Node* temp = new Node;
		temp->data = k;
		temp->next = head;

		head = temp;
		cout << "Current linked list:\n";
		printLL(head);
		cout << "\nEnter another number: ";
		
	}

	cout << "DELETE THIS\n";
	deleteLL(head);
}