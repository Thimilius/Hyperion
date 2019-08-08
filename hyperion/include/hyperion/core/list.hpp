#pragma once

#include "hyperion/common.hpp"

#include <initializer_list>

namespace Hyperion {

    template<typename T>
    class CList {
    private:
        inline static T s_dummy_item = T();

        s32 m_count = 0;
        s32 m_capacity = 0;
        T *m_data = nullptr;
    public:
        inline CList() {
            Reserve(10);
        }
        
        inline CList(std::initializer_list<T> list) {
            Reserve((s32)list.size());

            for (auto item : list) {
                Add(item);
            }
        }

        inline CList(s32 capacity) {
            HYP_ASSERT_MESSAGE(capacity > 0, "List capacity must be greater than 0!");
            Reserve(capacity);
        }

        inline CList(const CList &list) {
            if (this == &list) {
                return;
            }

            m_count = list.m_count;
            m_capacity = list.m_capacity;
            m_data = new T[m_count];

            memcpy(m_data, list.m_data, m_count * sizeof(T));
        }

        inline ~CList() {
            if (m_data) {
                delete[] m_data;
            }
        }

        inline s32 GetCount() const {
            return m_count;
        }

        inline s32 GetCapacity() const {
            return m_capacity;
        }
        
        inline bool IsEmpty() const {
            return m_count == 0;
        }

        inline void Add(const T &item) {
            if (m_count == m_capacity) {
                Reserve(m_capacity * 2);
            }

            m_data[m_count] = item;
            m_count++;
        }

        inline void AddRange(std::initializer_list<T> list) {
            for (auto item : list) {
                Add(item);
            }
        }

        inline T &ElementAt(s32 index) const {
            HYP_ASSERT_MESSAGE(index >= 0 && index < m_count, "List bounds check failed!");
            return m_data[index];
        }

        inline s32 IndexOf(const T &item) const {
            for (s32 i = 0; i < m_count; i++) {
                if (item == m_data[i]) {
                    return i;
                }
            }
            return -1;
        }

        inline bool Contains(const T &item) const {
            return IndexOf(item) >= 0;
        }

        inline T &Find(bool (*predicate)(const T &)) const {
            for (s32 i = 0; i < m_count; i++) {
                T &item = m_data[i];
                if (predicate(item)) {
                    return item;
                }
            }
            return s_dummy_item;
        }

        inline void Remove(const T &item) {
            s32 index = IndexOf(item);
            if (index >= 0) {
                RemoveAt(index);
            }
        }

        inline void RemoveAt(s32 index) {
            if (index < 0 || index >= m_count) {
                return;
            }

            char *pointer = (char*)&m_data[index];
            size_t element_size = sizeof(T);

            memcpy(pointer, pointer + element_size, (m_count - index - 1) * element_size);

            m_count--;
        }

        inline void Clear() {
            m_count = 0;
        }

        inline T *begin() {
            return m_count > 0 ? &m_data[0] : nullptr;
        }
        
        inline T *end() { 
            // End points past the last element
            return m_count > 0 ? &m_data[m_count] : nullptr;
        }

        CList operator=(const CList &list) {
            if (this != &list) {
                if (m_data) {
                    delete[] m_data;
                }
                m_count = list.m_count;
                m_capacity = list.m_capacity;
                m_data = new T[m_count + 1];
                memcpy(m_data, list.m_data, m_count * sizeof(T));
            }
            return *this;
        }

        T &operator[] (s32 index) {
            HYP_ASSERT_MESSAGE(index >= 0 && index < m_count, "List bounds check failed!");
            return m_data[index];
        }
    private:
        inline void Reserve(s32 capacity) {
            T *old_data = m_data;

            m_data = new T[capacity];
            memcpy(m_data, old_data, m_count * sizeof(T));
            m_capacity = capacity;

            delete[] old_data;
        }
    };
}