#include <iostream>
#include <vector>
#include "rb_tree.h"

int main() {
    std::vector<int> vec_in = {5, 4, 3, 2, 7, 99, 103, 95, 187, 288};
    RB_tree a;
    a.insert_vector(vec_in);
    a.show();
    std::cout << a.size() << "\n";
    a.erase(7);
    a.insert(177);
    a.insert(100);
    a.insert(96);
    a.show();
    std::cout << a.size() << "\n";
    std::vector<int> vec = a.get_sort_vector();
    std::cout << (a.get_tree_root()->value) << "\n";
    a.clear();
    a.show();
    std::cout << a.size() << "\n";
    std::cout << vec.size() << "\n";
    for (auto i: vec)
        std::cout << i << " ";
}
