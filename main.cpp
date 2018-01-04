#include "ScriptUI.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::experimental::filesystem;

int main()
{
	/*
	std::ostringstream oss;
	auto path = fs::current_path();
	oss << path << "\\portraits\\";
	std::cout << oss.str();
	std::string x;
	std::cin >> x;
	oss << x;
	for (auto &p : fs::directory_iterator(oss.str()))
	{
		std::ostringstream poss;
		poss << p;
		int ps = poss.str().find(x) + x.size() + 1;
		auto px = poss.str().substr(ps);
		px = px.substr(0, px.find("."));
		std::cout << px << "\n";

		//std::cout << p << std::endl;
	}

	system("pause");
	*/

	ScriptUI sui;
	//sui.createWindow();

	sui.run();

	return 0;
}