#include <cassert>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <utility>

class Any {
public:
	// Конструктор по умолчанию, создает пустой Any
	constexpr Any() noexcept = default;

	// Конструктор, создающий Any, копируя или перемещая переданный объект
	template<typename T>
	Any(T && object) : m_instance(make_holder<T>(std::forward<T>(object))) {}

	// Конструктор копирования
	Any(const Any & rhs) {
		if (rhs.m_instance) {
			m_instance = rhs.m_instance->clone();
			std::cout << m_instance.get() << std::endl;
		}
	}

	// Конструктор перемещения
	Any(Any && rhs) noexcept : m_instance(std::move(rhs.m_instance)) {}

	// Оператор присваивания, копирующий или перемещающий переданный объект
	template<typename T>
	Any & operator=(T && rhs) noexcept {
		m_instance = make_holder<T>(std::forward<T>(rhs));
		return *this;
	}

	// Перемещающий оператор присваивания
	Any & operator=(const Any & rhs) {
		if (&rhs != this) {
			m_instance = rhs.m_instance ? rhs.m_instance->clone() : nullptr;
		}
		return *this;
	}

	// Копирующий оператор присваивания
	Any & operator=(Any && rhs) {
		std::swap(m_instance, rhs.m_instance);
		return *this;
	}

	// Деструктор
	~Any() = default;

	// Получить ссылку на объект в Any.
	// Если Т не совпадает с типом объекта, нужно бросить exception.
	template<typename T>
	T & get() {
		assert(typeid(T) == m_instance->type());
		return static_cast<storage<T> *>(m_instance.get())->m_value;
	}

	// Получить константную ссылку на объект в Any.
	// Если Т не совпадает с типом объекта, нужно бросить exception.
	template<typename T>
	const T & get() const {
		assert(typeid(T) == m_instance->type());
		return static_cast<const storage<T> *>(m_instance.get())->m_value;
  }

	// Очистить Any
	void reset() noexcept {
    m_instance.reset();
  }

	// Проверить, пустой ли Any
	bool is_empty() const noexcept {
    return !m_instance;
  }

	// Обменять значение с другим Any
	void swap(Any & other) noexcept {
    std::swap(m_instance, other.m_instance);
  }

	// Создать объект Т в Any из аргументов Args без дополнительных копирований
	template<typename T, typename... Args>
	void emplace(Args &&... args) noexcept {
    m_instance = make_holder<T>(std::forward<Args>(args)...);
  }

private:
	struct storage_base;

	std::unique_ptr<storage_base> m_instance;

	struct storage_base {
		virtual ~storage_base() = default;

		virtual const std::type_info & type() const noexcept = 0;
		virtual std::unique_ptr<storage_base> clone() const = 0;
	};

	template<typename T>
	struct storage : public storage_base {
		T m_value;

		template<typename U>
		storage(U && value) : m_value(std::forward<U>(value)) {}

		const std::type_info & type() const noexcept override { return typeid(T); }

		std::unique_ptr<storage_base> clone() const override {
			return std::make_unique<storage<T>>(m_value);
		}
	};

	template<typename T, typename... Args>
	std::unique_ptr<storage_base> make_holder(Args &&... args) const noexcept {
		return std::make_unique<storage<T>>(std::forward<Args>(args)...);
	}
};
