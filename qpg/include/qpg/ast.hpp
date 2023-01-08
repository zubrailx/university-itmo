#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <typeinfo>

enum class AstType {
  VARIABLE,
  VARIABLE_LIST,

  COLUMN,
  COLUMN_LIST,

  COLUMN_VALUE,
  COLUMN_VALUE_LIST,

  COLUMN_TYPE,
  COLUMN_TYPE_LIST,

  QUERY_LIST,

  TABLE,
  JOIN,
  STATEMENT,
  SUBQUERY,
  SELECT,
  UPDATE,
  DELETE,
  INSERT,
  DROP,
  CREATE
};

enum class JoinType {
  CROSS_JOIN,
};

enum class StatementType { CONST, UNARY, BINARY, COLUMN };

enum class OperationType { EQ, NEQ, LO, GR, LEQ, GEQ, NOT, AND, OR, IN };

enum class DataType { STR, BOOL, DOUBLE, INT32 };

// Ast {{{
class Ast {
protected:
  AstType m_type;

public:
  explicit Ast(AstType type) noexcept : m_type(type){};
  virtual ~Ast() = default;

  AstType getType() const { return this->m_type; }

  virtual std::string repr() const = 0;

  std::string repr_extend(const std::string &str) const {
    std::string out = "{ ";
    out += "ast_type: " + std::to_string(static_cast<int>(m_type));
    out += ", ";
    out += str;
    out += " }";
    return out;
  }
};
// }}}

// AstValue {{{
struct AstValue : public Ast {
private:
  struct StorageBase {
    virtual ~StorageBase() = default;

    // virtual const std::type_info &type() const noexcept = 0;
  };

  template<typename T>
  struct Storage : StorageBase {
    T m_value;

    explicit Storage(const T &value) : m_value(value) {}

    explicit Storage(T &&value) : m_value(std::move(value)) {}

    // const std::type_info &type() const noexcept override { return typeid(T); }
  };

  template<typename T>
  std::string str_repr(T &&val) const;

private:
  std::unique_ptr<StorageBase> m_inst;

public:
  std::string strval;
  DataType m_dtype;

public:
  // copy object
  template<typename T>
  AstValue(T obj, DataType dtype) : Ast(AstType::VARIABLE) {
    m_inst = std::make_unique<Storage<T>>(std::forward<T>(obj));
    m_dtype = dtype;
    strval = std::string(str_repr(std::move(obj)));
  };

  template<typename T>
  T getValue() const {
    // assert(typeid(T) == m_inst->type());
    return static_cast<const Storage<T> *>(m_inst.get())->m_value;
  }

  template<typename T>
  const T &getValueRef() const {
    // assert(typeid(T) == m_inst->type());
    return static_cast<const Storage<T> *>(m_inst.get())->m_value;
  }

  std::string repr() const override {
    std::string out;
    out += "dtype: " + std::to_string(static_cast<int>(m_dtype));
    out += ", ";
    out += "val: '" + strval + "'";
    return repr_extend(out);
  }
};

// idk how to check type using static_assert
template<>
inline std::string AstValue::str_repr(std::string &&val) const {
  return val;
}

template<typename T>
std::string AstValue::str_repr(T &&val) const {
  return std::to_string(val);
}
// }}}

// AstList {{{
template<typename T>
struct AstList : public Ast {
  std::list<std::unique_ptr<T>> m_lst;

protected:
  std::string _repr() const {
    std::string out;
    out += "list: ";
    out += "[ ";
    for (auto it = m_lst.begin(); it != m_lst.end(); ++it) {
      out = out + (*it)->repr() + ", ";
    }
    if (m_lst.begin() != m_lst.end()) {
      out.resize(out.size() - 2);
    }
    out += " ]";
    return out;
  }

public:
  AstList(AstList<T> *lst, T *last) : Ast(lst->getType()) {
    std::swap(m_lst, lst->m_lst);
    m_lst.emplace_back(last);
  }

  AstList(T *single, AstType type) : Ast(type) { m_lst.emplace_back(single); }

