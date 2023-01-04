#include <qpg/qpg.hpp>

int main() {
  std::string buf;
  std::string line;
  std::cout << "> ";
  while(getline(std::cin, line)) {
    buf.append(line);
    buf.append("\n");
    if (line.find(';') != std::string::npos) {
      AstWrapper ast_wr;
      int code = parse_input(buf, ast_wr);
      if (code) {
        std::cout << "ret_code: " << code << std::endl;
      } else {
        std::cout << ast_wr.list->repr() << std::endl;
      }
      buf.clear();
      std::cout << "> ";
    }
  }
  // auto ast =parse_input("SELECT * FROM tab WHERE ( \"val\" < table1.col ) or ( false and true );");
  // parse_input("SELECT * FROM (select * from (select table4 from table5 cross_join table6) as table7 where col = 4) AS table3 CROSS_JOIN table2;");
  // auto ast =parse_input("UPDATE table1 set col1 = 'val' where col1 = false and col2 = true;");
  // auto ast = parse_input("DELETE FROM table1 where col1 = 'val';");
  // auto ast = parse_input("INSERT INTO table1 VALUES ('val', 'val2', -4, -4.2, false);");
  // auto ast = parse_input("DROP TABLE idkwhat;");
  // auto ast = parse_input("CREATE TABLE idk ( col1 INT32, col2 DOUBLE, col3 BOOL, col4 STR);");
  // auto ast = parse_input("select * from table1 where true;");
  // std::cout << ast.get()->repr() << std::endl;
  return 0;
}
