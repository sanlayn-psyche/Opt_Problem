#pragma once

#include<string>
#include<vector>
#include<fstream>

#include "gurobi_c++.h"

using namespace std;

#define MAX(a,b) a > b ? a : b
#define MIN(a,b) a > b ? b : a

template<uint16_t n, typename T>
class Position
{
public:
    Position()
    {
        for (int i = 0; i < n; i++)
        {
            m_cord[i] = T();
        }
    }

    Position(vector<T> &v) 
    {
        for (int i = 0; i < MIN(v.size(), n); i++)
        {
            m_cord[i] = v[i];
        }
    }

    Position(T* ptr)
    {
        for (int i = 0; i < n; i++)
        {
            m_cord[i] = ptr[i];
        }
    }

    ~Position() {};

    T get_cord(int m)
    {
        m = m % n;
        return m_cord[m];
    }
    void set_cord(T value, int m)
    {
        m = m % n;
        m_cord[m] = value;
    }

    Position<n, T> operator + (Position<n, T> &p)
    {
        Position<n, T> R;
        for (int i = 0; i < n; i++)
        {
            R.m_cord[i] = m_cord[i] + p.m_cord[i];
        }
        return R;
    }

    Position<n, T> operator - (Position<n, T>& p)
    {
        Position<n, T> R;
        for (int i = 0; i < n; i++)
        {
            R.m_cord[i] = m_cord[i] - p.m_cord[i];
        }
        return R;
    }

    T norm()
    {
        T length = 0;
        for (size_t i = 0; i < n; i++)
        {
            length += m_cord[i]*m_cord[i];
        }
        return sqrt(length);
    }

protected:
    T m_cord[n];
};


class Pos: public Position<2, double>
{
public:
    Pos(double x1, double x2)
    {
        m_cord[0] = x1;
        m_cord[1] = x2;
    }
    Pos(double* x) : Position<2, double>(x) {};
    Pos(vector<double>& v) : Position<2, double>(v) {};
    ~Pos() {};
};

class TSPSolve
{
public:
    TSPSolve(const TSPSolve&) = delete;
    TSPSolve(TSPSolve&&) = delete;
    TSPSolve& operator=(const TSPSolve&) = delete;
    TSPSolve& operator=(TSPSolve&&) = delete;

    static TSPSolve& Instance()
    {
        static TSPSolve instance;
        return instance;
    }


    void m_clear();
    bool m_solve(string root, bool usercut = true);


private:
    TSPSolve() {};
    vector<Pos> m_position;

   

};

