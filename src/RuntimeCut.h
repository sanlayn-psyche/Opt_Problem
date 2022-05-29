#pragma once

#include "gurobi_c++.h"
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

template<typename T>
class MinCut
{
public: 
	T* m_data;
    T m_minCut;

    int m_num;

    vector<int>::iterator m_graph_end;
    int m_graph_cutPoint;
    vector<int> m_graph;
    

    vector<pair<T, int>> m_heap;


    vector<int> depart_time;
    vector<int> depart_parent_map;


    vector<int> depart_time_best;
    vector<int> depart_parent_map_best;

    int m_minCut_leaf;



	MinCut(int num)
	{
        m_num = num;
		m_data = (T*)malloc(m_num * m_num * sizeof(T));
        m_graph.resize(m_num);
        m_graph_end = m_graph.end();
        m_heap.resize(m_num);
        depart_time.resize(m_num);
        depart_parent_map.resize(m_num);
        depart_time_best.resize(m_num);
        depart_parent_map_best.resize(m_num);

        m_refresh_departSet();

	}

    ~MinCut()
    {
        free(m_data);
        m_graph.clear();
        m_heap.clear();
        depart_time.clear();
        depart_parent_map.clear();
        depart_time_best.clear();
        depart_parent_map_best.clear();
    }

    void m_update_departSet()
    {
        for (int i = 0; i < m_num; i++)
        {
            depart_time_best[i] = depart_time[i];
            depart_parent_map_best[i] = depart_parent_map[i];
        }
    }
    void m_refresh_departSet()
    {

        for (int i = 0; i < m_num; i++)
        {
            depart_time[i] = 0;
            depart_time_best[i] = 0;
            depart_parent_map[i] = i;
            depart_parent_map_best[i] = i;
        }
    }

    int m_findRoot(int x, vector<int> &pmap)
    {
        if (pmap[x] != x) 
        {
            pmap[x] = m_findRoot(pmap[x], pmap);
        }

        return pmap[x];
    }

    void m_combine(int x1, int x2, vector<int>& pmap, vector<int>&time)
    {
        int root1 = m_findRoot(x1, pmap);
        int root2 = m_findRoot(x2, pmap);
        

        if (root1 == root2)
        {
            return;
        }


        if (time[x1] < time[x2])
        {
            pmap[x1] = x2;
        }
        else if (time[x1] > time[x2])
        {
            pmap[x2] = x1;
        }
        else 
        {
            pmap[x2] = x1;
            time[x1] = time[x1] + 1;
        }
    }


