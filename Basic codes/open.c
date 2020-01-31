#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main()
{
	char buf[20];// = (char *) calloc(20, sizeof(char));
	int op = open("test_open", O_RDWR), wr = write(op, "toto\ntata\n", 11);
	printf("%d - %d\n", op, wr);
//	sleep(3);
        int rd = read(op, buf, op);
	buf[rd] = 0;
	close(op);
//	printf("%d - %s\n", rd, (char *)buf);
	printf("%d\n", rd);
	printf("%s\n", buf);

}
