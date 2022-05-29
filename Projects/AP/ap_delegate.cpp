#include "ap_delegate.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

#include "gurobi_c++.h"


using std::cout;
ap_delegate::ap_delegate()
{
}

ap_delegate::~ap_delegate()
{
	m_clear();
}

void ap_delegate::m_clear()
{

}

void ap_delegate::m_solve(string file_p2p, string file_v2p, string result)
{
	m_clear();
	std::fstream ifs(file_v2p);

	if (!ifs.is_open())
	{
		cout << "Error: Failed to open file!\n";
		return;
	}
  
    try
    {
        // 整数规划求解环境初始化
        GRBEnv* env = new GRBEnv();
        GRBModel* model = new GRBModel(*env);
        model->set(GRB_IntParam_LazyConstraints, 1);
        model->set(GRB_IntParam_PreCrush, 1);

        // 检测数据尺寸
        int place = 0, vehicle = 0;
        string line, seg;
        std::istringstream iss;

        std::getline(ifs, line);
        vehicle++;
        iss.str(line);
        while (std::getline(iss, seg, ' '))
        {
            place++;
        }
 
        while (std::getline(ifs, line))
        {
            vehicle++;
        }
   

        // 分配空间
        int rows = place + vehicle;
        GRBVar* vars = (GRBVar*)malloc((place + 1) * rows * sizeof(GRBVar));
        // 前 place 列表示地区到地区，接着的 vehicle 表示车到地区，最后一列地区到终点



        // 读取数据.同时创建变量
        ifs.seekg(0);
        for (int i = 0; i < vehicle; i++)
        {
            std::getline(ifs, line);
            iss.str(line);
            for (int j = 0; j < place; j++)
            {
                std::getline(iss, seg, ' ');
                vars[(i + place) * (place + 1) + j] = model->addVar(0.0, 1.0, atoi(seg.c_str()), GRB_BINARY, "V_" + std::to_string(i) + "_P_" + std::to_string(j));
            }
        }
        ifs.close();
        ifs.open(file_p2p);
        for (int i = 0; i < place; i++)
        {
            std::getline(ifs, line);
            iss.str(line);

            for (int j = 0; j < place; j++)
            {
                std::getline(iss, seg, ' ');
                if (i != j)
                {
                    vars[i * (place + 1) + j] = model->addVar(0.0, 1.0, atoi(seg.c_str()), GRB_BINARY, "P_" + std::to_string(i) + "_P_" + std::to_string(j));
                }
                else
                {
                    vars[i * (place + 1) + j] = model->addVar(0.0, 0.0, 1.0, GRB_BINARY, "P_" + std::to_string(i) + "_P_" + std::to_string(j));
                }

            }
        }
        ifs.close();
        for (int i = 0; i < place + vehicle; i++)
        {
            vars[i * (place + 1) + place] = model->addVar(0.0, 1.0, 0.0, GRB_BINARY, "P_" + std::to_string(i) + "_T");
        }



        // 添加约束
        GRBLinExpr expr = 0;
        for (int i = 0; i < place + vehicle; i++)
        {
            expr = 0;
            for (int j = 0; j <= place; j++)
            {
                expr += vars[i * (place + 1) + j];
            }
            model->addConstr(expr == 1, "Row_" + std::to_string(i));
        }

       
        for (int j = 0; j < place; j++)
        {
            expr = 0;
            for (int i = 0; i < place + vehicle; i++)
            {
                expr += vars[i * (place + 1) + j];
            }
            model->addConstr(expr == 1, "Col_" + std::to_string(j));
        }

        expr = 0;
        for (int i = 0; i < place + vehicle; i++)
        {
            expr += vars[i * (place + 1) + place];
        }
        model->addConstr(expr == vehicle, "Terminal");

        expr = 0;
        for (int i = 0; i < vehicle; i++)
        {
            for (int j = 0; j < place; j++)
            {
                expr += vars[(i + place) * (place + 1) + j];
            }
        }
        model->addConstr(expr == vehicle);



        // 求解模型
        model->optimize();

        // 输出可行解
        if (model->get(GRB_IntAttr_SolCount) > 0)
        {
            double* Sol = (double*)malloc((place + 1) * rows * sizeof(double));

            Sol = model->get(GRB_DoubleAttr_X, vars, (place + 1) * rows);

            std::cout << "\nSuccessfully found a solution!\n";

            std::cout << "\nPlace to place data load from: \n" << file_p2p << std::endl;
            std::cout << "\nVehicle to place data load from: \n" << file_v2p << std::endl;
            std::cout << "\nResult save to: \n" << result << std::endl;

            std::vector<int> rotine;
            int now;
            int next;

            std::ofstream ofs(result);

            for (int i = 0; i < vehicle; i++)
            {
               
                now = i + place;
                next = -1;

                while (next != place)
                {
                    for (int j = 0; j <= place; j++)
                    {
                        if (Sol[now * (place + 1) + j] >= 0.9)
                        {
                            next = j;
                            rotine.push_back(next);
                            now = next;
                            break;
                        }
                    }
                  
                }

                ofs << rotine.size() << " ";
                for (int number: rotine)
                {
                    ofs << number << " ";
                }
                rotine.clear();
               
            }


            ofs.close();
            free(Sol);
        }
        else
        {
            std::cout << "\nFailed to find any solution!";
        }
    }
    catch (const GRBException& erro)
    {
        cout << erro.getMessage();
    }
   
    return;
}
