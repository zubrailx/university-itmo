#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <utility>

#include "csv_parser.hpp"
#include "tree_printer.hpp"
#include "utility.hpp"

#pragma once

struct Node;

struct Rib {
	Node * target;
	long dist;
	Rib(Node * target, long dist) : target(target), dist(dist) {}
};

struct Node {
	std::string name;
	std::list<Rib> ribs;
	long dist = 0;
	int color = 0;
	Node() = default;
	explicit Node(std::string name) : name(std::move(name)) {}
};

std::vector<Node> get_node_list_sorted(
		const std::vector<std::tuple<std::string, std::string, int>> & data);

std::vector<std::string> get_sorted_unique_members(
		const std::vector<std::tuple<std::string, std::string, int>> & data);

Node * find_node(std::vector<Node> & vect, const std::string & target);
Node * find_node(std::vector<Node> & vect, std::string && target);
void print_node(const Node & node);
void print_nodes(const std::vector<Node> & nodes);
void clear_nodes(std::vector<Node> & nodes);
std::pair<Node *, Node *> get_entries(std::vector<Node> & nodes);

long bfs(std::vector<Node> & nodes, TreePrinter & tp);
long dfs(std::vector<Node> & nodes, TreePrinter & tp);
long dfs_limited(std::vector<Node> & nodes, TreePrinter & tp, int limit);
long iterative_deepening_search(std::vector<Node> & nodes, TreePrinter & tp, int limit);
long bidirectional_search(std::vector<Node> & nodes, TreePrinter & tp);
long dijkstra_search(std::vector<Node> & nodes, TreePrinter & tp);
long greedy_first_best_match_search(std::vector<Node> & nodes, TreePrinter & tp,
																		std::vector<Node> & datah);
long minimization_a_star_search(std::vector<Node> & nodes, TreePrinter & tp,
																std::vector<Node> & datah);
