#include<sys/stat.h>
#include<fcntl.h>
#include "../lib/tlpi_hdr.h"

//#ifdef BUF_SIZE 
	#define BUF_SIZE 	1024
//#endif

int
main (int argc, char *argv[]) {

	int 	inputFD, outputFD, openFlags;
	mode_t		filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s oldFile newFile\n", argv[0]);

	/* Open input and output files */
	inputFD = open(argv[1], O_RDONLY);

	if(inputFD == -1) {
		errExit("Error: Opening file %s\n",argv[1]);
	}
	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH; /* rw-rw-rw- */

	outputFD = open(argv[2], openFlags, filePerms);
	if (outputFD == -1)
		errExit("Error: Opening file %s", argv[2]);

	/* Transfer data until we encounter end of input or an error */
	while ((numRead = read(inputFD, buf, BUF_SIZE)) > 0) {
		if (write(outputFD, buf, numRead) != numRead)
			fatal("couldn't write whole buffer");
	}
	if (numRead == -1)
		errExit("read");
	if (close(inputFD) == -1)
		errExit("close input");
	if (close(outputFD) == -1)
		errExit("close output");
	
	exit(EXIT_SUCCESS);
}
