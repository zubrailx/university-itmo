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
	long result;

	{
		print_line();
		std::cout << "BFS algorithm\n";
		result = bfs(nodes_sorted, tp);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);

		print_line();
		std::cout << "DFS algorithm\n";
		result = dfs(nodes_sorted, tp);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);

		print_line();
		std::cout << "DFS_limited algorithm\n";
		result = dfs_limited(nodes_sorted, tp, 4);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);

		print_line();
		std::cout << "Iterative_deepening algorithm\n";
		result = iterative_deepening_search(nodes_sorted, tp, 2);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);

		print_line();
		std::cout << "Bidirectional algorithm\n";
		result = bidirectional_search(nodes_sorted, tp);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);

		print_line();
		std::cout << "Dijkstra algorithm\n";
		result = dijkstra_search(nodes_sorted, tp);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);
	}

	{
		CSVParser parser2 = CSVParser("test/in.2");
		auto datah_tmp = parser2.parse_file<std::string, int>();
		std::vector<Node> datah(datah_tmp.size());
		size_t idx = 0;
		for (auto & it : datah_tmp) {
			datah[idx].name = std::get<0>(it);
			datah[idx++].dist = std::get<1>(it);
		}
		// sort data because find works as binary search
		std::sort(datah.begin(), datah.end(),
							[](Node & lsv, Node & rsv) { return lsv.name < rsv.name; });
		print_line();
		std::cout << "Greedy_first_best_match algorithm\n";
		result = greedy_first_best_match_search(nodes_sorted, tp, datah);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);

		print_line();
		std::cout << "Minimization_A_* algorithm\n";
		result = minimization_a_star_search(nodes_sorted, tp, datah);
		std::cout << "Result: " << result << std::endl;
		tp.print();
		tp.clear();
		clear_nodes(nodes_sorted);
	}

	print_line();
	// test_printer();
}
