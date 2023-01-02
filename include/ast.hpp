#pragma once

#include <subtok.hpp>

#include <cassert>
#include <list>
#include <memory>
#include <string>
#include <typeinfo>

enum class AstType {
  VARIABLE,
  VARIABLE_LIST,
  COLUMN,
  COLUMN_LIST,
  STATEMENT_LIST,
};

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
struct Column : Ast {
private:
  bool m_htn; // has table name
  std::string m_table;
  std::string m_column;

public:
  Column(std::string &&column)
      : Ast(AstType::COLUMN), m_column(std::move(column)) {
    m_htn = false;
  }

  Column(std::string &&table, std::string &&column)
      : Column(std::move(column)) {
    m_table = std::move(table);
    m_htn = true;
  }

  std::string repr() const {
    std::string out;
    out += "table: '" + m_table + "', ";
    out += "column: '" + m_column + "'";
    return repr_extend(out);
  }
};

class ColumnList : public AstList<Column> {
private:
  bool do_all = false; // select all columns
public:
  ColumnList(ColumnList *lst, Column *last) : AstList(lst, last) {}
  ColumnList(Column *col) : AstList(col, AstType::COLUMN_LIST) {}
  ColumnList() : AstList(AstType::COLUMN_LIST) { do_all = true; }

  std::string repr() const override {
    std::string out;
    out += "do_all: " + std::string(do_all ? "true" : "false");
    out += ", ";
    out += AstList<Column>::_repr();
    return repr_extend(out);
  }
};
// }}}
