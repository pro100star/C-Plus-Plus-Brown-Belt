struct Node {
    Node(int v, Node* p)
            : value(v)
            , parent(p)
    {}

    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent;
};

Node* min(Node* node) {
    if (node->left != nullptr) {
        return min(node->left);
    }
    return node;
}

Node* Next(Node* me) {
    if (me->parent == nullptr) {
        if (me->right == nullptr) {
            return nullptr;
        }
        return min(me->right);
    }
    if (me->parent->left == me) {
        if (me->right != nullptr) {
            return min(me->right);
        }
        return me->parent;
    }
    if (me->right != nullptr) {
        return min(me->right);
    }
    while (me->parent->right == me) {
        me = me->parent;
        if (me->parent == nullptr) {
            return nullptr;
        }
    }
    return me->parent;
}
