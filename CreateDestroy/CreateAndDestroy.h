// CreateAndDestroy class definition 
// Member functions defined in CreateAndDestroy.cpp

#include <string> 
using namespace std; 

#ifndef CREATE_H 
#define CREATE_H 

class CreateAndDestroy { 
 public: 
    CreateAndDestroy(int, string);  // constructor
   ~CreateAndDestroy();  // desctructor
private: 
    int objectID; // ID number for object
    string  message; // message describing object 
};   // endclass CreateAndDestroy 

#endif
