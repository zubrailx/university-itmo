#ifndef INC_02_BUFFER_BUFFER_H
#define INC_02_BUFFER_BUFFER_H

#include <cstddef>

class BufferView;

// Владеющий буфер
class Buffer {
public:
  // Конструктор по умолчанию, создает пустой буфер
  Buffer() = default;

  // Конструктор, создающий буфер указанного размера
  explicit Buffer(std::size_t capacity);

  // Конструктор, создающий буфер из существующих данных, копируя их
  // внутрь буфера
  Buffer(const void *data, std::size_t size);

  // Конструктор, создающий владеющий буфер из невладеющего, копируя
  // данные из него внутрь буфера
  explicit Buffer(const BufferView &buffer_view);

  // Конструктор копирования
  Buffer(const Buffer &buffer);

  // Конструктор перемещения
  Buffer(Buffer &&buffer) noexcept;

  // Копирующий оператор присваивания
  Buffer &operator=(const Buffer &buffer);

  // Перемещающий оператор присваивания
  Buffer &operator=(Buffer &&buffer) noexcept;

  // Деструктор
  ~Buffer();

  // Получить указатель на данные в буфере (возвращаемое значение нельзя
  // игнорировать)
  [[nodiscard]] void *get_data() const;

  // Получить размер данных в буфере
  [[nodiscard]] std::size_t get_size() const;

  // Установить размер данных в буфере
  void set_size(std::size_t new_size);

  // Получить емкость буфера
  [[nodiscard]] std::size_t get_capacity() const;

  // Установить емкость буфера
  void set_capacity(std::size_t new_capacity);

private:
  void *m_data = nullptr;
  size_t m_size = 0;
  size_t m_capacity = 0;
};

#endif // INC_02_BUFFER_BUFFER_H
