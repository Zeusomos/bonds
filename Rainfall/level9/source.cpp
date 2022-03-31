#include <cstring>
#include <stdlib.h>
 
class N{
   public:
       N(int value)
       {
           this->value = value;
           func = &N::operator+;
       }
       int operator+(N &rhs)
       {
           in