#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100
#define BACKLOG 5


int
main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Server socket: %d\n", sfd);

    /* Construct server socket address, bind socket to it,
       and make this a listening socket */

    /* For an explanation of the following check, see the errata notes for
       pages 1168 and 1172 at http://www.man7.org/tlpi/errata/. */


    remove(SV_SOCK_PATH);

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));

    listen(sfd, BACKLOG);

    for (;;) {          /* Handle client connections iteratively */

        /* Accept a connection. The connection is returned on a new
           socket, 'cfd'; the listening socket ('sfd') remains open
           and can be used to accept further connections. */

        cfd = accept(sfd, NULL, NULL);

        /* Transfer data from connected socket to stdout until EOF */

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
            write(STDOUT_FILENO, buf, numRead);

        close(cfd);
    }
}