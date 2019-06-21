#include <systemd/sd-daemon.h>
#include "definitions.h"

int main(void)
{
	int fd = 0;
	int n = sd_listen_fds(0);
	if (n > 1)
	{
		fprintf(stderr, "too many file descriptors...\n");
		exit(EXIT_FAILURE);
	}
	else if (n == 1)
	{
		printf("systemd socket...\n");
		fd = SD_LISTEN_FDS_START + 0;
	}
	else
	{
		printf("own socket...\n");
		struct sockaddr_un un;
		fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (fd < 0)
		{
			printf("socket failed...\n");
			exit(EXIT_FAILURE);
		}

		memset(&un, 0, sizeof(un));
		un.sun_family = AF_UNIX;
		strcpy(un.sun_path, SERVER_SOCK_FILE);
		unlink(SERVER_SOCK_FILE);

		if (bind(fd, (const struct sockaddr *)&un, sizeof(un)) != 0)
		{
			fprintf(stderr, "bind failed...\n");
			exit(EXIT_FAILURE);
		}

		if (listen(fd, SOMAXCONN) != 0)
		{
			fprintf(stderr, "listen failed...\n");
			exit(EXIT_FAILURE);
		}
	}
	struct sockaddr sa;
	socklen_t len = sizeof(sa);

	int connfd = accept(fd, &sa, &len);
	if (connfd < 0)
	{
		fprintf(stderr, "acccept failed...\n");
		exit(EXIT_FAILURE);
	}

	char buff[MAX_BUF_SIZE];

	memset(buff, 0, sizeof(buff));
	if (read(connfd, buff, sizeof(buff)) > 0)
	{
		printf("rabbit: %s", buff);
		if (write(connfd, "the matrix has you\n", 20) == -1)
		{
			fprintf(stderr, "write failed...\n");
			close(fd);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "read failed...\n");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);
}
