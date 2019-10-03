#define BUFSIZE 16

void dir_hexdump(const char *path) {
    DIR *dirp = opendir(path);
    if (dirp == NULL) {
        perror(path);
        exit(1);
    }

    struct dirent *dir = readdir(dirp);
    if (dir == NULL) {
        perror(path);
        exit(1);
    }
    
    /*
     * struct dirent {
     * ino_t d_ino;
     * off_t d_off;
     * unsigned short d_reclen;
     * unsigned char d_type;
     * char d_name[256#;]
     */

    int offset = 0;
    int size;
    
    static unsigned char buf[BUFSIZE];

    unsigned short length = 0;
    for (struct dirent *tmp = dir; tmp->d_reclen > 0; tmp = (void*)tmp + tmp->d_reclen)
        length += tmp->d_reclen;

    do {
        size = length >= BUFSIZE? BUFSIZE: length;
        length -= size;
        memcpy(buf, dir, size);
        dir = (void*)dir + size;

        printf("\x1b[46m");     // background color cyan
        printf("|%07x|", offset);
        offset += size;
        printf("\x1b[49m ");    // background color default

        printf(" | ");
        for (int i = 0; i < size; i++) {
            if (buf[i] == 0){
                printf("\x1b[30m");     // foreground color black
                printf("%02x ", buf[i]);
                printf("\x1b[39m");     // foreground color default
            } else {
                printf("%02x ", buf[i]);
            }
            if(i%2 == 1) {
                printf("| ");
            }
        }
    
        printf(" ");
        printf("\x1b[45m");     // background color magenta
        printf("|");
        for (int i = 0; i < size; i++) {
            if (0x20 <= buf[i] && 0x7e >= buf[i]) {
	            printf("%c", buf[i]);
            } else {
                printf("\x1b[40m");     // background color black
	            printf(".");
                printf("\x1b[45m");     // background color magenta
            }
        }
        printf("|");

        printf("\x1b[49m");     // background color default
        printf("\n");
    }while(length > 0);
}

