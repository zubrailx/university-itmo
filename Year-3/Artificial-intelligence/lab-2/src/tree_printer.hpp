#include <fstream>
#include <memory>
#include <utility>
#include <vector>

#pragma once

class TreePrinter {
private:
	int m_last = 0;
	int m_level = 0;
	std::ostream & m_stream;
	const std::string M_TREE_SIGNS[6] = {"├", "─", "│", " ", "└", "┐"};

	struct Tree {
		std::vector<std::pair<std::string, std::unique_ptr<Tree>>> arr;
	};
	std::unique_ptr<Tree> root;

public:
	TreePrinter(std::ostream & stream);

	void step_in(int step = 1);
	void step_out(int step = 1);
	void set_level(int level);

	std::vector<int> append(const std::string & str);
	std::vector<int> insert_under(const std::string & str, std::vector<int> pos);

	void print();

	void clear();

private:
	void print_helper(const Tree * subtree, int level, std::vector<int> & stack);

	void print_full_line(const Tree * subtree, int idx, std::vector<int> & stack,
											 int level);

	void print_line(const Tree * root, const std::vector<int> & stack, int level);
};
