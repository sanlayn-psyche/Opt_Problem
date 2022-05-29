#include <fstream>
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::cout;

namespace ot
{
	template<typename T>
	void print(vector<T> vec, string path = " ")
	{
		std::ofstream ofs(path);
		if (!ofs.is_open())
		{
			cout << "Error: Failed to open file!\n";
			for (T i : vec)
			{
				cout << i << " ";
			}
			cout << "\n";
			return;
		}

		for (T i:vec)
		{
			ofs << i << " ";
		}
		ofs << "\n";
		ofs.close();
	}

	template<typename T>
	void print(T *vec, int n, string path = " ")
	{
		std::ofstream ofs(path);
		if (!ofs.is_open())
		{
			cout << "Error: Failed to open file!\n";
			for (int i = 0; i < n; i++)
			{
				cout << vec[i] << " ";
			}
			cout << "\n";
			return;
		}

		for (int i = 0; i < n; i++)
		{
			ofs << vec[i] << " ";
		}
		ofs << "\n";
		ofs.close();
		
	}

}