// TSP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "TSPSolve.h"

void main(int argc, char** argv)
{
	TSPSolve* tsp = &TSPSolve::Instance();
	if (argc == 1)
	{	
		printf("Demo output!\n");
		tsp->m_solve("../../Data/TSP/20_0.txt", true);
	}
	else if (argc == 2)
	{
		tsp->m_solve(string(argv[1]), true);
	}
	else if (argc >= 3)
	{
		if (argv[2] == "true" || argv[2] == "1")
		{
			tsp->m_solve(string(argv[1]), true);

		}
		else
		{
			tsp->m_solve(string(argv[1]), false);
		}
	}
	
	return;


}




