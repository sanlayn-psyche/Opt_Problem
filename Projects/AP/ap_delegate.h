#pragma once



#include <string>
using std::string;
class ap_delegate
{
public:


	ap_delegate();
	~ap_delegate();

	void m_clear();
	void m_solve(string file_p2p = "../../Data/AP/palace2place.txt", 
		string file_v2p = "../../Data/AP/vehicle2place.txt", 
		string result = "../../Data/AP/result.txt");

	



};

