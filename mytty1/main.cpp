/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char** argv)
{
   int fd;
   int res;
   char buf[100];
   
   fd = open("/dev/tty1", O_RDWR);
   if (fd < 0) 
   {
      perror("open");
      return 1;
   }

   int count = 0;
   while (1)
   {
      res = read(fd, buf, 1);
      if (res < 0)
      {
         perror("read");
         return 1;
      }
      printf("%04d $ %d\n",count++, (int)buf[0]);
   }

   return 0;
}
