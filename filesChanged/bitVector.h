
//returns 0 -> hole in place, returns 1 ->allocated/shared page exists in place
unsigned char returnSpecificElement(unsigned char * bitVector, int place);

//changes place bit from 0 -> 1, 1 -> 0
void changeSpecificElement(unsigned char * bitVector, int place);
