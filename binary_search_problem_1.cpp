#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>

int minimum(const std::vector<int> & list) {
	assert(!list.empty());
	return std::accumulate(list.begin(), list.end(), list.back(),
			[](int value, int minimum) {
				if (value < minimum) return value;
				return minimum;
			});
}

int minimum(const std::vector<int> & list, int s, int e) {
	assert(s < e);
	if (s + 1 == e) return list[s]; // sorted list case
	if (list[s] < list[e-1]) return list[s]; // terminate
	int m = (s + e) / 2;  // mid index
	if (list[m] < list[e-1]) return minimum(list, m, e); // [m,e)
	return minimum(list, s, m); // [s, m)
}

int main(int argc, const char *argv[])
{
	std::cout << minimum({5,7,1,2,3}) << std::endl;
	std::cout << minimum({5,7,1,2,3}, 0, 5) << std::endl;
	return 0;
}
