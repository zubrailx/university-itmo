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
class AstValue : public Ast {
private:
  struct StorageBase {
    virtual ~StorageBase() = default;

    virtual const std::type_info &type() const noexcept = 0;
  };

  template <typename T> struct Storage : StorageBase {
    T m_value;

    explicit Storage(T &&value) : m_value(std::forward<T>(value)) {}

    const std::type_info &type() const noexcept override { return typeid(T); }
  };

  template <typename T>
  std::unique_ptr<StorageBase> make_holder(T &&value) const noexcept {
    return std::make_unique<Storage<T>>(std::forward<T>(value));
  }

  template <typename T> std::string str_repr(T &&val) const;

private:
  std::unique_ptr<StorageBase> m_inst;
  std::string strval;
  DataType m_dtype;

public:
  template <typename T>
  AstValue(T &&obj, DataType dtype)
      : Ast(AstType::VARIABLE), m_inst(make_holder(obj)), m_dtype(dtype) {
    // get string at runtime
    strval = std::string(str_repr(std::forward<T>(obj)));
  };

  template <typename T> T getValue() {
    assert(typeid(T) == m_inst->type());
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
template <> inline std::string AstValue::str_repr(std::string &&val) const {
  return val;
}

template <typename T> std::string AstValue::str_repr(T &&val) const {
  return std::to_string(val);
}
// }}}

// AstList {{{
template <typename T> class AstList : public Ast {
protected:
  std::list<std::unique_ptr<T>> m_lst;

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
    m_lst.push_back(std::unique_ptr<T>(last));
  }

  AstList(T *single, AstType type) : Ast(type) {
    m_lst.push_back(std::unique_ptr<T>(single));
  }

  AstList(AstType type) : Ast(type) {}

  std::string repr() const override { return repr_extend(_repr()); }
};
// }}}

// Column {{{
struct AstColumn : Ast {
private:
  bool m_htn; // has table name
  std::string m_table;
  std::string m_column;

public:
  AstColumn(const char *column)
      : Ast(AstType::COLUMN), m_column(std::string(column)) {
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

class AstColumnList : public AstList<AstColumn> {
private:
  bool do_all = false; // select all columns
public:
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
class AstTable : public Ast {
private:
  std::string m_name;
  bool m_has_alias = false;
  std::string m_alias;

public:
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
class AstJoin : public Ast {
private:
  std::unique_ptr<Ast> m_lsv;
  JoinType m_jtype;
  std::unique_ptr<Ast> m_rsv;

public:
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
class AstStatement : public Ast {
private:
  StatementType m_stype;

protected:
  std::string str_repr() const {
    std::string out;
    out += "stype: ";
    out += std::to_string(static_cast<int>(m_stype));
    return out;
  }

public:
  AstStatement(StatementType stype) : Ast(AstType::STATEMENT) {
    m_stype = stype;
  }
};

class AstStatementConst : public AstStatement {
private:
  std::unique_ptr<AstValue> m_res;

public:
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

class AstStatementColumn : public AstStatement {
private:
  std::unique_ptr<AstColumn> m_col;

public:
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

class AstStatementUnary : public AstStatement {
private:
  std::unique_ptr<AstStatement> m_operand;
  OperationType m_op;

public:
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

class AstStatementBinary : public AstStatement {
private:
  std::unique_ptr<AstStatement> m_left;
  std::unique_ptr<AstStatement> m_right;
  OperationType m_op;

public:
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
class AstSelect : public Ast {
private:
  std::unique_ptr<AstColumnList> m_collist;
  std::unique_ptr<Ast> m_table_ref;

  std::unique_ptr<AstStatement> m_cond;
  bool m_has_cond;

public:
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
class AstSubquery : public Ast {
private:
  std::unique_ptr<AstSelect> m_query;

  bool m_has_alias = false;
  std::string m_alias;

public:
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

class AstColumnValue : public Ast {
private:
  std::unique_ptr<AstColumn> m_column;
  std::unique_ptr<AstValue> m_value;

public:
  AstColumnValue(const char *col, AstValue *value)
      : Ast(AstType::COLUMN_VALUE) {
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
class AstUpdate : public Ast {
private:
  std::string m_table;
  std::unique_ptr<AstList<AstColumnValue>> m_collist;

  std::unique_ptr<AstStatement> m_cond;
  bool m_has_cond;

public:
  AstUpdate(const char *table, AstList<AstColumnValue> *collist,
            AstStatement *cond)
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
class AstDelete : public Ast {
private:
  std::string m_table;

  std::unique_ptr<AstStatement> m_cond;
  bool m_has_cond;

public:
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
class AstInsert : public Ast {
private:
  std::string m_table;

  std::unique_ptr<AstColumnList> m_collist;
  std::unique_ptr<AstList<AstValue>> m_vallist;

public:
  AstInsert(const char *table, AstColumnList *collist,
            AstList<AstValue> *vallist)
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
class AstDrop : public Ast {
private:
  std::string m_table;

public:
  AstDrop(const char *table) : Ast(AstType::DROP) {
    m_table = std::string(table);
  }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    return repr_extend(out);
  }
};

// {{{ AstColumnType
class AstColumnType : public Ast {
private:
  std::unique_ptr<AstColumn> m_column;
  DataType m_type;

public:
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
class AstCreate : public Ast {
private:
  std::string m_table;
  std::unique_ptr<AstList<AstColumnType>> m_collist;

public:
  AstCreate(const char *table, AstList<AstColumnType> *collist)
      : Ast(AstType::CREATE) {
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
