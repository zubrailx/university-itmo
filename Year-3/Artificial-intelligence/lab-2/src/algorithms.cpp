#include "algorithms.hpp"

#include <vector>

static const std::string START_POINT_NAME = "С.Петербург";
static const std::string END_POINT_NAME = "Житомир";

std::vector<std::string> get_sorted_unique_members(
		const std::vector<std::tuple<std::string, std::string, int>> & data) {
	std::vector<std::string> members;
	for (const auto & line : data) {
		members.push_back(std::get<0>(line));
		members.push_back(std::get<1>(line));
	}
	std::sort(members.begin(), members.end());
	members.erase(std::unique(members.begin(), members.end()), members.end());
	return members;
}

Node * find_node(std::vector<Node> & vect, const std::string & target) {
	int start, end, idx;
	start = 0, end = vect.size() - 1;
	while (start <= end) {
		idx = start + (end - start) / 2;
		if (vect[idx].name > target) {
			end = idx - 1;
		} else if (vect[idx].name < target) {
			start = idx + 1;
		} else {
			return &vect[idx];
		}
	}
	return nullptr;
}

std::vector<Node> get_node_list_sorted(
		const std::vector<std::tuple<std::string, std::string, int>> & data) {
	std::vector<Node> nodes_sorted;
	for (const auto & str : get_sorted_unique_members(data)) {
		nodes_sorted.emplace_back(str);
	}
	for (auto & [from, to, dist] : data) {
		auto node_from = find_node(nodes_sorted, from);
		auto node_to = find_node(nodes_sorted, to);
		node_from->ribs.emplace_back(node_to, dist);
		node_to->ribs.emplace_back(node_from, dist);
	}
	return nodes_sorted;
}

void print_node(const Node & node) {
	std::cout << node.name << ", " << node.dist << std::endl;
	for (auto it = node.ribs.begin(); it != node.ribs.end(); ++it) {
		std::cout << "  " << it->target->name << ", " << it->dist << std::endl;
	}
}

void print_nodes(const std::vector<Node> & nodes) {
	for (const auto & e : nodes) {
		print_node(e);
	}
}

void clear_nodes(std::vector<Node> & nodes) {
	for (auto & node : nodes) {
		node.dist = 0;
		node.color = 0;
	}
}

std::pair<Node *, Node *> get_entries(std::vector<Node> & nodes) {
	auto start = find_node(nodes, START_POINT_NAME);
	auto end = find_node(nodes, END_POINT_NAME);
	return std::make_pair(start, end);
}

std::string get_node_info(const Node * node) {
	return node->name + ", " + std::to_string(node->dist);
}

// поиск в ширину
long bfs(std::vector<Node> & nodes, TreePrinter & tp) {
	auto [start, end] = get_entries(nodes);
	std::queue<std::pair<Node *, std::vector<int>>> queue;
	start->color = 1;
	auto vct = tp.append(get_node_info(start));
	queue.push(std::make_pair(start, vct));
	while (!queue.empty()) {
		auto [v, vct] = queue.front();
		queue.pop();
		for (auto & e : v->ribs) {
			auto v2 = e.target;
			if (v2->color == 0) {
				v2->dist = v->dist + e.dist;
				v2->color = 1;
				auto vct2 = tp.insert_under(get_node_info(v2), vct);
				queue.push(std::make_pair(v2, vct2));
			}
			if (v2 == end) {
				return v2->dist;
			}
		}
	}
	return -1;
}

// поиск в глубину
static long dfs_helper(std::vector<Node> & nodes, Node * cur,
											 const Node * target, TreePrinter & tp) {
	cur->color = 1;
	tp.append(get_node_info(cur));
	if (cur == target) {
		return cur->dist;
	} else {
		tp.step_in();
		for (auto it = cur->ribs.begin(); it != cur->ribs.end(); ++it) {
			if (!it->target->color) {
				it->target->dist = cur->dist + it->dist;
				long ret = dfs_helper(nodes, it->target, target, tp);
				if (ret != -1) {
					return ret;
				}
			}
		}
		tp.step_out();
	}
	return -1;
}

long dfs(std::vector<Node> & nodes, TreePrinter & tp) {
	auto [start, end] = get_entries(nodes);
	return dfs_helper(nodes, start, end, tp);
}

// поиск с ограничением глубины
static long dfs_limited_helper(std::vector<Node> & nodes, Node * cur,
															 const Node * target, TreePrinter & tp, int limit,
															 int level) {
	if (level > limit) {
		return -1;
	}
	cur->color = 1;
	tp.append(get_node_info(cur));
	if (cur == target) {
		return cur->dist;
	} else {
		tp.step_in();
		for (auto it = cur->ribs.begin(); it != cur->ribs.end(); ++it) {
			if (!it->target->color) {
				it->target->dist = cur->dist + it->dist;
				long ret =
						dfs_limited_helper(nodes, it->target, target, tp, limit, level + 1);
				if (ret != -1) {
					return ret;
				}
			}
		}
		tp.step_out();
	}
	return -1;
}

long dfs_limited(std::vector<Node> & nodes, TreePrinter & tp, int limit) {
	auto [start, end] = get_entries(nodes);
	dfs_limited_helper(nodes, start, end, tp, limit, 0);
	return -1;
}

// поиск с итеративным углублением
long iterative_deepening_search(std::vector<Node> & nodes, TreePrinter & tp) {
	// auto [start, end] = get_entries(nodes);
	return -1;
}

// двунаправленный поиск
long bidirectional_search(std::vector<Node> & nodes, TreePrinter & tp) {
	// auto [start, end] = get_entries(nodes);
	return -1;
}

// жадный поиск по первому наилучшему соответствию
long greedy_first_best_match_search(std::vector<Node> & nodes,
																		TreePrinter & tp) {
	// auto [start, end] = get_entries(nodes);
	return -1;
}

// поиск методом минимизации суммарной оценки A*
long minimization_a_star_search(std::vector<Node> & nodes, TreePrinter & tp) {
	// auto [start, end] = get_entries(nodes);
	return -1;
}
