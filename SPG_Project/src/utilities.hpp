#pragma once
#include <queue>
#include <algorithm>

template <typename T>
void clearQueue(std::queue<T>& q)
{
    std::queue<T> empty;
    std::swap(q, empty);
}

template <typename T>
bool listContainElement(std::list<std::pair<T, T>>& list, T pair_item_1, T pair_item_2)
{
    return std::find(list.begin(), list.end(), std::make_pair(pair_item_1, pair_item_2)) != list.end();
}