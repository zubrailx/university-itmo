#include "csv_parser.hpp"

#include <iostream>

using std::cout;
using std::endl;


// Variant: 2 (С.Петербург -> Житомир)
int main() {
	CSVParser parser("test/in.1");
	auto data = parser.parse_file<std::string, std::string, int>();
	for (auto line : data) {
		auto [from, to, dist] = line;
		cout << from << to << dist << endl;
	}
}
