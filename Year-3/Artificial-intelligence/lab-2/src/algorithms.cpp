#include "algorithms.hpp"

#include <limits>
#include <unordered_set>
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

Node * find_node(std::vector<Node> & vect, const Node * node) {
	return find_node(vect, node->name);
};

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

static std::string get_node_info(const Node * node) {
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
static long dfs_helper(std::vector<Node> & nodes, Node * cur, const Node * target,
											 TreePrinter & tp) {
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
				long ret = dfs_limited_helper(nodes, it->target, target, tp, limit, level + 1);
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
	return dfs_limited_helper(nodes, start, end, tp, limit, 0);
}

static long iterative_deepening_search_helper(
		std::vector<Node> & nodes, std::queue<std::pair<Node *, std::vector<int>>> & queue,
		Node * cur, const Node * target, TreePrinter & tp, std::vector<int> tp_pos,
		int limit, int level) {
	if (level == limit) {
		queue.push(std::make_pair(cur, tp_pos));
	} else if (level > limit) {
		return -1;
	}
	cur->color = 1;
	auto tp_chpos = tp.insert_under(get_node_info(cur), tp_pos);
	if (cur == target) {
		return cur->dist;
	} else {
		for (auto it = cur->ribs.begin(); it != cur->ribs.end(); ++it) {
			if (!it->target->color) {
				it->target->dist = cur->dist + it->dist;
				long ret = iterative_deepening_search_helper(nodes, queue, it->target, target,
																										 tp, tp_chpos, limit, level + 1);
				if (ret != -1) {
					return ret;
				}
			}
		}
	}
	return -1;
}

// поиск с итеративным углублением
long iterative_deepening_search(std::vector<Node> & nodes, TreePrinter & tp,
																int limit) {
	auto [start, end] = get_entries(nodes);
	std::queue<std::pair<Node *, std::vector<int>>> queue;
	queue.push(std::make_pair(start, std::vector(0, 0)));
	int result = -1;
	while (!queue.empty() && result == -1) {
		auto pair = queue.front();
		queue.pop();
		result = iterative_deepening_search_helper(nodes, queue, pair.first, end, tp,
																							 pair.second, limit, 0);
		++limit;
	}
	return result;
}

static long bidirectional_search_helper(std::vector<Node> & nodes, Node * start,
																				Node * target, TreePrinter & tp) {
	// tuple<Node, dist, tp_stack>
	std::list<std::tuple<Node *, long, std::vector<int>>> s_queue;
	s_queue.push_back(std::tuple(start, start->dist, tp.append(get_node_info(start))));
	std::list<std::tuple<Node *, long, std::vector<int>>> t_queue;
	t_queue.push_back(std::tuple(target, target->dist, tp.append(get_node_info(target))));
	int s_vcl = s_queue.size();// vertexes from current level
	int s_vnl;								 // vertexes from next level
	int t_vcl = t_queue.size();
	int t_vnl;
	while (s_vcl && t_vcl) {
		s_vnl = 0;
		t_vnl = 0;
		// process all vertexes from current level from start
		for (; s_vcl > 0; --s_vcl) {
			auto [v, vd, vct] = s_queue.front();
			s_queue.pop_front();
			for (auto it = v->ribs.begin(); it != v->ribs.end(); ++it) {
				if (it->target->color == 0) {
					++s_vnl;
					long dist = it->dist + vd;
					auto vct2 =
							tp.insert_under(it->target->name + ", " + std::to_string(dist), vct);
					it->target->color = 1;
					s_queue.push_back(std::tuple(it->target, dist, vct2));
				}
			}
		}
		// process all vertexes from current level from target
		for (; t_vcl > 0; --t_vcl) {
			auto [v, vd, vct] = t_queue.front();
			t_queue.pop_front();
			for (auto it = v->ribs.begin(); it != v->ribs.end(); ++it) {
				if (it->target->color == 0 || it->target->color == 1) {
					++t_vnl;
					long dist = it->dist + vd;
					auto vct2 =
							tp.insert_under(it->target->name + ", " + std::to_string(dist), vct);
					it->target->color = 2;
					t_queue.push_back(std::tuple(it->target, it->dist + vd, vct2));
				}
			}
		}
		s_vcl = s_vnl;
		t_vcl = t_vnl;
		// check for intersections and find minimum distance
		std::unordered_map<Node *, long> s_set;
		bool found = false;
		long min_dist = LONG_MAX;
		for (auto it = s_queue.begin(); s_vnl != 0; --s_vnl, ++it) {
			s_set[std::get<0>(*it)] = std::get<1>(*it);
		}
		for (auto it = t_queue.begin(); t_vnl != 0; --t_vnl, ++it) {
			// found
			if (auto its = s_set.find(std::get<0>(*it)); its != s_set.end()) {
				found = true;
				min_dist = std::min(std::get<1>(*it) + its->second, min_dist);
			}
		}
		if (found) {
			return min_dist;
		}
	}
	return -1;
}

// двунаправленный поиск
long bidirectional_search(std::vector<Node> & nodes, TreePrinter & tp) {
	auto [start, end] = get_entries(nodes);
	return bidirectional_search_helper(nodes, start, end, tp);
}

// алгоритм дейкстры
long dijkstra_search(std::vector<Node> & nodes, TreePrinter & tp) {
	auto [start, target] = get_entries(nodes);
	using t_node_vint = std::pair<Node *, std::vector<int>>;
	auto cmp = [](t_node_vint lsv, t_node_vint rsv) {
		return lsv.first->dist > rsv.first->dist;
	};
	std::priority_queue<t_node_vint, std::vector<t_node_vint>, decltype(cmp)> pqueue;
	pqueue.emplace(start, tp.append(get_node_info(start)));
	while (!pqueue.empty()) {
		auto [v, vct] = pqueue.top();
		pqueue.pop();
		for (auto it = v->ribs.begin(); it != v->ribs.end(); ++it) {
			auto v2 = it->target;
			if (v2->color == 0) {
				v2->color = 1;
				v2->dist = it->dist + v->dist;
				auto vct2 = tp.insert_under(get_node_info(v2), vct);
				if (v2 == target) {
					return v2->dist;
				} else {
					pqueue.emplace(v2, vct2);
				}
			}
		}
	}
	return -1;
}

long greedy_first_best_match_search(std::vector<Node> & nodes, TreePrinter & tp,
																		std::vector<Node> & hdist) {
	auto [start, target] = get_entries(nodes);
	using t_node_vint = std::pair<Node *, std::vector<int>>;

	auto fnode_dist_str = [&hdist](Node * node) {
		return std::to_string(find_node(hdist, node->name)->dist);
	};
	auto cmp = [&hdist](t_node_vint lsv, t_node_vint rsv) {
		return find_node(hdist, lsv.first->name)->dist >
					 find_node(hdist, rsv.first->name)->dist;
	};
	std::priority_queue<t_node_vint, std::vector<t_node_vint>, decltype(cmp)> pqueue(cmp);
	pqueue.emplace(start, tp.append(get_node_info(start) + ", " + fnode_dist_str(start)));

	while (!pqueue.empty()) {
		auto [v, vct] = pqueue.top();
		pqueue.pop();
		for (auto it = v->ribs.begin(); it != v->ribs.end(); ++it) {
			auto v2 = it->target;
			if (v2->color == 0) {
				v2->color = 1;
				v2->dist = it->dist + v->dist;
				auto vct2 = tp.insert_under(get_node_info(v2), vct);
				if (v2 == target) {
					return v2->dist;
				} else {
					pqueue.emplace(v2, vct2);
				}
			}
		}
	}
	return -1;
}

// поиск методом минимизации суммарной оценки A*
long minimization_a_star_search(std::vector<Node> & nodes, TreePrinter & tp,
																std::vector<Node> & hdist) {
	auto [start, target] = get_entries(nodes);
	using t_node_vint = std::tuple<Node *, std::vector<int>>;

	auto fnode_dist_str = [&hdist](Node * node) -> std::string {
		return std::to_string(find_node(hdist, node->name)->dist);
	};
	auto cmp = [&hdist](t_node_vint lsv, t_node_vint rsv) {
		return std::get<0>(lsv)->dist + find_node(hdist, std::get<0>(lsv)) >
					 std::get<0>(rsv)->dist + find_node(hdist, std::get<0>(rsv));
	};
	std::priority_queue<t_node_vint, std::vector<t_node_vint>, decltype(cmp)> openq(cmp);
	start->color = 1;
	openq.emplace(start, tp.append(get_node_info(start)));
	while (!openq.empty()) {
		auto [v, vct] = openq.top();
		openq.pop();
		v->color = 2;
		for (auto it = v->ribs.begin(); it != v->ribs.end(); ++it) {
			auto & v2 = it->target;
			if (v2->color == 0) {
				v2->dist = v->dist + it->dist;
				v2->color = 1;
				auto vct2 = tp.insert_under(get_node_info(v2) + ", " + fnode_dist_str(v2), vct);
				openq.emplace(v2, vct2);
				if (v2 == target) {
					return v2->dist;
				}
			}
		}
	}
	return -1;
}
