#pragma once
#include <glad\glad.h>
#include <iostream>

/*
Copy and pasted (mostly) verbatim from geeks for geeks. Simple one class templated splay tree solution
   * https://www.geeksforgeeks.org/splay-tree-set-1-insert/ // right_rotate, left_rotate, splay, pre_order, and search functions
   * https://www.geeksforgeeks.org/splay-tree-set-2-insert-delete/ // insert function
@author GeeksForGeeks, David Wadsworth
*/

namespace DataStructures
{
    template <typename T>
	class SplayTree
	{
        class Node
        {
        public:
            Node(GLint key, T* value)
                : key(key), value(value), left(nullptr), right(right)
            {}

            ~Node()
            {
                delete value;
            }

            GLint key;
            T* value;
            Node* left, * right;
        };

        // A utility function to right  
        // rotate subtree rooted with y  
        // See the diagram given above.  
        Node* right_rotate(Node* x)
        {
            Node* y = x->left;
            x->left = y->right;
            y->right = x;
            return y;
        }

        // A utility function to left  
        // rotate subtree rooted with x  
        // See the diagram given above.  
        Node* left_rotate(Node* x)
        {
            Node* y = x->right;
            x->right = y->left;
            y->left = x;
            return y;
        }

        // This function brings the key at 
        // root if key is present in tree.  
        // If key is not present, then it 
        // brings the last accessed item at  
        // root. This function modifies the 
        // tree and returns the new root  
        Node* splay(Node* root, int key)
        {
            // Base cases: root is NULL or 
            // key is present at root  
            if (root == nullptr || root->key == key)
                return root;

            // Key lies in left subtree  
            if (root->key > key)
            {
                // Key is not in tree, we are done  
                if (root->left == nullptr)
                    return root;
                
                // Zig-Zig (Left Left)  
                if (root->left->key > key)
                {
                    // First recursively bring the 
                    // key as root of left-left  
                    root->left->left = splay(root->left->left, key);

                    // Do first rotation for root,  
                    // second rotation is done after else  
                    root = right_rotate(root);
                }
                else if (root->left->key < key) // Zig-Zag (Left Right)  
                {
                    // First recursively bring 
                    // the key as root of left-right  
                    root->left->right = splay(root->left->right, key);

                    // Do first rotation for root->left  
                    if (root->left->right != nullptr)
                        root->left = left_rotate(root->left);
                }

                // Do second rotation for root  
                return (root->left == nullptr) ? root : right_rotate(root);
            }
            else // Key lies in right subtree  
            {
                // Key is not in tree, we are done  
                if (root->right == nullptr)
                    return root;
                
                // Zag-Zig (Right Left)  
                if (root->right->key > key)
                {
                    // Bring the key as root of right-left  
                    root->right->left = splay(root->right->left, key);

                    // Do first rotation for root->right  
                    if (root->right->left != nullptr)
                        root->right = right_rotate(root->right);
                }
                else if (root->right->key < key)// Zag-Zag (Right Right)  
                {
                    // Bring the key as root of  
                    // right-right and do first rotation  
                    root->right->right = splay(root->right->right, key);
                    root = left_rotate(root);
                }

                // Do second rotation for root  
                return (root->right == nullptr) ? root : left_rotate(root);
            }
        }
        // A utility function to print  
        // pre order traversal of the tree.  
        // The function also prints height of every Node  
        void pre_order(Node* node)
        {
            if (node != nullptr)
            {
                std::cout << node->key << " ";
                pre_order(node->left);
                pre_order(node->right);
            }
        }

        void pre_order(Node* node, std::vector<T*>& list)
        {
            if (node != nullptr)
            {
                pre_order(node->left, list);
                list.push_back(node->value);
                pre_order(node->right, list);
            }
        }

        GLuint size_;
        Node* root_;
        std::vector<std::unique_ptr<Node>> nodes_;
    public:
        SplayTree()
            : root_(nullptr), size_(0)
        {}

        ~SplayTree()
        {
            nodes_.clear();
            root_ = nullptr;
        }

        T* insert(int key, T* value)
        {
            ++size_;

            // Simple Case: If tree is empty  
            if (root_ == nullptr) 
            {
               root_ = new Node(key, value);
               nodes_.push_back(std::unique_ptr<Node>(root_));
               return value;
            }
            // Bring the closest leaf node to root  
            root_ = splay(root_, key);

            // If key is already present, then return  
            if (root_->key == key)
            {
                --size_;
                delete value;
                return root_->value;
            }
            // Otherwise allocate memory for new node  
            auto new_node = new Node(key, value);

            // If root's key is greater, make  
            // root as right child of newnode  
            // and copy the left child of root to newnode  
            if (root_->key > key)
            {
                new_node->right = root_;
                new_node->left = root_->left;
                root_->left = nullptr;
            }

            // If root's key is smaller, make  
            // root as left child of newnode  
            // and copy the right child of root to newnode  
            else
            {
                new_node->left = root_;
                new_node->right = root_->right;
                root_->right = nullptr;
            }

            root_ = new_node;
            nodes_.push_back(std::unique_ptr<Node>(new_node));
            return value;
        }

        T* insert(T* value)
        {
            return insert(size_, value);
        }

        // The search function for Splay tree.  
        // Note that this function returns the  
        // new root of Splay Tree. If key is  
        // present in tree then, it is moved to root.  
        T* search(int key)
        {
            root_ = splay(root_, key);

            if (root_)
                if (root_->key == key)
                    return root_->value;

            return nullptr;
        }

        // returns an array of ordered values in the binary tree
        std::vector<T*> get_ordered_list()
        {
            std::vector<T*> return_list;
            pre_order(root_, return_list);
            return return_list;
        }

        GLuint size()
        {
            return size_;
        }

        void print()
        {
            pre_order(root_);
            std::cout << std::endl;
        }
	};

}