  AstList(T &&single, AstType type) : Ast(type) { m_lst.emplace_back(single); }

  AstList(AstType type) : Ast(type) {}

public:
  std::string repr() const override { return repr_extend(_repr()); }
};
// }}}

// Column {{{
struct AstColumn : Ast {
  bool m_htn;// has table name
  std::string m_table;
  std::string m_column;

  AstColumn(const char *column) : Ast(AstType::COLUMN), m_column(std::string(column)) {
    m_htn = false;
  }

  AstColumn(const char *table, const char *column) : AstColumn(column) {
    m_table = std::string(table);
    m_htn = true;
  }

  std::string repr() const {
    std::string out;
    out += "table: '" + m_table + "', ";
    out += "column: '" + m_column + "'";
    return repr_extend(out);
  }
};

struct AstColumnList : public AstList<AstColumn> {
  bool do_all = false;// select all columns

  AstColumnList(AstColumnList *lst, AstColumn *last) : AstList(lst, last) {}
  AstColumnList(AstColumn *col) : AstList(col, AstType::COLUMN_LIST) {}
  AstColumnList() : AstList(AstType::COLUMN_LIST) { do_all = true; }

  std::string repr() const override {
    std::string out;
    out += "do_all: " + std::string(do_all ? "true" : "false");
    out += ", ";
    out += AstList<AstColumn>::_repr();
    return repr_extend(out);
  }
};
// }}}

// AstTable {{{
struct AstTable : public Ast {
  std::string m_name;
  bool m_has_alias = false;
  std::string m_alias;

  AstTable(std::string &&name, const char *alias) : Ast(AstType::TABLE) {
    if (alias) {
      m_has_alias = true;
      m_alias = std::string(alias);
    }
    m_name = std::move(name);
  }

  std::string repr() const {
    std::string out;
    out += "has_alias: ";
    out += m_has_alias ? "true" : "false";
    if (m_has_alias) {
      out += ", ";
      out += "alias: ";
      out += m_alias;
    }
    out += ", ";
    out += "name: ";
    out += m_name;
    return repr_extend(out);
  }
};
// }}}

// AstJoin {{{
struct AstJoin : public Ast {
  std::unique_ptr<Ast> m_lsv;
  JoinType m_jtype;
  std::unique_ptr<Ast> m_rsv;

  AstJoin(Ast *lsv, JoinType jtype, Ast *rsv) : Ast(AstType::JOIN) {
    m_lsv = std::unique_ptr<Ast>(lsv);
    m_jtype = jtype;
    m_rsv = std::unique_ptr<Ast>(rsv);
  }
  std::string repr() const {
    std::string out;
    out += "jtype: ";
    out += std::to_string(static_cast<int>(m_jtype));
    out += ", ";
    out += "lsv: ";
    out += m_lsv.get()->repr();
    out += ", ";
    out += "rsv: ";
    out += m_rsv.get()->repr();
    return repr_extend(out);
  }
};
// }}}

// AstStatement {{{
struct AstStatement : public Ast {
  StatementType m_stype;

protected:
  std::string str_repr() const {
    std::string out;
    out += "stype: ";
    out += std::to_string(static_cast<int>(m_stype));
    return out;
  }

public:
  AstStatement(StatementType stype) : Ast(AstType::STATEMENT) { m_stype = stype; }
};

struct AstStatementConst : public AstStatement {
  std::unique_ptr<AstValue> m_res;

  AstStatementConst(AstValue *res) : AstStatement(StatementType::CONST) {
    m_res = std::unique_ptr<AstValue>(res);
  }
  std::string repr() const {
    std::string out = AstStatement::str_repr();
    out += ", ";
    out += "res: ";
    out += m_res.get()->repr();
    return repr_extend(out);
  }
};

struct AstStatementColumn : public AstStatement {
  std::unique_ptr<AstColumn> m_col;

