#include "algorithms.hpp"

static const std::string START_POINT_NAME = "С.Петербург";
static const std::string END_POINT_NAME = "Житомир";

std::vector<std::string> get_sorted_unique_members(
    const std::vector<std::tuple<std::string, std::string, int>> &data) {
  std::vector<std::string> members;
  for (const auto &line : data) {
    members.push_back(std::get<0>(line));
    members.push_back(std::get<1>(line));
  }
  std::sort(members.begin(), members.end());
  members.erase(std::unique(members.begin(), members.end()), members.end());
  return members;
}

Node *find_node(std::vector<Node> &vect, const std::string &target) {
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
    const std::vector<std::tuple<std::string, std::string, int>> &data) {
  std::vector<Node> nodes_sorted;
  for (const auto &str : get_sorted_unique_members(data)) {
    nodes_sorted.emplace_back(str);
  }
  for (auto &[from, to, dist] : data) {
    auto node_from = find_node(nodes_sorted, from);
    auto node_to = find_node(nodes_sorted, to);
    node_from->ribs.emplace_back(node_to, dist);
    node_to->ribs.emplace_back(node_from, dist);
  }
  return nodes_sorted;
}

void print_node(const Node &node) {
  std::cout << node.name << ", " << node.dist << std::endl;
  for (auto it = node.ribs.begin(); it != node.ribs.end(); ++it) {
    std::cout << "  " << it->target->name << ", " << it->dist << std::endl;
  }
}

void print_nodes(const std::vector<Node> &nodes) {
  for (const auto &e : nodes) {
    print_node(e);
  }
}

void clear_distances(std::vector<Node> &nodes) {
  for (auto &node : nodes)
    node.dist = 0;
}

std::pair<Node *, Node *> get_entries(std::vector<Node> &nodes) {
  auto start = find_node(nodes, START_POINT_NAME);
  auto end = find_node(nodes, END_POINT_NAME);
  return std::make_pair(start, end);
}

// поиск в ширину
void bfs(std::vector<Node> &nodes) {
  auto [start, end] = get_entries(nodes);
  std::queue<Node *> queue;
  queue.push(start);
  while (queue.empty()) {
  }
}

// поиск в глубину
void dfs(std::vector<Node> &nodes) {}

// поиск с ограничением глубины
void depth_limited_search(std::vector<Node> &nodes) {}

// поиск с итеративным углублением
void iterative_deepening_search(std::vector<Node> &nodes) {}

// двунаправленный поиск
void bidirectional_search(std::vector<Node> &nodes) {}

// жадный поиск по первому наилучшему соответствию
void greedy_first_best_match_search(std::vector<Node> &nodes) {}

// поиск методом минимизации суммарной оценки A*
void minimization_a_star_search(std::vector<Node> &nodes) {}
