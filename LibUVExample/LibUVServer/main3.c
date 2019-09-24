#include <stdlib.h>
#include <uv.h>
#include "tinyweb3.h"
#define MAXPATH  1024

int main(int argc, char * argv[])
{
	//char buffer[MAXPATH];
	//_getcwd(buffer, MAXPATH);
	//printf("%s", buffer);

	char dirBuffer[MAXPATH];
	strcpy(dirBuffer, argv[0]);
	for (int i = strlen(dirBuffer); i > 0; i--)
	{
		if (dirBuffer[i] == '\\')
		{
			dirBuffer[i] = '\0';
			break;
		}
	}
	tinyweb_start(uv_default_loop(), "0.0.0.0", 8080, dirBuffer);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}