//
// Created by Andrew on 16.11.2020.
//

/** алгоритм балансировки кч-дерева взят с
    https://ru.wikipedia.org/wiki/%D0%9A%D1%80%D0%B0%D1%81%D0%BD%D0%BE-%D1%87%D1%91%D1%80%D0%BD%D0%BE%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE
*/

#ifndef RB_TREE_H
#define RB_TREE_H

#pragma GCC system_header

#include <vector>

class RB_tree {
    struct node_st {int value; bool color_red; node_st *leftBranch, *rightBranch, *parent;};
    /** структура узла(вершины): значение в узле, цвет узла, правый и левый потомки, "родитель" */

private:
    node_st *tree_root;                                /** корень дерева */
    int tree_size;                                     /** число узлов дерева */

private:
    node_st *CreateNewNode(int);                       /** создание новой вершины с заданным value */
    void DeleteNode(node_st*);                         /** удаление вершины */
    void Clear(node_st*);                              /** очистка всего дерева */
    static node_st *GetGrandparent(node_st*);          /** возвращает указатель на "дедушку" */
    static node_st *GetUncle(node_st*);                /** возвращает указатель на "дядю" */
    static node_st *GetSibling(node_st*);              /** возвращает указатель на "брата" */
    static void ReplaceNode(node_st*, node_st*);       /** меняет две вершины дерева местами */
    void DeleteOneChild(node_st*);                     /** удаляет потомка (не более одного) */
    void RotateLeft(node_st*);                         /** вращение дерева влево */
    void RotateRight(node_st*);                        /** вращение дерева вправо */
    void Insert(int);                                  /** вставка нового элемента в дерево */
    void BalanceInsert(node_st*);                      /** балансировка вставки */
    void Show(node_st*, int, char);                    /** вывести красно-черное дерево */
    node_st *FindNode(int);                            /** найти узел с известным значением  */
    void Erase(int);                                   /** удалить элемент из дерева по его value */
    void BalanceErase(node_st*);                       /** балансировка удаления */
    void GetSortVector(node_st*, std::vector<int>*);   /** рекурсивный вывод в вектор */
    void InsertVector(std::vector<int>*);              /** ввод элементов вектора в кч-дерево */

public:
    RB_tree();
    ~RB_tree();
    void clear();			                           /** отчистить дерево */
    bool find(int);			                           /** найти значение */
    void insert(int);		                           /** вставить значение */
    void erase(int);		                           /** удалить значение */
    void show();                                       /** изобразить всё дерево */
    std::vector<int> get_sort_vector();                /** возвращает вектор элементов по возрастанию */
    void insert_vector(std::vector<int>);              /** вводит элементы вектора в кч-дерево */
    int size() const;	                               /** узнать число узлов дерева */
    node_st* get_tree_root();                          /** возвращает указатель на корень дерева */
};

RB_tree::RB_tree()
{
    tree_root = nullptr;
    tree_size = 0;
}

RB_tree::~RB_tree()
{
    Clear(tree_root);
    tree_root = nullptr;
}

RB_tree::node_st *RB_tree::CreateNewNode(int value)
{
    auto *node = new node_st;
    node->value = value;
    node->leftBranch = nullptr;
    node->rightBranch = nullptr;
    node->parent = nullptr;
    node->color_red = true;
    tree_size++;
    return node;
}

void RB_tree::DeleteNode(node_st *node)
{
    tree_size--;
    if (node->parent != nullptr) {
        if (node->parent->leftBranch == node)
            node->parent->leftBranch = nullptr;
        else
            node->parent->rightBranch = nullptr;
    }
    else
        tree_root = nullptr;
    delete node;
}

void RB_tree::Clear(node_st *node)
{
    if (node == nullptr)
        return;
    Clear(node->leftBranch);
    Clear(node->rightBranch);
    DeleteNode(node);
}

RB_tree::node_st *RB_tree::GetGrandparent(node_st *node)
{
    if (node->parent != nullptr) {
        return node->parent->parent;
    }
    else {
        return nullptr;
    }
}

RB_tree::node_st *RB_tree::GetUncle(node_st *node)
{
    node_st *grandparent = GetGrandparent(node);
    if (grandparent == nullptr)
        return nullptr;
    if (node->parent == grandparent->leftBranch)
        return grandparent->rightBranch;
    else
        return grandparent->leftBranch;
}

