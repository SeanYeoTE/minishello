#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc > 1)
	{
		if (argv[i] == NULL | argv[i] == " " | argv == "")
			printf("\n");
		while (argv[i])
		{
			printf("%s", argv[i]);
			printf(" ");
			i++;
		}
		printf("\n");
	}
	return (0);
}
