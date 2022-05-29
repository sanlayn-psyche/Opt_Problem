//
//  GraphCut.cpp
//  ImageCut
//
//  Created by 朱家明 on 2021/12/2.
//

#include "GraphCut.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLOAT_ZERO 0.000001

template<typename T>
T min(T a, T b)
{
    return a > b ? b : a;

}

template<typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}


Node::Node():
    m_edge(0)
{
                      
}


Node::Node(int x, int y):
    Node()
{
    m_position[0] = x;
    m_position[1] = y;
}

int* Node::m_getPostion()
{
    return m_position;
}


void PushRelabel(Node* source, Node* terminal)
{
    vector<Node*> visitList;
    vector<Node*> graphNode;
    
    Node* ite = nullptr;
    Node* neibour = nullptr;
    bool m_flag;
 

    // 深度优先搜索找出所有能访问到的节点
    visitList.push_back(source);
    m_flag = source->m_ifVisited;
    while (visitList.size() > 0)
    {
        ite = visitList.back();
        visitList.pop_back();
        graphNode.push_back(ite);
        ite->m_ifVisited = !m_flag;

        for (auto i = ite->m_edge.begin(); i != ite->m_edge.end(); i++)
        {
            neibour = (*i)->m_getNeibour(ite);
            if (neibour != nullptr && neibour->m_ifVisited == m_flag)
            {
                visitList.push_back(neibour);
            }
        }
    }

    // 若未搜索到汇节点, 则直接结束
    if (terminal->m_ifVisited != m_flag)
    {
        return;
    }


    // 初始化设置, 高度及初始流. 注意, 要保证所有与源节点相连的边, 其 m_from 均指向源节点
    source->m_h = graphNode.size();
    for (auto i = source->m_edge.begin(); i != source->m_edge.end(); i++)
    {
        if ((*i)->m_cap > FLOAT_ZERO)
        {
            (*i)->m_flow = (*i)->m_cap;
            (*i)->m_to->m_exessFlow = (*i)->m_cap;
            source->m_exessFlow -= (*i)->m_cap;
        }
    }

    int ifRelabel = 1;
    int ifPush = 1;
    long MAXITEATION = 1000;
    long k = 0;
    while ((ifRelabel || ifPush) && k < MAXITEATION)
    {
        k++;
        ifPush = 0;
        ifRelabel = 0;
        for (auto i = graphNode.begin(); i != graphNode.end(); i++)
        {
            if ((*i)->m_exessFlow > FLOAT_ZERO)
            {
                for (auto j = (*i)->m_edge.begin(); j != (*i)->m_edge.end(); j++)
                {
                    ifPush += Push(*i, *j);
                }
            }
        }
        if (!ifPush)
        {
            for (auto i = graphNode.begin(); i != graphNode.end(); i++)
            {
                if ((*i)->m_exessFlow > FLOAT_ZERO)
                {
                    ifRelabel += Relabel(*i);
                }
            }
        }
    }

}

bool Push(Node* u, Edge *uv)
{
    double minFlow = 0.0;
    double resFlow = 0.0;

    if (uv->m_from == u)
    {
        if (u->m_h != uv->m_to->m_h + 1)
        {
            return false;
        }
        resFlow = uv->m_cap - uv->m_flow;
        minFlow = u->m_exessFlow > resFlow ? resFlow : u->m_exessFlow;
        uv->m_flow += minFlow;
        uv->m_to->m_exessFlow += minFlow;
        u->m_exessFlow -= minFlow;
        return true;
    }
    if (uv->m_to == u)
    {
        if (u->m_h != uv->m_from->m_h + 1)
        {
            return false;
        }
        resFlow = uv->m_cap + uv->m_flow;
        minFlow = u->m_exessFlow > resFlow ? resFlow : u->m_exessFlow;
        uv->m_flow -= minFlow;
        uv->m_from->m_exessFlow += minFlow;
        u->m_exessFlow -= minFlow;
        return true;
    }

    return false;
}

bool Relabel(Node* u)
{
    Node* minNei = nullptr;
    long minH = 9999909;
    Node* neibour = nullptr;
    double res = 0.0;
    auto minIndex = u->m_edge.begin();

    for (auto i = u->m_edge.begin(); i != u->m_edge.end(); i++)
    {
        
        if ((*i)->m_getRes(u) > FLOAT_ZERO)
        {
            neibour = (*i)->m_getNeibour(u);
            if (u->m_h <= neibour->m_h && neibour->m_h < minH)
            {
                minNei = neibour;
                minH = neibour->m_h;
                minIndex = i;
            }
        }
    }

    if (minNei != nullptr)
    {
        u->m_h = 1 + minH;
        Push(u, *minIndex);
        return true;
    }
    else
    {
        return false;
    }

}




