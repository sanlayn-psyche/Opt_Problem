#include "RuntimeCut.h"

#include "output.h" 

#define MAX(x,y) x > y ? x : y

RuntimeCut::RuntimeCut(GRBVar* vars, Mode mode, int n)
{
	m_vars = vars;
	m_mode = mode;
	m_num = n;
	m_Solution = (double*)malloc(m_num * m_num * sizeof(double));
}

RuntimeCut::~RuntimeCut()
{
	free(m_Solution);
}

void RuntimeCut::set_mode(Mode mode)
{
	m_mode = mode;
}

void RuntimeCut::m_findAllLoop(double* sol, vector<vector<int>>& loop, double gap)
{
	vector<bool> VisitList(m_num, false);
	int ite = 0;
	int counter = 0;
	loop.push_back(vector<int>());

	while (counter < m_num)
	{
		if (VisitList[ite])
		{
			for (int i = 0; i < m_num; i++)
			{
				if (!VisitList[i])
				{
					if (loop.size() > 0)
					{
						int s = *loop.back().begin();
						int e = loop.back().back();
						if ((m_Solution[s * m_num + e] <= gap && m_Solution[e * m_num + s] <= gap))
						{
							loop.pop_back();
						}
					}
					ite = i;
					loop.push_back(vector<int>());
					break;
				}
			}
		}

		if (VisitList[ite])
		{
			break;
		}
		else
		{
			loop.back().push_back(ite);
			VisitList[ite] = true;
			counter++;
		}

		int max_index = ite;
		double max_value = 0.0;

		for (size_t i = 0; i < m_num; i++)
		{
			if (!VisitList[i])
			{
				double temp = MAX(m_Solution[ite * m_num + i], m_Solution[i * m_num + ite]);
				if (temp > max_value)
				{
					max_value = temp;
					max_index = i;
				}
			}
		}
		if (max_index != ite && max_value >= gap)
		{
			ite = max_index;
		}
	}

	VisitList.clear();
}



void RuntimeCut::callback()
{
	try
	{
		if (where == GRB_CB_MIPNODE && getIntInfo(GRB_CB_MIPNODE_STATUS) == GRB_OPTIMAL && m_mode == RC_CUTMODE_USER)
		{
			static MinCut<double> mincut(m_num);
	
			mincut.m_data = getNodeRel(m_vars , m_num * m_num);
	
			
			mincut.get_minCut();

 			GRBLinExpr expr = 0;
			if (mincut.m_minCut < 2.0 && (mincut.m_graph_cutPoint != 0 || mincut.m_graph_cutPoint != m_num))
			{
				for (size_t i = 0; i < mincut.m_graph_cutPoint; i++)
				{
					for (size_t j = mincut.m_graph_cutPoint; j < mincut.m_graph.size(); j++)
					{
						expr += m_vars[mincut.m_graph[i] * m_num + mincut.m_graph[j]];
					}
				}
				addCut(expr >= 2.0);

				m_Solution = getNodeRel(m_vars, m_num * m_num);
				vector<vector<int>> loop;

				m_findAllLoop(m_Solution, loop, 0.0);
				int max_index, max_lenth = 0;

				if (loop.size() > 1)
				{
					for (int i = 0; i < loop.size(); i++)
					{
						if (loop[i].size() > max_lenth)
						{
							max_lenth = loop[i].size();
							max_index = i;
						}
					}
					GRBLinExpr expr = 0;

					for (auto j = loop[max_index].begin(); j != loop[max_index].end() - 1; j++)
					{
						expr += m_vars[(*j) * m_num + *(j + 1)];
					}
					expr += m_vars[loop[max_index][0] + loop[max_index].back() * m_num];
					addLazy(expr <= max_lenth - 1);
				}
				
				/*
				
				int close_index;
				double close_value;

				vector<bool> visitList(m_num, false);
				expr = 0;
				for (int i = 0; i < mincut.m_graph_cutPoint; i++)
				{
					if (!visitList[i])
					{
						visitList[i] = true;
						close_value = 0.0;
						close_index = i;

						for (int j = 0; j < mincut.m_graph_cutPoint; j++)
						{
							if (!visitList[j])
							{
								if (close_value < m_Solution[mincut.m_graph[i] * m_num + mincut.m_graph[j]])
								{
									close_value = m_Solution[mincut.m_graph[i] * m_num + mincut.m_graph[j]];
									close_index = j;
								}
								if (close_value < m_Solution[mincut.m_graph[j] * m_num + mincut.m_graph[i]])
								{
									close_value = m_Solution[mincut.m_graph[j] * m_num + mincut.m_graph[i]];
									close_index = j;
								}
							}
						}
						if (close_index != i)
						{
							expr += m_vars[mincut.m_graph[i] * m_num + mincut.m_graph[close_index]];
						}
					}
					else
					{
						continue;
					}


				}
				addLazy(expr <= mincut.m_graph_cutPoint - 1);
			}
			*/


				

			}
		}

		if (where == GRB_CB_MIPSOL)
		{
			vector<vector<int>> loop;
			m_Solution = getSolution(m_vars, m_num * m_num);
			m_findAllLoop(m_Solution, loop);
			int max_index, max_lenth = 0;
			
			if (loop.size() > 1)
			{
				for (int i = 0; i < loop.size(); i++)
				{
					if (loop[i].size() > max_lenth)
					{
						max_lenth = loop[i].size();
						max_index = i;
					}
				}
				GRBLinExpr expr = 0;

				for (auto j = loop[max_index].begin(); j != loop[max_index].end() - 1; j++)
				{
					expr += m_vars[(*j) * m_num + *(j + 1)];
				}
				expr += m_vars[loop[max_index][0] + loop[max_index].back() * m_num];
				addLazy(expr <= max_lenth - 1);
			}

		}
	}
    catch (const GRBException& erro)
	{
		std::cout << "Error: callback!" << std::endl;
    }


}
