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
           int a = this->value + rhs.value;
           return a;
       }
       int operator-(N &rhs)
       {
           return this->value - r