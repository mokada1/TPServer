#pragma once

#include <stdio.h>
#include <stdlib.h>

class TPError
{
public:
	static void ErrorHandling(const char* message)
	{
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
	}
};