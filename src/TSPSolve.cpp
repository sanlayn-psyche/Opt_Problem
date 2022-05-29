#include "TSPSolve.h"
#include "RuntimeCut.h"
#include <iostream>
#include "output.h" 


void TSPSolve::m_clear()
{
    m_position.clear();
}



bool TSPSolve::m_solve(string root, bool usercut)
{
    m_clear();
    ifstream ifs(root);
    if (!ifs.is_open())
    {
        std::cout << "Error: failed to open data file!" << endl;
        return false;
    }

    double x, y;
    while (ifs >> x >> y)
    {
        m_position.push_back(Pos(x, y));
    }
    ifs.close();


    try
    {
        GRBEnv* env = new GRBEnv();
        GRBModel *model = new GRBModel(*env);

        GRBVar* vars = (GRBVar*)malloc(m_position.size() * m_position.size() * sizeof(GRBVar));
    
        model->set(GRB_IntParam_LazyConstraints, 1);
        model->set(GRB_IntParam_PreCrush, 1);

        GRBLinExpr expr = 0;
        for (int i = 0; i < m_position.size(); i++)
        {
            expr = 0;
            for (int j = 0; j < m_position.size(); j++)
            {
                if (i == j)

                {
                    vars[i * m_position.size() + j] = model->addVar(0.0, 0.0, 100.0, GRB_BINARY, "Node_" + to_string(i));
                }
                else
                {
                    if (i < j)
                    {
                        vars[i * m_position.size() + j] = model->addVar(0.0, 1.0, (m_position[i] - m_position[j]).norm(), GRB_BINARY, "Edge_" + to_string(i) + "_with_" + to_string(j));
                    }
                    else
                    {
                        vars[i * m_position.size() + j] = vars[j * m_position.size() + i];
                    }
                }
                expr += vars[i * m_position.size() + j];
            }
            model->addConstr(expr == 2, "Node_" + to_string(i) + "_deg");
        }
        RuntimeCut cb(vars, RuntimeCut::RC_CUTMODE_USER, m_position.size());
        if (!usercut)
        {
            cb.set_mode(RuntimeCut::RC_CUTMODE_LAZY);
        }
    
        model->setCallback(&cb);
        model->optimize();


        
       if (model->get(GRB_IntAttr_SolCount) > 0)
       {
            double *Sol = (double *)malloc(m_position.size() * m_position.size()*sizeof(double));
            vector<int> connect;

            Sol = model->get(GRB_DoubleAttr_X, vars, m_position.size() * m_position.size());
         
            for (int i = 0; i < m_position.size(); i++)
            {
       
                for (int j = i + 1; j < m_position.size(); j++)
                {
                    if (Sol[i* m_position.size() + j] >= 0.8)
                    {
                        connect.push_back(i);
                        connect.push_back(j);
                    }
                }
       
            }
            std::cout << "\nSuccessfully find a solution!";
            ot::print(connect, string(root.begin(),root.end() - 4) + "_result.txt");
            std::cout << "\nData File: " << root << endl;
            if (usercut)
            {
                std::cout << "User Cut" << endl;
            }
            else
            {
                std::cout << "Lazy Cut" << endl;
            }
       }
      

    }
    catch (const GRBException &erro)
    {
        cout << erro.getMessage() << endl;
    }
  
    return true;
}