	T get_minCut()
	{
        
        m_minCut = T(m_num);
        for (int i = 0; i < m_num; i++)
        {
            m_graph[i] = i;
        }
        m_graph_end = m_graph.end();

        m_refresh_departSet();
      
        int index, temp_index;
        while (m_graph_end != m_graph.begin())
        {
            index = m_findACut();
            for (auto i = m_graph.begin(); i != m_graph_end; i++)
            {
                if (*i == index)
                {
                    temp_index = *(m_graph_end - 1);
                    *(m_graph_end - 1) = *i;
                    *i = temp_index;
                    break;
                }
            }
            m_graph_end--;
        }

 
        int temp;
        int p1 = 0, p2 = m_num - 1;
        while (p1 < p2)
        {
            if (m_findRoot(m_graph[p1], depart_parent_map_best) == m_findRoot(m_minCut_leaf, depart_parent_map_best))
            {
                p1++;
            }
            if (m_findRoot(m_graph[p2], depart_parent_map_best) != m_findRoot(m_minCut_leaf, depart_parent_map_best))
            {
                p2--;
            }
            if (p1 < p2)
            {
                temp = m_graph[p1];
                m_graph[p1] = m_graph[p2];
                m_graph[p2] = temp;
                p1++;
                p2--;
            }
        }
        m_graph_cutPoint = p2;

     
        return m_minCut;
        

     /*
        int  dis[N];
        bool vis[N];
        int n, m;

        
        int i, j;
        int res = 99999;


        for (i = 0; i < n; i++)
        {
            m_graph[i] = i;
        }
        m_graph_end = m_graph.end();
            
        while (m_graph_end != m_graph.begin())
        {
            int k = 1, pre = 0;
            for (i = 1; i < n; i++) 
            {
                dis[m_graph[i]] = m_data[m_graph[0] * m_num + m_graph[i]];
                if (dis[m_graph[i]] > dis[m_graph[k]])
                {
                    k = i;
                }
            }
            memset(vis, 0, sizeof(vis));
            vis[m_graph[0]] = true;
            for (i = 1; i < n; i++) 
            {
                if (i == n - 1) 
                {
                    res = res > dis[m_graph[k]] ? dis[m_graph[k]] : res;
                    for (j = 0; j < n; j++) {
                        m_data[m_graph[pre]*m_num + m_graph[j]] += m_data[m_graph[j] * m_num + m_graph[k]];
                        m_data[m_graph[j] * m_num + m_graph[pre]] += m_data[m_graph[j] * m_num + m_graph[k]];
                    }
                    m_graph[k] = m_graph[--n];
                }
                vis[m_graph[k]] = true;
                pre = k;
                k = -1;
                for (j = 1; j < n; j++)
                {
                    if (!vis[m_graph[j]])
                    {
                        dis[m_graph[j]] += m_data[m_graph[pre] * m_num + m_graph[j]];
                        if (k == -1 || dis[m_graph[k]] < dis[m_graph[j]])
                            k = j;
                    }
                }
            }
        }
        return res;
        */
	}


    int m_findACut()
    {
        for (auto i = m_graph.begin(); i != m_graph_end; i++)
        {
            m_heap[i - m_graph.begin()].first = 0;
            m_heap[i - m_graph.begin()].second = *i;
        }
        auto heap_end = m_heap.begin() + (m_graph_end - m_graph.begin()) - 1;

        for (auto i = m_heap.begin(); i != heap_end; i++)
        {
            i->first += m_data[m_num * heap_end->second + i->second];
        }

     
        double min_cut_local = 0.0;

        pair<T, int> temp;
        while (heap_end !=  m_heap.begin())
        {
    
            make_heap(m_heap.begin(), heap_end);

            temp = m_heap.front();
            m_heap[0] = *heap_end;
            *heap_end = temp;

            min_cut_local = heap_end->first;

            for (auto i = m_heap.begin(); i != heap_end; i++)
            {
                i->first += m_data[m_num * heap_end->second + i->second];
            }
 
            heap_end--;
        }

        int s = m_heap[1].second;
        int last = m_heap[0].second;

        if (min_cut_local < m_minCut)
        {
            m_minCut = min_cut_local;
            m_update_departSet();
            m_minCut_leaf = last;
        }

        m_combine(s, last, depart_parent_map, depart_time);
  
        for (auto i = m_heap.begin(); i !=  m_heap.begin() + (m_graph_end - m_graph.begin()) - 1; i++)
        {
            m_data[m_num * i->second + s] += m_data[m_num * i->second + last];
            m_data[m_num * s + i->second] = m_data[m_num * i->second + s];
        }

        return last;
    }


};


class RuntimeCut : public GRBCallback
{
public:
	enum Mode
	{
		RC_CUTMODE_LAZY,
		RC_CUTMODE_USER
	};

	GRBVar* m_vars;
	
	RuntimeCut(GRBVar* vars, Mode mode, int n);
    ~RuntimeCut();

    void set_mode(Mode mode);
private:
 
    void m_findAllLoop(double* sol, vector<vector<int>> &loop, double gap = 0.9);

	Mode m_mode = RC_CUTMODE_LAZY;
    int m_num;
	void callback();
    double* m_Solution;

};

