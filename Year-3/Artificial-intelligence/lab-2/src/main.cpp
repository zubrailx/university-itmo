#include <iostream>

#include "csv_parser.hpp"
#include "utility.hpp"
#include "tree_printer.hpp"

void test_tree();

// Variant: 2 (С.Петербург -> Житомир)
int main() {
	// CSVParser parser("test/in.1");
	// auto data = parser.parse_file<std::string, std::string, int>();
	// for (const auto & line : data) {
	// 	const auto & [from, to, dist] = line;
	// 	std::cout << from << to << dist << std::endl;
	// }
  test_tree();
}

void test_tree() {
  TreePrinter tp(std::cout);
  tp.step_in(2);
  tp.append("level-2(1)");
  tp.append("level-2(2)");
  tp.step_out();
  tp.append("level-1");
  tp.step_in(4);
  tp.append("level-3");
  tp.step_out();
  tp.append("level-2");
  tp.step_out();
  tp.append("level-1");
  tp.step_out();
  auto v = tp.append("level-0");
  int res = tp.insert_under("level-under-0", v);
  res = tp.insert_under("level-under-0", v);
  res = tp.insert_under("level-under-0", v);
  res = tp.insert_under("level-under-0", v);
  std::cout << res << std::endl;
  tp.print();
}
