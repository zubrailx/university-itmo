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
  Node *target;
  long dist;
  Rib(Node *target, long dist) : target(target), dist(dist) {}
};

struct Node {
  std::string name;
  std::list<Rib> ribs;
  long dist = 0;
  explicit Node(std::string name) : name(std::move(name)) {}
};

std::vector<Node> get_node_list_sorted(
    const std::vector<std::tuple<std::string, std::string, int>> &data);

std::vector<std::string> get_sorted_unique_members(
    const std::vector<std::tuple<std::string, std::string, int>> &data);

Node *find_node(std::vector<Node> &vect, const std::string &target);
void print_node(const Node &node);
void print_nodes(const std::vector<Node> &nodes);
void clear_distances(std::vector<Node> &nodes);
std::pair<Node *, Node *> get_entries(std::vector<Node> &nodes);

void bfs(std::vector<Node> &nodes);
void dfs(std::vector<Node> &nodes);
void depth_limited_search(std::vector<Node> &nodes);
void iterative_deepening_search(std::vector<Node> &nodes);
void bidirectional_search(std::vector<Node> &nodes);
void greedy_first_best_match_search(std::vector<Node> &nodes);
void minimization_a_star_search(std::vector<Node> &nodes);
