//
//  GraphCut.hpp
//  ImageCut
//
//  Created by 朱家明 on 2021/12/2.
//

#ifndef GraphCut_h
#define GraphCut_h

#include <stdio.h>
#include <vector>

#include <string.h>
#include <assert.h>

using std::vector;

namespace GT
{
	template<typename T>
	class Node;

	template<typename T>
	class Edge
	{
	public:
		Edge(T cap = 0, Node<T> *from = nullptr, Node<T>*to = nullptr)
		{
			m_cap = cap;
			m_node[0] = from;
			m_node[1] = to;
		}

		Node<T>* get_neibour(Node<T> *v)
		{
			if (v == m_node[0])
			{
				return m_node[1];
			}
			if (v == m_node[1])
			{
				return m_node[0];
			}
			return nullptr;
		}

		// 0 for from, 1 for to
		Node<T>* get_neibour(int index)
		{
			assert(index <= 1 && index >= 0);
			return m_node[index];
		}

		T get_res()
		{
			return m_res;
		}

		
		void act_addFlow(T flow)
		{
			m_node[0]->m_flow = m_node[0]->m_flow - flow;
			m_node[0]->m_res = m_node[0]->m_cap - m_node[0]->m_flow;

			m_node[1]->m_flow = m_node[1]->m_flow + flow;
			m_node[1]->m_res = m_node[1]->m_cap - m_node[1]->m_flow;

			m_flow = m_flow + flow;
			m_res = m_cap - m_flow;
		}
	
	private:
		T m_cap;
		T m_flow;
		T m_res;
		Node<T>* m_node[2];
	};

	template<typename T>
	class Node
	{
	public:
		Node(T cap)
		{
			m_cap = cap;
			m_height = 0;
		}

	private:
		bool m_ifVisited = false;

		// 0 for from, 1 for to
		vector<Edge<T>*> m_neibour[2];

	public:

		long m_height = 0;
		T m_cap;
		T m_res;
		T m_flow;

		T get_overflow()
		{
			return m_cap-m_flow;
		}

		bool if_visited(bool root_state)
		{
			if (m_ifVisited != root_state)
			{
				m_ifVisited = root_state;
				return false;
			}
			return true;
		}

		Edge<T>* act_addNeibour(T cap, Node& n)
		{
			Edge<T>* e = new Edge<T>(cap, this, &n);
			m_neibour[1].push_back(e);
			n.m_neibour[0].push_back(e);
			return e;
		}
		

		int act_push()
		{
			T minFlow;
			T resFlow;
			int counter = 0;

			for (int k = 0; k < 2; k++)
			{
				for (auto i = m_neibour[k].begin(); i != m_neibour[k].end(); i++)
				{
					if (m_flow - m_cap > 0)
					{
						if ((*i)->get_neibour(k)->m_height == m_height - 1)
						{
							resFlow = (*i)->get_res();

							minFlow = m_flow - m_cap > resFlow ? resFlow : m_flow - m_cap;

							(*i)->act_addFlow(minFlow);

							counter++;
						}
						else
						{
							continue;
						}
					}
					else
					{
						break;
					}
				}
			}
	
			return counter;
		}


		int act_relabel()
		{
			int counter = 0;
			if (m_flow - m_cap > 0)
			{
				long minH = 9999909;
				long tempH;
				Edge<T>* ptr = nullptr;

				for (int k = 0; k < 2; k++)
				{
					for (auto i = m_neibour[k].begin(); i != m_neibour[k].end(); i++)
					{
						tempH = (*i)->get_neibour(k)->m_height;
						if ((*i)->get_res() > 0 && 
							tempH >= m_height &&
							tempH < minH)
						{
							ptr = *i;
							minH = tempH;
						}
					}
				}

				if (ptr != nullptr)
				{
					m_height = minH + 1;
					counter++;
					act_push();

				}

				
			}
			else
			{
				return 0;
			}
			return counter;
	
		}


	};

}







#endif /* GraphCut_h */
