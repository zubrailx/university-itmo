#include <fstream>
#include <memory>
#include <utility>
#include <vector>

class TreePrinter {
private:
  int m_last = 0;
  int m_level = 0;
  std::ostream &m_stream;
  const std::string M_TREE_SIGNS[6] = {"├", "─", "│", " ", "└", "┐"};

  struct Tree {
    std::vector<std::pair<std::string, std::unique_ptr<Tree>>> arr;
  };
  std::unique_ptr<Tree> root;

public:
  TreePrinter(std::ostream &stream) : m_stream(stream) {
    root = std::make_unique<Tree>();
  }

  void step_in(int step = 1) { m_level += step; }

  void step_out(int step = 1) {
    m_level -= step;
    m_level = std::max(0, m_level);
  }

  std::vector<int> append(const std::string &str) {
    std::vector<int> stack;
    auto *arr = &root.get()->arr;
    for (int i = 0; i < m_level && i < m_last; ++i) {
      // invariant - size > 0
      int offset = arr->size() - 1;
      stack.push_back(offset);
      arr = &(*arr)[offset].second->arr;
    }

    if (m_level <= m_last) {
      stack.push_back(arr->size());
      arr->emplace_back(str, new Tree);
    } else {
      int idx = m_last;

      if (arr->empty()) { // in case parent is NILL
        stack.push_back(0);
        arr = &arr->emplace_back(M_TREE_SIGNS[5], std::make_unique<Tree>())
                   .second->arr;
        idx += 1;
      }
      for (; idx < m_level; ++idx) {
        stack.push_back(arr->size());
        arr = &arr->emplace_back(M_TREE_SIGNS[5], std::make_unique<Tree>())
                   .second->arr;
      }
      stack.push_back(arr->size());
      arr->emplace_back(str, new Tree);
      m_last = idx;
    }
    return stack;
  }

  int insert_under(const std::string &str, std::vector<int> &pos) {
    auto *arr = &root.get()->arr;
    for (size_t i = 0; i < pos.size(); ++i) {
      arr = &(*arr)[pos[i]].second->arr;
    }
    arr->emplace_back(str, std::make_unique<Tree>());
    return arr->size() - 1;
  }

  void print() {
    std::vector<int> helper;
    print_helper(root.get(), 0, helper);
  }

private:
  void print_helper(const Tree *subtree, int level, std::vector<int> &stack) {
    stack.push_back(0);
    const auto &arr = subtree->arr;
    if (arr.size()) {
      // Process first
      print_full_line(subtree, 0, stack, level);
      // Process [1; N - 2]
      for (size_t i = 1; i < arr.size() - 1; ++i) {
        print_full_line(subtree, i, stack, level);
      }
      // Process last
      if (arr.size() > 1) {
        print_full_line(subtree, arr.size() - 1, stack, level);
      }
    }
    stack.pop_back();
  }

  void print_full_line(const Tree *subtree, int idx, std::vector<int> &stack,
                       int level) {
    stack[level] = idx;
    print_line(root.get(), stack, level);
    m_stream << subtree->arr[idx].first << std::endl;
    print_helper(subtree->arr[idx].second.get(), level + 1, stack);
  }

  void print_line(const Tree *root, const std::vector<int> &stack, int level) {
    auto *arr = &root->arr;
    for (int i = 0; i < level; ++i) {
      if (arr->size() - stack[i] - 1) {
        m_stream << M_TREE_SIGNS[2] << M_TREE_SIGNS[3] << M_TREE_SIGNS[3];
      } else {
        m_stream << M_TREE_SIGNS[3] << M_TREE_SIGNS[3] << M_TREE_SIGNS[3];
      }
      arr = &(*arr)[stack[i]].second->arr;
    }
    if (arr->size() > 1 && arr->size() - stack[level] - 1) {
      m_stream << M_TREE_SIGNS[0] << M_TREE_SIGNS[1] << M_TREE_SIGNS[1];
    } else {
      m_stream << M_TREE_SIGNS[4] << M_TREE_SIGNS[1] << M_TREE_SIGNS[1];
    }
  }
};
