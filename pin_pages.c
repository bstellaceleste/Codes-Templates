/*
 *We want to create a shared memory region and after mmapping it, pin it to physical memory.
 */

//#include <linux/slab.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 4096

int main()
{
	/* name of the shared memory */
	const char *name = "vtf";
	/* shared mem file descriptor - 
	 * nb of pages to be pinned - 
	 * write_flag(1 to say that pinned pages should be writable) - 
	 * result of get_user_pages_fast */
	int fd, nb_pages, write = 1, ret = -1;
	/* pointer to the shared mem object  */
	void *ptr;
	/* starting user address for the pin pages  */
	unsigned long start_add;
	/* pinned pages  */
	struct page **pages;
	void *test = NULL;
	/* create the shared mem object  */
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared mem obj  */
	ftruncate(fd, SIZE);
	
	/* memory map the shared mem obj */
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

	/* the starting address is the pointer to the shared mem we've just mmapped  */
	start_add = (unsigned long)ptr;

	/* we'll allocate 1MB for this sh_mm so we need 2⁸ pages of 4KB each  */
	nb_pages = 2 << 8;

	/* let's allocate memory for the pages we're going to pin  */
	pages = calloc(1,sizeof(struct page *));

	/* now let's pin the shared memory in physical mem  */
//	ret = get_user_pages_fast(start_add, nb_pages, write, pages);

	printf("ptr:%p - pages: %p - test: %p - test_char: %s\n", ptr, pages, test, (char*)test);

	return ret;
}
