#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

int main(int argc, char** argv) {
    assert(argc == 2);
    char buff_to_send[strlen(argv[1]) + 1];
    memset(buff_to_send, 0, sizeof(buff_to_send));
    strcat(buff_to_send, argv[1]);
    int fd = open("/dev/character_device", O_RDWR);
    assert(!(fd < 0));
    write(fd, buff_to_send, strlen(buff_to_send));//strlen(argv[1])
    char data[strlen(argv[1]) + strlen("Hello,  ")];
    read(fd, data, 128);
    printf("%s\n", data);
    assert(!(close(fd) < 0));
    return 0;
}
