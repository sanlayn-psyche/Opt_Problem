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

class Node;

struct Edge
{
    double m_cap = 0.0;
    double m_flow = 0.0;
    Node* m_from = nullptr;
    Node* m_to = nullptr;

	Node* m_getNeibour(Node* v)
	{
		if (v == m_from)
		{
			return m_to;
		}
		if (v == m_to)
		{
			return m_from;
		}
		return nullptr;
	}

	double m_getRes(Node* u)
	{
		if (u == m_from)
		{
			return m_cap - m_flow;
		}
		if (u == m_to)
		{
			return m_cap + m_flow;
		}
		return 0.0;
	}

};

class Node
{
public:
    Node();
    Node(int x, int y);
    
public:
	
    long m_h = 0;
    vector<Edge *> m_edge;
    double m_exessFlow = 0.0;
    bool m_ifVisited = false;
	
	int* m_getPostion();

private:
	// 节点的位置
	int m_position[2];
};


void PushRelabel(Node* source, Node* terminal);
bool Push(Node* u, Edge* e);
bool Relabel(Node* u);





#endif /* GraphCut_h */
