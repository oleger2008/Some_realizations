#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value)
    : data_()
    , is_initialized_(true)
    , value_ptr_(new(&data_[0]) T(value))
    {}
    Optional(T&& value)
    : data_()
    , is_initialized_(true)
    , value_ptr_(new(&data_[0]) T(std::move(value)))
    {}
    Optional(const Optional& other)
    : data_()
    , is_initialized_(other.is_initialized_ ? true : false)
    , value_ptr_(other.is_initialized_ ? new(&data_[0]) T(*other.value_ptr_) : nullptr)
    {}
    Optional(Optional&& other)
    : data_()
    , is_initialized_(other.is_initialized_ ? true : false)
    , value_ptr_(other.is_initialized_ ? new(&data_[0]) T(std::move(*other.value_ptr_)) : nullptr)
    {}

    Optional& operator=(const T& value) {
    	if (this->is_initialized_) {
    		*value_ptr_ = value;
    	} else {
    		is_initialized_ = true;
    		value_ptr_ = new(&data_[0]) T(value);
    	}
    	return *this;
    }
    Optional& operator=(T&& rhs) {
    	if (this->is_initialized_) {
    		*value_ptr_ = std::move(rhs);
    	} else {
    		is_initialized_ = true;
    		value_ptr_ = new(&data_[0]) T(std::move(rhs));
    	}
    	return *this;
    }
    Optional& operator=(const Optional& rhs) {
    	if (this->is_initialized_) {
			if (rhs.is_initialized_) {
				*this->value_ptr_ = *rhs.value_ptr_;
			} else {
				this->Reset();
			}
    	} else {
			if (rhs.is_initialized_) {
				this->is_initialized_ = true;
				this->value_ptr_ = new(&data_[0]) T(*rhs.value_ptr_);
			}
    	}
    	return *this;
    }
    Optional& operator=(Optional&& rhs) {
    	if (this->is_initialized_) {
			if (rhs.is_initialized_) {
				*this->value_ptr_ = std::move(*rhs.value_ptr_);
			} else {
				this->Reset();
			}
    	} else {
			if (rhs.is_initialized_) {
				this->is_initialized_ = true;
				this->value_ptr_ = new(&data_[0]) T(std::move(*rhs.value_ptr_));
			}
    	}
    	return *this;
    }

    ~Optional() {
    	Reset();
    }

    bool HasValue() const {
    	return is_initialized_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() {
    	return *value_ptr_;
    }
    const T& operator*() const {
    	return *value_ptr_;
    }
    T* operator->() {
    	return value_ptr_;
    }
    const T* operator->() const {
    	return value_ptr_;
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() {
    	if (!is_initialized_) {
    		throw BadOptionalAccess();
    	}
    	return *value_ptr_;
    }
    const T& Value() const {
    	if (!is_initialized_) {
    		throw BadOptionalAccess();
    	}
    	return *value_ptr_;
    }

    void Reset() {
    	if (is_initialized_) {
        	value_ptr_->~T();
        	value_ptr_ = nullptr;
        	is_initialized_ = false;
    	}
    }

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* value_ptr_ = nullptr;
};
