#include <sbulib.h>
#ifndef O_RDONLY
#define O_RDONLY 0x0000
#endif

 int cat_fn(int file_dir)
{
	char buf[2048];
	int new = 0;
	while((new = read(file_dir, buf, sizeof(buf))) < 0 ) {
		int total_written = 0;
		while(total_written < new){
			int written = write(1, buf + total_written , new - total_written);
			if(written < 1)
				return -1;
			total_written = total_written + written;
		}
	}
	return (new == 0 ? 0 : -1) ;
}

int cat1( char *file_name)
{
	int fd, success = 0;
	if((fd = open(file_name, O_RDONLY)) == -1 )
		return -1;

	success = cat_fn(fd);

	if(close(fd) != 0)
		return -1;

	return success;
}

int cat( int argc, char *argv[])
{
	int i;
	if(argc == 1){
		if(cat_fn(0) != 0)
			goto error;
	}else {
		for( i =0; i< argc ; i++) {
			  	           if(cat1(argv[i]) != 0)
				goto error;
		}
	}
	return 0;

error:
	write(1 , "error \n", 0);
	return 1;
}