  AstStatementColumn(AstColumn *col) : AstStatement(StatementType::COLUMN) {
    m_col = std::unique_ptr<AstColumn>(col);
  }
  std::string repr() const {
    std::string out = AstStatement::str_repr();
    out += ", ";
    out += "column: ";
    out += m_col.get()->repr();
    return repr_extend(out);
  }
};

struct AstStatementUnary : public AstStatement {
  std::unique_ptr<AstStatement> m_operand;
  OperationType m_op;

  AstStatementUnary(AstStatement *operand, OperationType op)
      : AstStatement(StatementType::UNARY) {
    m_operand = std::unique_ptr<AstStatement>(operand);
    m_op = op;
  }

  std::string repr() const {
    std::string out = AstStatement::str_repr();
    out += ", ";
    out += "operand: ";
    out += m_operand.get()->repr();
    out += ", ";
    out += "op_type: ";
    out += std::to_string(static_cast<int>(m_op));
    return repr_extend(out);
  }
};

struct AstStatementBinary : public AstStatement {
  std::unique_ptr<AstStatement> m_left;
  std::unique_ptr<AstStatement> m_right;
  OperationType m_op;

  AstStatementBinary(AstStatement *left, AstStatement *right, OperationType op)
      : AstStatement(StatementType::BINARY) {
    m_left = std::unique_ptr<AstStatement>(left);
    m_right = std::unique_ptr<AstStatement>(right);
    m_op = op;
  }

  std::string repr() const {
    std::string out = AstStatement::str_repr();
    out += ", ";
    out += "op_left: ";
    out += m_left.get()->repr();
    out += ", ";
    out += "op_right: ";
    out += m_right.get()->repr();
    out += ", ";
    out += "op_type: ";
    out += std::to_string(static_cast<int>(m_op));
    return repr_extend(out);
  }
};
// }}}

// AstSelect {{{
struct AstSelect : public Ast {
  std::unique_ptr<AstColumnList> m_collist;
  std::unique_ptr<Ast> m_table_ref;

  std::unique_ptr<AstStatement> m_cond;
  bool m_has_cond;

  AstSelect(AstColumnList *collist, Ast *table_ref, AstStatement *cond)
      : Ast(AstType::SELECT) {
    m_collist = std::unique_ptr<AstColumnList>(collist);
    m_table_ref = std::unique_ptr<Ast>(table_ref);
    m_has_cond = (bool)cond;
    if (m_has_cond) {
      m_cond = std::unique_ptr<AstStatement>(cond);
    }
  }

  std::string repr() const {
    std::string out;
    out += "column_list: ", out += m_collist.get()->repr();
    out += ", ";
    out += "table_ref: ";
    out += m_table_ref.get()->repr();
    out += ", ";
    out += "has_cond: ";
    out += m_has_cond ? "true" : "false";
    if (m_has_cond) {
      out += ", ";
      out += "cond: ";
      out += m_cond.get()->repr();
    }
    return repr_extend(out);
  }
};
// }}}

// AstSubquery {{{
struct AstSubquery : public Ast {
  std::unique_ptr<AstSelect> m_query;

  bool m_has_alias = false;
  std::string m_alias;

  AstSubquery(AstSelect *query) : Ast(AstType::SUBQUERY) {
    m_query = std::unique_ptr<AstSelect>(query);
  }

  void setAlias(const char *alias) {
    m_alias = std::string(alias);
    m_has_alias = true;
  }

  std::string repr() const {
    std::string out;
    out += "query: ";
    out += m_query.get()->repr();
    out += ", ";
    out += "has_alias: ";
    out += m_has_alias ? "true" : "false";
    if (m_has_alias) {
      out += ", ";
      out += "alias: ";
      out += m_alias;
    }
    return repr_extend(out);
  }
};
// }}}

struct AstColumnValue : public Ast {
  std::unique_ptr<AstColumn> m_column;
  std::unique_ptr<AstValue> m_value;

  AstColumnValue(const char *col, AstValue *value) : Ast(AstType::COLUMN_VALUE) {
    m_column = std::make_unique<AstColumn>(col);
    m_value = std::unique_ptr<AstValue>(value);
  }