RB_tree::node_st *RB_tree::GetSibling(node_st *node)
{
    if (node == node->parent->leftBranch)
        return node->parent->rightBranch;
    else
        return node->parent->leftBranch;
}

void RB_tree::ReplaceNode(node_st *node, node_st *child) {
    child->parent = node->parent;
    if (node == node->parent->leftBranch) {
        node->parent->leftBranch = child;
    } else {
        node->parent->rightBranch = child;
    }
}

void RB_tree::DeleteOneChild(node_st *node)
{
    node_st *child = (node->rightBranch == nullptr) ? node->leftBranch : node->rightBranch;
    if (child == nullptr) {
        BalanceErase(node);
        DeleteNode(node);
        return;
    }
    ReplaceNode(node, child);
    if (!node->color_red) {
        if (child->color_red)
            child->color_red = false;
        else
            BalanceErase(child);
    }
    DeleteNode(node);
}

void RB_tree::RotateLeft(node_st *node)
{
    node_st *swap = node->rightBranch;
    swap->parent = node->parent;
    if (node->parent != nullptr) {
        if (node->parent->leftBranch == node)
            node->parent->leftBranch = swap;
        else
            node->parent->rightBranch = swap;
    }
    else
        tree_root = swap;
    node->rightBranch = swap->leftBranch;
    if (swap->leftBranch != nullptr)
        swap->leftBranch->parent = node;
    node->parent = swap;
    swap->leftBranch = node;
}

void RB_tree::RotateRight(node_st *node)
{
    node_st *swap = node->leftBranch;
    swap->parent = node->parent;
    if (node->parent != nullptr) {
        if (node->parent->leftBranch == node)
            node->parent->leftBranch = swap;
        else
            node->parent->rightBranch = swap;
    }
    else
        tree_root = swap;
    node->leftBranch = swap->rightBranch;
    if (swap->rightBranch != nullptr)
        swap->rightBranch->parent = node;
    node->parent = swap;
    swap->rightBranch = node;
}

void RB_tree::Insert(int key)
{
    node_st *add = CreateNewNode(key);
    if (tree_root == nullptr) {
        tree_root = add;
        BalanceInsert(add);
        return;
    }
    if (find(key))
        return;
    node_st *node = tree_root;
    while ((node->leftBranch != nullptr || node->value < add->value)
           && (node->rightBranch != nullptr || node->value > add->value)) {
        if (node->value < add->value)
            node = node->rightBranch;
        else
            node = node->leftBranch;
    }
    if (node->value < add->value)
        node->rightBranch = add;
    else
        node->leftBranch = add;
    add->parent = node;
    BalanceInsert(add); // проверяем, не нарушены ли свойства красно-черного дерева
}

void RB_tree::BalanceInsert(node_st *node)
{
    if (node->parent == nullptr)
        node->color_red = false;
    else {
        if (!node->parent->color_red) {
            return;
        }
        else {
            node_st *u = GetUncle(node), *g;
            if ((u != nullptr) && (u->color_red)) {
                node->parent->color_red = false;
                u->color_red = false;
                g = GetGrandparent(node);
                g->color_red = true;
                BalanceInsert(g);
            } else {
                g = GetGrandparent(node);
                if ((node == node->parent->rightBranch) && (node->parent == g->leftBranch)) {
                    RotateLeft(node->parent);
                    node = node->leftBranch;
                } else if ((node == node->parent->leftBranch) && (node->parent == g->rightBranch)) {
                    RotateRight(node->parent);
                    node = node->rightBranch;
                }
                g = GetGrandparent(node);
                node->parent->color_red = false;
                g->color_red = true;
                if ((node == node->parent->leftBranch) && (node->parent == g->leftBranch)) {
                    RotateRight(g);
                } else {
                    RotateLeft(g);
                }
            }
        }
    }
}

void RB_tree::Show(node_st *node, int depth, char dir)
{
    int n;    //  вывод не мой, но он классный
    if (!node) return;
    for (n=0; n<depth; n++) putchar(' ');
    printf("%c[%d] (%s)\n", dir, node->value, node->color_red? "red":"black");
    Show(node->leftBranch, depth + 2, '-');
    Show(node->rightBranch, depth + 2, '+');
}

