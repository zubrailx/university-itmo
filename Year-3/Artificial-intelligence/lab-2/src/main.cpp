#include <iostream>

#include "csv_parser.hpp"
#include "utility.hpp"

using std::cout;
using std::endl;

// Variant: 2 (С.Петербург -> Житомир)
int main() {
	CSVParser parser("test/in.1");
	auto data = parser.parse_file<std::string, std::string, int>();
	for (const auto & line : data) {
		const auto & [from, to, dist] = line;
		cout << from << to << dist << endl;
	}
}
