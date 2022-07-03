#include "Buffer.h"
#include "BufferView.h"
#include <cstring>
#include <stdexcept>
#include <cassert>

Buffer::Buffer(std::size_t capacity) : m_capacity(capacity) {
  m_data = ::malloc(m_capacity);
  assert(m_data);
}

Buffer::Buffer(const void *data, std::size_t size) : m_size(size), m_capacity(size) {
  m_data = ::malloc(m_capacity);
  assert(m_data);
  ::memcpy(m_data, data, m_size);
}

Buffer::Buffer(const BufferView &buffer_view)
    : Buffer(buffer_view.get_data(), buffer_view.get_size()) {
}

Buffer::Buffer(const Buffer &buffer)
    : Buffer(buffer.m_data, buffer.m_size) {
  m_capacity = buffer.m_capacity;
}

Buffer::Buffer(Buffer &&buffer) noexcept
    : m_data(buffer.m_data), m_size(buffer.m_size),
      m_capacity(buffer.m_capacity) {
  buffer.m_data = nullptr;
}

Buffer &Buffer::operator=(const Buffer &buffer) {
  if (&buffer == this) { return *this; }
  if (buffer.m_capacity > this->m_capacity) {
    m_data = ::realloc(m_data, buffer.m_capacity);
  }
  assert(m_data);
  ::memcpy(m_data, buffer.m_data, m_size);
  m_size = buffer.m_size;
  m_capacity = buffer.m_capacity;
  return *this;
}

Buffer &Buffer::operator=(Buffer &&buffer) noexcept {
  if (&buffer == this) { return *this; }
  m_size = buffer.m_size;
  m_capacity = buffer.m_capacity;
  std::swap(this->m_data, buffer.m_data);
  return *this;
}

Buffer::~Buffer() {
  ::free(m_data);
}

void *Buffer::get_data() const { return m_data; }

std::size_t Buffer::get_size() const { return m_size; }

void Buffer::set_size(std::size_t new_size) {
  assert(new_size <= m_capacity);
  m_size = new_size;
}

std::size_t Buffer::get_capacity() const { return m_capacity; }

void Buffer::set_capacity(std::size_t new_capacity) {
  m_data = ::realloc(m_data, new_capacity);
  assert(m_data);
  m_capacity = new_capacity;
  if (m_capacity < m_size) {
    m_size = m_capacity;
  }
}
