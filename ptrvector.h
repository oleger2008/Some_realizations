#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Реализуйте копирующий конструктор самостоятельно
        for (T* item : other.items_) {
        	if (item == nullptr) {
        		items_.push_back(nullptr);
        	} else {
        		items_.push_back(new T(*item));
        	}
        }

        /*size_t size = other.items_.size();
        items_.resize(size, nullptr);
        for (size_t i = 0; i < size; ++i) {
        	if (other.items_[i] != nullptr) {
        		items_[i] = new T(*other.items_[i]);
        	}
        }*/
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        // Реализуйте тело деструктора самостоятельно
    	for (T* item : items_) {
    		delete item;
    	}
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        // Реализуйте метод самостоятельно
    	return items_;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        // Реализуйте метод самостоятельно
    	return items_;
    }

private:
    std::vector<T*> items_;
};
