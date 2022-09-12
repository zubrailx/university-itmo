#include <iostream>

#include "algorithms.hpp"
#include "csv_parser.hpp"
#include "test.hpp"
#include "tree_printer.hpp"
#include "utility.hpp"

// Variant: 2 (С.Петербург -> Житомир)
int main() {
	CSVParser parser("test/in.1");
	auto data = parser.parse_file<std::string, std::string, int>();
	std::vector<Node> nodes_sorted = get_node_list_sorted(data);
	TreePrinter tp(std::cout);
	bfs(nodes_sorted, tp);
	tp.print();
  tp.clear();
}
