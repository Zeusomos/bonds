#include <string.h>
 
int main(int argc, char **argv)
{
   char buff[40];
   int numb = atoi(argv[1]);
   if (numb > 9)
       return 1;
   memcpy