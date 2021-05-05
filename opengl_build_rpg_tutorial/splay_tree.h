#pragma once
#include <vector>
#include <memory>
#include <iostream>

template <typename T>
class SplayTree
{
    class Node
    {
    public:
        Node(std::size_t key, T* value)
            : key(key), value(value)
        {}

        ~Node()
        {
            delete value;
        }
        std::size_t key;
        T* value;
        Node* left, * right;
    };

    // A utility function to right
    // rotate subtree Nodeed with y
    // See the diagram given above.
    Node* right_rotate(Node* x)
    {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }

    // A utility function to left
    // rotate subtree Nodeed with x
    // See the diagram given above.
    Node* left_rotate(Node* x)
    {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // This function brings the key at
    // node if key is present in tree.
    // If key is not present, then it
    // brings the last accessed item at
    // Node. This function modifies the
    // tree and returns the new Node
    Node* splay(Node* node, std::size_t key)
    {
        // Base cases: Node is NULL or
        // key is present at Node
        if (node == nullptr || node->key == key)
            return node;

        // Key lies in left subtree
        if (node->key > key)
        {
            // Key is not in tree, we are done
            if (node->left == nullptr) return node;

            // Zig-Zig (Left Left)
            if (node->left->key > key)
            {
                // First recursively bring the
                // key as node of left-left
                node->left->left = splay(node->left->left, key);

                // Do first rotation for node,
                // second rotation is done after else
                node = right_rotate(node);
            }
            else if (node->left->key < key) // Zig-Zag (Left Right)
            {
                // First recursively bring
                // the key as node of left-right
                node->left->right = splay(node->left->right, key);

                // Do first rotation for Node->left
                if (node->left->right != nullptr)
                    node->left = left_rotate(node->left);
            }

            // Do second rotation for node
            return (node->left == nullptr) ? node : right_rotate(node);
        }
        else // Key lies in right subtree
        {
            // Key is not in tree, we are done
            if (node->right == nullptr) return node;

            // Zig-Zag (Right Left)
            if (node->right->key > key)
            {
                // Bring the key as Node of right-left
                node->right->left = splay(node->right->left, key);

                // Do first rotation for node->right
                if (node->right->left != nullptr)
                    node->right = right_rotate(node->right);
            }
            else if (node->right->key < key)// Zag-Zag (Right Right)
            {
                // Bring the key as node of
                // right-right and do first rotation
                node->right->right = splay(node->right->right, key);
                node = left_rotate(node);
            }

            // Do second rotation for Node
            return (node->right == nullptr) ? node : left_rotate(node);
        }
    }

    // A utility function to print
    // preorder traversal of the tree.
    // The function also prints height of every node
    void pre_order(Node* node)
    {
        if (node != nullptr)
        {
            std::cout << node->key << " ";
            pre_order(node->left);
            pre_order(node->right);
        }
    }

    void in_order(Node* node, std::vector<T*>& list)
    {
        if (node != nullptr)
        {
            in_order(node->left, list);
            list.push_back(node->value);
            in_order(node->right, list);
        }
    }

    Node* root_;
    std::vector<std::unique_ptr<Node>> nodes_;
public:
    SplayTree()
        : root_(nullptr)
    {}

    ~SplayTree()
    {
        nodes_.clear();
        root_ = nullptr;
    }

    SplayTree(SplayTree&&) = delete;
    SplayTree(const SplayTree&&) = delete;
    SplayTree& operator=(SplayTree&&) = delete;
    SplayTree& operator=(const SplayTree&&) = delete;

    // Function to insert a new key k
    // in splay tree with given root
    T* insert(std::size_t k, T* value)
    {
        // Simple Case: If tree is empty
        if (root_ == nullptr)
        {
            root_ = new Node(k, value);
            nodes_.push_back(std::unique_ptr<Node>(root_));
            return value;
        }

        // Bring the closest leaf node to root
        root_ = splay(root_, k);

        // If key is already present, then return
        if (root_->key == k)
        {
            delete value;
            return root_->value;
        }
        // Otherwise allocate memory for new Node
        Node* new_node = new Node(k, value);

        // If root's key is greater, make
        // root as right child of new node
        // and copy the left child of root to new Node
        if (root_->key > k)
        {
            new_node->right = root_;
            new_node->left = root_->left;
            root_->left = nullptr;
        }

        // If root's key is smaller, make
        // root as left child of new Node
        // and copy the right child of root to new Node
        else
        {
            new_node->left = root_;
            new_node->right = root_->right;
            root_->right = nullptr;
        }

        root_ = new_node;
        nodes_.push_back(std::unique_ptr<Node>(new_node));
        return value; // new node becomes new root
    }

    T* insert(T* value)
    {
        return insert(nodes_.size(), value);
    }

    void remove(std::size_t key)
    {
        Node* temp;
        if (!root_)
            return;

        // Splay the given key    
        root_ = splay(root_, key);

        // If key is not present, then
        // return root
        if (key != root_->key)
            return;

        // If key is present
        // If left child of root does not exist
        // make root->right as root   
        if (!root_->left)
        {
            temp = root_;
            root_ = root_->right;
        }

        // Else if left child exits
        else
        {
            temp = root_;

            /*Note: Since key == root->key,
            so after Splay(key, root->lchild),
            the tree we get will have no right child tree
            and maximum node in left subtree will get splayed*/
            // New root
            root_ = splay(root_->left, key);

            // Make right child of previous root  as
            // new root's right child
            root_->right = temp->right;
        }

        for (auto i = 0; i < nodes_.size(); ++i)
            if (nodes_[i]->key == key)
            {
                nodes_.erase(nodes_.begin() + i);
                break;
            }

        // return root of the new Splay Tree
        return;
    }

    T* search(std::size_t key)
    {
        root_ = splay(root_, key);

        if (root_)
            if (root_->key == key)
                return root_->value;

        return nullptr;
    }

    std::vector<T*> get_ordered_list()
    {
        std::vector<T*> temp_list;
        in_order(root_, temp_list);
        return temp_list;
    }

    std::size_t size()
    {
        return nodes_.size();
    }

    void print()
    {
        pre_order(root_);
        std::cout << std::endl;
    }

};
