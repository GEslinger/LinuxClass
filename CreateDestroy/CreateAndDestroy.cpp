 // CreateAndDestroy.cpp 
 // CreateAndDestroy class member-function definitions. 

 #include <iostream> 
 #include "CreateAndDestroy.h"       // include CreateAndDestroy class definition 

 using namespace std; 

  CreateAndDestroy::CreateAndDestroy( int ID, string messageText ){    // constructor
     objectID = ID;
     message = messageText; // descriptive message
     cout << "Object " << objectID << "  constructor runs " << message << endl;
  }

  CreateAndDestroy::~CreateAndDestroy() {    				// destructor
  cout << "Object "<< objectID << "  destructor runs   " << message << endl;
  }

