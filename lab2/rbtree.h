#pragma once
#include <memory>
#define STRUCT typedef struct

typedef long long rbtree_key_t;

STRUCT node node;

struct node{
    node* parent;
    node* left;
    node* right;
    unsigned long long color: 1, key: 63;
};

STRUCT {
    node* root;
} rbtree;

enum RBColor {black = 0, red = 1};

void rbtree_null_node_create();
void rbtree_null_node_destroy();
rbtree* construct_rbtree();
node* rbtree_insert(rbtree* tree, rbtree_key_t key);
void rbtree_insert_fixup(rbtree* tree, node* z);
void rbtree_left_rotate(rbtree* tree, node* z);
void rbtree_right_rotate(rbtree* tree, node* z);
void rbtree_delete(rbtree* tree, node* z);
void rbtree_transplant(rbtree* tree, node* z, node* v);
void rbtree_delete_fixup(rbtree* tree, node* z);
node* rbtree_minimun(node* root);
node* rbtree_find(rbtree* tree, rbtree_key_t key);
void rbtree_destroy(rbtree* tree);

node* rbtree_null_node;

void rbtree_null_node_create()
{
    node* res = (node*)malloc(sizeof(node));;
    res->color = black;
    res->key = 0;
    res->parent = NULL;
    res->left = NULL;
    res->right = NULL;
    rbtree_null_node = res;
}

void rbtree_null_node_destroy()
{
    free(rbtree_null_node);
}

rbtree* construct_rbtree()
{
    rbtree* res = (rbtree*)malloc(sizeof(rbtree));
    res->root = rbtree_null_node;
    return res;
}

void rbtree_destroy(rbtree* tree)
{
    node* current = tree->root;
    node* prev;
    while (current != rbtree_null_node)
    {
        if(current->left == rbtree_null_node){
            if(current->right == rbtree_null_node){
                prev = current;
                current = current->parent;
                free(prev);
            } else{
                prev = current;
                current = current->right;
                current->parent = prev->parent;
                free(prev);
            }
        } else{
            if(current->right == rbtree_null_node){
                prev = current;
                current = current->left;
                current->parent = prev->parent;
                free(prev);
            } else{
                prev = current;
                current = current->left;
                current->parent = prev->right;
                prev->right->parent = prev->parent;
                free(prev);
            }
        }
    }
    free(tree);
}

node* rbtree_insert(rbtree* tree, rbtree_key_t key)
{
    node* z = (node*)malloc(sizeof(node));
    z->key = key;
    node* y = rbtree_null_node;
    node* x = tree->root;
    while (x != rbtree_null_node)
    {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;}
    z->parent = y;
    if (y == rbtree_null_node) tree->root = z;
    else if(z->key < y->key) y->left = z;
    else y->right = z;
    z->left = rbtree_null_node;
    z->right = rbtree_null_node;
    z->color = red;
    rbtree_insert_fixup(tree, z);
    return z;
}

void rbtree_insert_fixup(rbtree* tree, node* z)
{
    while (z->parent->color == red)
    {
        if (z->parent == z->parent->parent->left){
            node* y = z->parent->parent->right;
            if(y->color == red){
                z->parent->color = black;
                y->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else{
                if (z == z->parent->right){
                z = z->parent;
                rbtree_left_rotate(tree, z);
                }
                z->parent->color = black;
                z->parent->parent->color = red;
                rbtree_right_rotate(tree, z->parent->parent);
            }
        }
        else{
            node* y = z->parent->parent->left;
            if(y->color == red){
                z->parent->color = black;
                y->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else{
                if (z == z->parent->left){
                z = z->parent;
                rbtree_right_rotate(tree, z);
                }
                z->parent->color = black;
                z->parent->parent->color = red;
                rbtree_left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = black;
}

void rbtree_left_rotate(rbtree* tree, node* z)
{
    node* y = z->right;
    z->right = y->left;
    if(y->left != rbtree_null_node) y->left->parent = z;
    y->parent = z->parent;
    if(z->parent == rbtree_null_node) tree->root = y;
    else if(z == z->parent->left) z->parent->left = y;
    else z->parent->right = y;
    y->left = z;
    z->parent = y;
}

void rbtree_right_rotate(rbtree* tree, node* z)
{
    node* y = z->parent;
    z->parent = y->parent;
    if(z->right != rbtree_null_node) z->right->parent = y;
    y->left = z->right;
    if(y->parent == rbtree_null_node) tree->root = z;
    else if(y == y->parent->left) y->parent->left = z;
    else y->parent->right = z;
    y->parent = z;
    z->right = y;
}

void rbtree_delete(rbtree* tree, node* z)
{
    node* y = z;
    char y_oridinal_color = y->color;
    node* x;
    if(z->left == rbtree_null_node){
        x = z->right;
        rbtree_transplant(tree, z, z->right);
    } else if(z->right == rbtree_null_node){
        x = z->left;
        rbtree_transplant(tree, z, z->left);
    } else{
        y = rbtree_minimun(z->right);
        y_oridinal_color = y->color;
        x = y->right;
        if(y->parent == z){
            x->parent = y;
        } else{
            rbtree_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbtree_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_oridinal_color == black) rbtree_delete_fixup(tree, x);
    free(z);
}

void rbtree_transplant(rbtree* tree, node* z, node* v)
{
    if(z->parent == rbtree_null_node) tree->root = v;
    else if(z == z->parent->left) z->parent->left = v;
    else z->parent->right = v;
    v->parent = z->parent;
}

void rbtree_delete_fixup(rbtree* tree, node* x)
{
    while ((x != tree->root) && (x->color == black))
    {
        if(x == x->parent->left){
            node* w = x->parent->right;
            if(w->color == red){
                w->color = black;
                x->parent->color = red;
                rbtree_left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if((w->left->color == black) && (w->right->color == black)){
                w->color = red;
                x = x->parent;
            } else{
                if(w->right->color == black){
                    w->left->color = black;
                    w->color = red;
                    rbtree_right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = black;
                w->right->color = black;
                rbtree_left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else{
            node* w = x->parent->left;
            if(w->color == red){
                w->color = black;
                x->parent->color = red;
                rbtree_right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if((w->right->color == black) && (w->left->color == black)){
                w->color = red;
                x = x->parent;
            } else{
                if(w->left->color == black){
                    w->right->color = black;
                    w->color = red;
                    rbtree_left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = black;
                w->left->color = black;
                rbtree_right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = black;
}

node* rbtree_minimun(node* root)
{
    while (root->left != rbtree_null_node)
    {
        root = root->left;
    }
    return root;
}

node* rbtree_find(rbtree* tree, rbtree_key_t key)
{
    node* x = tree->root;
    while(x != rbtree_null_node){
        if(x->key == key) return x;
        if(key < x->key) x = x->left;
        else x = x->right;
    }
    return rbtree_null_node;
}
