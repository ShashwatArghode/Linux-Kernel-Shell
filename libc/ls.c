#include <sbulib.h>
#ifndef O_RDONLY 
#define O_RDONLY        000000
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY     002000
#endif

int ls(int argc , char *argv[])
{

        int cur_dir;
        int nread, file_dir ,pos;
	char buf[1024];
        struct temp_dirent *d;

        cur_dir = open(argv[1], O_DIRECTORY);

        if(-1 == cur_dir)
        {
                if(argc >1){
                        file_dir = open(argv[1], O_RDONLY);
                }else {
                        file_dir = open("." , O_DIRECTORY);
                }

                if( file_dir == -1)
                {
                        write(1, argv[1], strLength(argv[1]));
                        close(file_dir);
                        return 0;
                }
        }else
                {
                        for (; ;){
                                nread = getdents(0,(struct temp_dirent*) buf, 1024);
                                if(nread == 0)
                                        break;
                                        for(pos =0; pos < nread; ){
                                        d = (struct temp_dirent *)(buf +pos);
                                        write(1 , d->name, (strLength(d->name) +1));
                                        pos = pos + d->len;
                                }
                        }
                        close(cur_dir);
                        return 0;
                }
//              close(cur_dir);
//              return 0;


        return 0;
}
                  
