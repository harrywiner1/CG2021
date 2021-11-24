#include <algorithm>
#include <sstream>
#include "Utils.h"
#include <map>
#include "ModelTriangle.h"
#include <fstream>
#include <vector>
#include "CanvasPoint.h"
using namespace std;

std::vector<std::string> split(const std::string &line, char delimiter)
{
	auto haystack = line;
	std::vector<std::string> tokens;
	size_t pos;
	while ((pos = haystack.find(delimiter)) != std::string::npos)
	{
		tokens.push_back(haystack.substr(0, pos));
		haystack.erase(0, pos + 1);
	}
	// Push the remaining chars onto the vector
	tokens.push_back(haystack);
	return tokens;
}
