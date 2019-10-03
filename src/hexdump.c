#define BUFSIZE 16

void file_hexdump(const char *path){
    FILE *fp;
    int offset = 0;
    int size;

    fp = fopen(path, "rb");

    if(fp == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(1);
    }

    unsigned char buf[BUFSIZE];

    do {
        size = fread(buf, sizeof(unsigned char), BUFSIZE, fp);
    
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
    }while(size != 0);

    fclose(fp);
}

