#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif

#ifndef _TREE_
#define _TREE_

#include "Allocator.hpp"

template<class T>
struct bst_node
{
    bst_node *left;
    bst_node *right;
    T data;

    bst_node(const T & _x, bst_node *_left = nullptr, bst_node *_right = nullptr)
        :data(_x), left(_left), right(_right)
    {

    }
};

template<class T, class Compare = std::less<T>, class Alloc = alloc>
class binary_search_tree
{

protected:
    typedef binary_search_tree<T, Compare, Alloc> self;
    typedef bst_node<T> Node;
    typedef Node* NodePtr;
    typedef simple_alloc<Node,Alloc> node_allocator;

public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

protected:

    NodePtr createNode(const T &x)
    {
        NodePtr ptr = node_allocator::allocate();
        construct(ptr, x);
        return ptr;
    }

    void destoryNode(NodePtr ptr)
    {
        destroy(ptr);
        node_allocator::deallocate(ptr);
    }

    void insert_aux(NodePtr &ptr, const T &x)
    {
        if (ptr == nullptr)
        {
            ptr = createNode(x);
        }
        else if (_comp(ptr->data, x))
        {
            insert_aux(ptr->right,x);
        }
        else
        {
            insert_aux(ptr->left, x);
        }
    }

    bool find_aux(const NodePtr &ptr, const T &x) const
    {
        if (ptr == nullptr)
        {
            return false;
        }
        else if (_comp(ptr->data, x))
        {
            return find_aux(ptr->right, x);
        }
        else if (_comp(x, ptr->data))
        {
            return find_aux(ptr->left, x);
        }
        else
        {
            return true;
        }
    }

    NodePtr find_min(NodePtr ptr) const
    {
        while (ptr != nullptr)
        {
            ptr = ptr->left;
        }
        return ptr;
    }

    NodePtr find_max(NodePtr ptr) const
    {
        while (ptr != nullptr)
        {
            ptr = ptr->right;
        }
        return ptr;
    }

    void remove_aux(NodePtr &ptr, const T &x)
    {
        if (ptr == nullptr)
        {
            return;
        }
        else if (_comp(ptr->data, x))
        {
            remove_aux(ptr->right, x);
        }
        else if (_comp(x, ptr->data))
        {
            remove_aux(ptr->left, x);
        }
        else
        {
            if (ptr->left != nullptr && ptr->right != nullptr)
            {
                NodePtr min_ptr = ptr->right;
                NodePtr min_ptr_parent = ptr;
                while (min_ptr->left != nullptr)
                {
                    min_ptr_parent = min_ptr;
                    min_ptr = min_ptr->left;
                }
                if (min_ptr == ptr->right)
                {
                    NodePtr tmp = ptr;
                    ptr->right->left = ptr->left;
                    ptr = ptr->right;
                    destoryNode(tmp);
                }
                else
                {
                    ptr->data = min_ptr->data;
                    remove_aux(min_ptr_parent->left, min_ptr_parent->left->data);
                }
            }
            else
            {
                NodePtr old_ptr = ptr;
                ptr = (ptr->left != nullptr ? ptr->left : ptr->right);
                destoryNode(old_ptr);
            }
        }
    }

    void print_aux(NodePtr ptr) const
    {
        if (ptr != nullptr)
        {
            print_aux(ptr->left);
            std::cout << ptr->data << " ";
            print_aux(ptr->right);
        }
    }

    void clear_aux(NodePtr ptr)
    {
        if (ptr == nullptr) return;
        clear_aux(ptr->left);
        clear_aux(ptr->right);
        node_allocator::deallocate(ptr);
    }

public:

    binary_search_tree()
        :_root(nullptr)
    {

    }

    binary_search_tree(Compare comp)
        :_root(nullptr), _comp(comp)
    {
    
    }

    binary_search_tree(const self &&x)
        :_root(x._root), _comp(x._comp)
    {
        x._root = nullptr;
    }

    ~binary_search_tree()
    {
        clear();
    }

    void insert(const T &x)
    {
        insert_aux(_root, x);
    }

    bool find(const T &x) const
    {
        return find_aux(_root, x);
    }

    void remove(const T &x)
    {
        remove_aux(_root, x);
    }

    void print() const
    {
        print_aux(_root);
    }

    void clear()
    {
        clear_aux(_root);
        _root = nullptr;
    }

protected:
    NodePtr _root;
    Compare _comp;

};
#endif