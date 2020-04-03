/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2020 Jia Lihong
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#pragma once

#include <functional>
#include <vector>
#include <set>
#include <cassert>
#include <utility>

template<typename T>
class MutablePriorityQueue
{
public:
    typedef size_t handle_type;
    typedef T value_type;

    explicit MutablePriorityQueue(std::function<bool(value_type &, value_type &)> comp)
        : m_funcComp(comp)
    {
        // the first place is not used
        m_vHandleHeap.push_back(-1);
    }
    size_t size() const { return m_vHandleHeap.size() - 1; }
    bool empty() const { return m_vHandleHeap.size() == 1; }
    handle_type push(const value_type &v)
    {
        handle_type handle;

        // if there is invalid element, take place its position
        if (!m_vInvalid.empty())
        {
            auto iter = m_vInvalid.end() - 1;
            handle = *iter;
            m_vElements[handle] = v;
            m_vInvalid.erase(iter);
        }
        else
        {
            handle = m_vElements.size();
            m_vElements.push_back(v);
            m_vHandleIndex.push_back(-1);
        }

        m_vHandleIndex[handle] = m_vHandleHeap.size();
        m_vHandleHeap.push_back(handle);

        update(m_vHandleIndex[handle]);
        return handle;
    }
    value_type pop()
    {
        assert(!empty());

        value_type result = m_vElements[m_vHandleHeap[1]];

        m_vInvalid.push_back(m_vHandleHeap[1]);

        std::swap(m_vHandleIndex[m_vHandleHeap[1]],
                m_vHandleIndex[m_vHandleHeap[m_vHandleHeap.size() - 1]]);
        std::swap(m_vHandleHeap[1],
                m_vHandleHeap[m_vHandleHeap.size() - 1]);

        m_vHandleHeap.erase(m_vHandleHeap.end() - 1);
        lower(1);

        return result;
    }
    value_type top()
    {
        assert(!empty());
        return m_vElements[m_vHandleHeap[1]];
    }
    handle_type topHandle()
    {
        assert(!empty());
        return m_vHandleHeap[1];
    }
    value_type value(handle_type const &handle)
    {
        return m_vElements[handle];
    }
    void update(handle_type const &handle, value_type const &v)
    {
        m_vElements[handle] = v;
        update(m_vHandleIndex[handle]);
    }

private:
    void update(size_t index)
    {
        if (!lower(index))
        {
            raise(index);
        }
    }
    bool lower(const size_t index)
    {
        const size_t leftIndex = 2 * index;
        const size_t rightIndex = 2 * index + 1;
        size_t raiseIndex = -1;
        if ((leftIndex < m_vHandleHeap.size())
                && m_funcComp(m_vElements[m_vHandleHeap[leftIndex]],
                              m_vElements[m_vHandleHeap[index]]))
        {
            raiseIndex = leftIndex;
        }
        else
        {
            raiseIndex = index;
        }

        if ((rightIndex < m_vHandleHeap.size())
                && m_funcComp(m_vElements[m_vHandleHeap[rightIndex]],
                              m_vElements[m_vHandleHeap[raiseIndex]]))
        {
            raiseIndex = rightIndex;
        }

        if (raiseIndex == index)
        {
            return false;
        }
        else
        {
            std::swap(m_vHandleIndex[m_vHandleHeap[raiseIndex]],
                    m_vHandleIndex[m_vHandleHeap[index]]);
            std::swap(m_vHandleHeap[raiseIndex], m_vHandleHeap[index]);
            lower(raiseIndex);
            return true;
        }
    }
    bool raise(size_t index)
    {
        size_t current = index;
        while (current != 1)
        {
            size_t parent = current / 2;
            if (m_funcComp(m_vElements[m_vHandleHeap[current]],
                           m_vElements[m_vHandleHeap[parent]]))
            {
                std::swap(m_vHandleIndex[m_vHandleHeap[current]],
                        m_vHandleIndex[m_vHandleHeap[parent]]);
                std::swap(m_vHandleHeap[current],
                          m_vHandleHeap[parent]);
                current = parent;
            }
            else
            {
                break;
            }
        }
        return true;
    }
    std::function<bool(value_type &, value_type &)> m_funcComp;
    std::vector<value_type> m_vElements;
    std::vector<handle_type> m_vHandleHeap;
    std::vector<size_t> m_vHandleIndex;
    std::vector<handle_type> m_vInvalid;
};
