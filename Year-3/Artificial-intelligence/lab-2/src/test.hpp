#include <iostream>

#include "tree_printer.hpp"

#pragma once

inline void test_printer() {
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
	tp.insert_under("level-under-0", v);
	tp.insert_under("level-under-0", v);
	tp.insert_under("level-under-0", v);
	tp.insert_under("level-under-0", v);
	tp.print();
	tp.clear();
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
	v = tp.append("level-0");
	tp.insert_under("level-under-0", v);
	tp.insert_under("level-under-0", v);
	tp.insert_under("level-under-0", v);
	tp.insert_under("level-under-0", v);
	tp.print();
}
