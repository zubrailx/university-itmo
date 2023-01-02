#include <ast.hpp>

template class AstVariable<int32_t>;
template class AstVariable<double>;
template class AstVariable<bool>;

// std::string
template<>
std::string AstVariable<std::string>::repr() const {
  return _repr(std::move(m_val)); // really scared
}

template class AstVariable<std::string>;