RB_tree::node_st *RB_tree::FindNode(int value)
{
    node_st *node = tree_root;
    while (node) {
        if (node->value == value) return node;
        node = node->value > value ? node->leftBranch:node->rightBranch;
    }
    return nullptr;
}

void RB_tree::Erase(int key)
{
    if (FindNode(key) == nullptr)
        return;
    node_st *node = FindNode(key), *change;
    int swap;
    if (node->rightBranch != nullptr) {
        change = node->rightBranch;
        while (change->leftBranch != nullptr) {
            change = change->leftBranch;
        }
    }
    else if (node->leftBranch != nullptr) {
        change = node->leftBranch;
        while (change->rightBranch != nullptr) {
            change = change->rightBranch;
        }
    }
    else {
        if (node->parent == nullptr)
            tree_root = nullptr;
        DeleteNode(node);
        return;
    }
    swap = node->value;
    node->value = change->value;
    change->value = swap;
    DeleteOneChild(change);
}

void RB_tree::BalanceErase(node_st *node)
{
    if (node->parent != nullptr) {
        node_st *s = GetSibling(node);
        if (s->color_red) {
            node->parent->color_red = true;
            s->color_red = false;
            if (node == node->parent->leftBranch)
                RotateLeft(node->parent);
            else
                RotateRight(node->parent);
        }
        s = GetSibling(node);
        if (!node->parent->color_red &&
            !s->color_red &&
            !s->leftBranch->color_red &&
            !s->rightBranch->color_red) {
            s->color_red = true;
            BalanceErase(node->parent);
        } else {
            s = GetSibling(node);
            if (node->parent->color_red &&
                !s->color_red &&
                !s->leftBranch->color_red &&
                !s->rightBranch->color_red) {
                s->color_red = true;
                node->parent->color_red = false;
            } else {
                s = GetSibling(node);
                if  (!s->color_red) {
                    if ((node == node->parent->leftBranch) &&
                        !s->rightBranch->color_red &&
                        s->leftBranch->color_red) {
                        s->color_red = true;
                        s->leftBranch->color_red = false;
                        RotateRight(s);
                    } else if ((node == node->parent->rightBranch) &&
                               !s->leftBranch->color_red &&
                               s->rightBranch->color_red) {
                        s->color_red = true;
                        s->rightBranch->color_red = false;
                        RotateLeft(s);
                    }
                }
                s = GetSibling(node);
                s->color_red = node->parent->color_red;
                node->parent->color_red = false;
                if (node == node->parent->leftBranch) {
                    s->rightBranch->color_red = false;
                    RotateLeft(node->parent);
                } else {
                    s->leftBranch->color_red = false;
                    RotateRight(node->parent);
                }
            }
        }

    }
}

void RB_tree::GetSortVector(node_st* node, std::vector<int>* vec)
{
    if (node->leftBranch != nullptr)
        GetSortVector(node->leftBranch, vec);
    vec->push_back(node->value);
    if (node->rightBranch != nullptr)
        GetSortVector(node->rightBranch, vec);
}

void RB_tree::InsertVector(std::vector<int>* vec)
{
    for (auto elem: *vec)
        Insert(elem);
}

void RB_tree::clear()
{
    Clear(tree_root);
    tree_root = nullptr;
}

bool RB_tree::find(int value)
{
    node_st *node = tree_root;
    while (node) {
        if (node->value == value) return true;
        node = node->value > value ? node->leftBranch:node->rightBranch;
    }
    return false;
}

void RB_tree::insert(int value)
{
    Insert(value);
}

void RB_tree::erase(int value)

{
    Erase(value);
}

void RB_tree::show()
{
    Show(tree_root,0,'*');
}

std::vector<int> RB_tree::get_sort_vector()
{
    std::vector<int> sort_vec;
    GetSortVector(tree_root, &sort_vec);
    return sort_vec;
}

void RB_tree::insert_vector(std::vector<int> vec)
{
    InsertVector(&vec);
}

int RB_tree::size() const
{
    return tree_size;
}

RB_tree::node_st* RB_tree::get_tree_root()
{
    if (tree_root != nullptr)
        return tree_root;
    else return nullptr;
}

#endif //RB_TREE_H
