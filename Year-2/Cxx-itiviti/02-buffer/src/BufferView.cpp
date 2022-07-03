#include "BufferView.h"
#include "Buffer.h"

BufferView::BufferView(void *data, size_t size) : m_data(data), m_size(size) {}

BufferView::BufferView(const Buffer &buffer)
    : m_data(buffer.get_data()), m_size(buffer.get_size()) {}

const void *BufferView::get_data() const { return m_data; }

std::size_t BufferView::get_size() const { return m_size; }