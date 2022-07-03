#ifndef $_PROJECT_BUFFERVIEW_H
#define $_PROJECT_BUFFERVIEW_H

#include <cstddef>

class Buffer;

// Невладеющий буфер. Предоставляет доступ только для чтения
class BufferView {
public:
  // Конструктор по умолчанию, создает пустой буфер
  BufferView() = default;

  // Конструктор, создающий буфер из существующих данных,
  // сохраняя указатель на них и их размер
  BufferView(void *data, size_t size);

  // Конструктор, создающий невладеющий буфер из владеющего, копируя
  // указатель на данные из него и их размер
  explicit BufferView(const Buffer &buffer);

  // Конструктор копирования
  BufferView(const BufferView &buffer_view) = default;

  // Конструктор перемещения
  BufferView(BufferView &&buffer_view) noexcept = default;

  // Копирующий оператор присваивания
  BufferView &operator=(const BufferView &buffer_view) = default;

  // Перемещающий оператор присваивания
  BufferView &operator=(BufferView &&buffer_view) noexcept = default;

  // Деструктор
  ~BufferView() = default;

  // Получить указатель на данные в буфере
  [[nodiscard]] const void *get_data() const;

  // Получить размер данных в буфере
  [[nodiscard]] std::size_t get_size() const;

private:
  void *m_data = nullptr;
  std::size_t m_size = 0;
};
#endif //$_PROJECT_BUFFERVIEW_H