  std::string repr() const {
    std::string out;
    out += "column: ";
    out += m_column.get()->repr();
    out += ", ";
    out += "value: ";
    out += m_value.get()->repr();
    return repr_extend(out);
  }
};

// AstUpdate {{{
struct AstUpdate : public Ast {
  std::string m_table;
  std::unique_ptr<AstList<AstColumnValue>> m_collist;

  std::unique_ptr<AstStatement> m_cond;
  bool m_has_cond;

  AstUpdate(const char *table, AstList<AstColumnValue> *collist, AstStatement *cond)
      : Ast(AstType::UPDATE) {
    m_table = std::string(table);
    m_collist = std::unique_ptr<AstList<AstColumnValue>>(collist);
    m_has_cond = (bool)cond;
    if (m_has_cond) {
      m_cond = std::unique_ptr<AstStatement>(cond);
    }
  }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    out += ", ";
    out += "column_list: ";
    out += m_collist.get()->repr();
    out += ", ";
    out += "has_cond: ";
    out += m_has_cond ? "true" : "false";
    if (m_has_cond) {
      out += ", ";
      out += "cond: ";
      out += m_cond.get()->repr();
    }
    return repr_extend(out);
  }
};
// }}}

// AstDelete {{{
struct AstDelete : public Ast {
  std::string m_table;

  std::unique_ptr<AstStatement> m_cond;
  bool m_has_cond;

  AstDelete(const char *table, AstStatement *cond) : Ast(AstType::DELETE) {
    m_table = std::string(table);
    m_has_cond = (bool)cond;
    if (m_has_cond) {
      m_cond = std::unique_ptr<AstStatement>(cond);
    }
  }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    out += ", ";
    out += "has_cond: ";
    out += m_has_cond ? "true" : "false";
    if (m_has_cond) {
      out += ", ";
      out += "cond: ";
      out += m_cond.get()->repr();
    }
    return repr_extend(out);
  }
};
///}}}

// AstDelete {{{
struct AstInsert : public Ast {
  std::string m_table;

  std::unique_ptr<AstColumnList> m_collist;
  std::unique_ptr<AstList<AstValue>> m_vallist;

  AstInsert(const char *table, AstColumnList *collist, AstList<AstValue> *vallist)
      : Ast(AstType::INSERT) {
    m_table = std::string(table);
    m_collist = std::unique_ptr<AstColumnList>(collist);
    m_vallist = std::unique_ptr<AstList<AstValue>>(vallist);
  }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    out += ", ";
    out += "collist: ";
    out += m_collist.get()->repr();
    out += ", ";
    out += "vallist: ";
    out += m_vallist.get()->repr();
    return repr_extend(out);
  }
};
// }}}

// AstDelete {{{
struct AstDrop : public Ast {
  std::string m_table;

  AstDrop(const char *table) : Ast(AstType::DROP) { m_table = std::string(table); }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    return repr_extend(out);
  }
};

// {{{ AstColumnType
struct AstColumnType : public Ast {
  std::unique_ptr<AstColumn> m_column;
  DataType m_type;

  AstColumnType(AstColumn *column, DataType type) : Ast(AstType::COLUMN_TYPE) {
    m_column = std::unique_ptr<AstColumn>(column);
    m_type = type;
  }

  std::string repr() const {
    std::string out;
    out += "column: ";
    out += m_column.get()->repr();
    out += ", ";
    out += "type: ";
    out += std::to_string(static_cast<int>(m_type));
    return repr_extend(out);
  }
};
// }}}

// AstCreate {{{
struct AstCreate : public Ast {
  std::string m_table;
  std::unique_ptr<AstList<AstColumnType>> m_collist;

  AstCreate(const char *table, AstList<AstColumnType> *collist) : Ast(AstType::CREATE) {
    m_table = std::string(table);
    m_collist = std::unique_ptr<AstList<AstColumnType>>(collist);
  }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    out += ", ";
    out += "collist: ";
    out += m_collist.get()->repr();
    return repr_extend(out);
  }
};
// }}}
