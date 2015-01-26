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
    bst_node *parent;
    T data;

    bst_node(const T & _x)
        :data(_x), left(nullptr), right(nullptr), parent(nullptr)
    {

    }
};

template<class T>
struct bst_iterator
{
    typedef bst_iterator<T> self;
    typedef bst_node<T> Node;
    typedef Node* NodePtr;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    NodePtr node;

    bst_iterator(NodePtr x = nullptr)
        :node(x)
    {

    }

    bst_iterator(const self &x)
        :node(x.node)
    {
        
    }

    void inor()
    {
        if (node != nullptr)
        {
            if (node->right != nullptr)
            {
                node = node->right;
                while (node->left != nullptr)
                {
                    node = node->left;
                }
            }
            else
            {
                NodePtr p_ptr = node->parent;
                while (p_ptr != nullptr && node == p_ptr->right)
                {
                    node = p_ptr;
                    p_ptr = p_ptr->parent;
                }
                node = p_ptr;
            }
        }
    }

    void deor()
    {
        if (node != nullptr)
        {
            if (node->left != nullptr)
            {
                while (node->right != nullptr)
                {
                    node = node->right;
                }
            }
            else
            {
                NodePtr p_ptr = node->parent;
                while (p_ptr != nullptr && node = p_ptr->left)
                {
                    node = p_ptr;
                    p_ptr = p_ptr->parent;
                }
                node = p_ptr;
            }
        }
    }

    bool operator== (const self &x) const
    {
        return node == x.node;
    }

    bool operator!=(const self &x) const
    {
        return node != x.node;
    }

    self& operator++()
    {
        inor();
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        inor();
        return tmp;
    }

    self& operator--()
    {
        deor();
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        deor();
        return tmp;
    }

    T operator*() const
    {
        return node->data;
    }

    T* operator->() const
    {
        return &(operator*());
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
    typedef bst_iterator<T> iterator;

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

    void insert_aux(NodePtr &ptr, NodePtr par, const T &x)
    {
        if (ptr == nullptr)
        {
            ptr = createNode(x);
            ptr->parent = par;
        }
        else if (_comp(ptr->data, x))
        {
            insert_aux(ptr->right, ptr,x);
        }
        else
        {
            insert_aux(ptr->left, ptr, x);
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
        while (ptr->left != nullptr)
        {
            ptr = ptr->left;
        }
        return ptr;
    }

    NodePtr find_max(NodePtr ptr) const
    {
        while (ptr->right != nullptr)
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
        insert_aux(_root, nullptr, x);
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

    iterator begin()
    {
        return iterator(find_min(_root));
    }

    iterator end()
    {
        return iterator(nullptr);
    }

protected:
    NodePtr _root;
    Compare _comp;

};
#endif