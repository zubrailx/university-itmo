#include <qpg.hpp>

int main() {
  parse_input("SELECT * FROM (select table1.col1 from table1) as table2 WHERE \"val\" < table1.col or false and true;");
  parse_input("UPDATE table1 set col1 = 'val';");
  parse_input("DELETE FROM table1 where col1 = 'val';");
  parse_input("INSERT INTO table1 VALUES ('val', 'val2', -4, -4.2, false);");
  parse_input("DROP TABLE idkwhat;");
  parse_input("CREATE TABLE idk ( col1 INT32, col2 DOUBLE, col3 BOOL, col4 STR);");

  return 0;
}
