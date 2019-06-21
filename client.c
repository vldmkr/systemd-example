#include "definitions.h"

int main(void)
{
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1)
	{
		fprintf(stderr, "socket failed...\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_un un;
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, CLIENT_SOCK_FILE);
	unlink(CLIENT_SOCK_FILE);
	if (bind(fd, (struct sockaddr *)&un, sizeof(un)) != 0)
	{
		fprintf(stderr, "bind failed...\n");
		exit(EXIT_FAILURE);
	}

	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, SERVER_SOCK_FILE);
	if (connect(fd, (const struct sockaddr *)&un, sizeof(un)) != 0)
	{
		printf("connect failed...\n");
		exit(EXIT_FAILURE);
	}

	if (write(fd, "follow the white rabbit\n", 25) > 0)
	{
		char buff[MAX_BUF_SIZE];
		memset(buff, 0, sizeof(buff));
		if (read(fd, buff, sizeof(buff)) > 0)
		{
			printf("matrix: %s", buff);
		}
		else
		{
			fprintf(stderr, "read failed...\n");
			close(fd);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "write failed...\n");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);
}