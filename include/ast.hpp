#pragma once

#include <string>

enum class AstType {
  VARIABLE,
};

// Virtual AST
class Ast {
protected:
  AstType m_type;

public:
  Ast(AstType type) noexcept : m_type(type){};
  ~Ast() = default;

  AstType getType() const { return this->m_type; }

  virtual std::string repr() const = 0;
};

// AstVariable
template <typename T> class AstVariable : public Ast {
private:
  std::string _repr(const std::string &&str) const {
    return "{ dtype: " + std::to_string(static_cast<int>(m_dtype)) +
           ", value: " + str + " }";
  }

private:
  T m_val;
  int m_dtype;

public:
  AstVariable(T &&obj, int dtype)
      : Ast(AstType::VARIABLE), m_val(std::move(obj)), m_dtype(dtype){};

  const T getValue() { return this->m_val; }

  std::string repr() const { return _repr(std::to_string(m_val)); }
};
