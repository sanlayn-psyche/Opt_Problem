#include <iostream>
#include "ap_delegate.h"

void main(int argc, char** argv)
{
    ap_delegate apd;

	if (argc == 1)
	{
		printf("Demo output!\n");
		apd.m_solve();
	}
	else if (argc == 4)
	{
		apd.m_solve(argv[1], argv[2], argv[3]);
	}


}

