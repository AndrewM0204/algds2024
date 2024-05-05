#include "rbtree.h"
#include <gtest/gtest.h>

bool root_is_black(rbtree* tree)
{
    return tree->root->color == black;
}

bool red_child_is_black(rbtree* tree)
{
    node* cur = tree->root;
    node* prev;
    while (cur != rbtree_null_node)
    {
        if(cur->left == rbtree_null_node){
            if(cur->color == red){
                if(!(cur->left->color == black) || !(cur->right->color == black)) return false;
            }
            cur = cur->right;
        } else{
            prev = cur->left;
            while ((prev->right != rbtree_null_node) && (prev->right != cur))
            {
                prev = prev->right;
            }
            if(prev == rbtree_null_node){
                prev->right = cur;
                cur = cur->left;
            } else{
                prev->right = rbtree_null_node;
                if(cur->color == red){
                if(!(cur->left->color == black) || !(cur->right->color == black)) return false;
                }
                cur = cur->right;
            }
        }
    }
    return true;
}

int black_heigth(node* root){
    if(root == rbtree_null_node) return 1;
    else if(root->color == red){
        int x = black_heigth(root->left);
        if(x == black_heigth(root->right)) return x;
        else return -1;
    } else{
        int x = black_heigth(root->left);
        if(x == black_heigth(root->right)) return x + 1;
        else return -1;
    }
}

bool black_heigth_all_path(rbtree* tree)
{
    return (black_heigth(tree->root) != -1);
}

TEST(rbtree_insert, root_is_black)
{
    rbtree_null_node_create();
    rbtree* tree = construct_rbtree();
    rbtree_insert(tree, 1);
    rbtree_insert(tree, 2);
    rbtree_insert(tree, 3);
    rbtree_insert(tree, 4);
    rbtree_insert(tree, 5);

    EXPECT_TRUE(root_is_black(tree));

    rbtree_destroy(tree);
    rbtree_null_node_destroy();
}

TEST(rbtree_insert, red_child_is_black)
{
    rbtree_null_node_create();
    rbtree* tree = construct_rbtree();
    rbtree_insert(tree, 1);
    rbtree_insert(tree, 2);
    rbtree_insert(tree, 3);
    rbtree_insert(tree, 4);
    rbtree_insert(tree, 5);

    EXPECT_TRUE(red_child_is_black(tree));

    rbtree_destroy(tree);
    rbtree_null_node_destroy();
}

TEST(rbtree_insert, black_heigth_all_path)
{
    rbtree_null_node_create();
    rbtree* tree = construct_rbtree();
    rbtree_insert(tree, 1);
    rbtree_insert(tree, 2);
    rbtree_insert(tree, 3);
    rbtree_insert(tree, 4);
    rbtree_insert(tree, 5);

    EXPECT_TRUE(black_heigth_all_path(tree));

    rbtree_destroy(tree);
    rbtree_null_node_destroy();
}

TEST(rbtree_delete, root_is_black)
{
    rbtree_null_node_create();
    rbtree* tree = construct_rbtree();
    for(int i = 1; i < 15; ++i){
        rbtree_insert(tree, i);
    }
    rbtree_delete(tree, rbtree_find(tree, 4));

    EXPECT_TRUE(root_is_black(tree));

    rbtree_destroy(tree);
    rbtree_null_node_destroy();
}

TEST(rbtree_delete, red_child_is_black)
{
    rbtree_null_node_create();
    rbtree* tree = construct_rbtree();
    for(int i = 1; i < 15; ++i){
        rbtree_insert(tree, i);
    }
    rbtree_delete(tree, rbtree_find(tree, 4));

    EXPECT_TRUE(red_child_is_black(tree));

    rbtree_destroy(tree);
    rbtree_null_node_destroy();
}

TEST(rbtree_delete, black_heigth_all_path)
{
    rbtree_null_node_create();
    rbtree* tree = construct_rbtree();
    for(int i = 1; i < 15; ++i){
        rbtree_insert(tree, i);
    }
    rbtree_delete(tree, rbtree_find(tree, 4));

    EXPECT_TRUE(black_heigth_all_path(tree));

    rbtree_destroy(tree);
    rbtree_null_node_destroy();
}
