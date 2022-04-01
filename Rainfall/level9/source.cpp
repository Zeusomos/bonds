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
           return this->value - rhs.value;
       }
       void setAnnotation(char *data){
           size_t len = strlen(data);
           memcpy(this->data, data, len);
       }
       int (N::*func)(N &);
       char data[100];
       int value;
};
 
int main(int argc, char **argv)
{
   if (argc < 2)
       exit(1);
   N *first = new N(5);
   N *second = new N(6);
   first->setAnnotation(argv[1]);
   (second->*(second->func))(*first);
   return 0;
}
