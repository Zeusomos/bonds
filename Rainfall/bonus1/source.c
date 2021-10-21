#include <string.h>
 
int main(int argc, char **argv)
{
   char buff[40];
   int numb = atoi(argv[1]);
   if (numb > 9)
       return 1;
   memcpy(buff, argv[2], numb * 4);
   if (numb == 0x574f4c46)
       execl("/bin/sh", "sh", 0);
   return 0;
}
