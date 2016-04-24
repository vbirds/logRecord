
#include <stdio.h>
#include "logrecord.h"

int main(void)
{
	int i = -2;

	if (i < 0 )
	{
		LR_LOG(__FILE__, __LINE__, 4, -1, "i 小于0");
	}

	return 0;
}
