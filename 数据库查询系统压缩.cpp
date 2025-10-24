#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node *left, *right;
    int left_max;
    bool had_left, orig_leaf, del;
    Node(int v): val(v), left(nullptr), right(nullptr),
                 left_max(-1), had_left(false),
                 orig_leaf(false), del(false) {}
};

// 用先序建 BST（O(n)）
Node* buildBST(const vector<int>& pre) {
    if (pre.empty()) return nullptr;
    Node* root = new Node(pre[0]);
    stack<Node*> st;
    st.push(root);
    for (int i = 1; i < (int)pre.size(); ++i) {
        Node* node = new Node(pre[i]);
        if (pre[i] < st.top()->val) {
            st.top()->left = node;
            st.push(node);
        } else {
            Node* parent = nullptr;
            while (!st.empty() && pre[i] > st.top()->val) {
                parent = st.top();
                st.pop();
            }
            parent->right = node;
            st.push(node);
        }
    }
    return root;
}

// 计算每个节点的 left_max、had_left、orig_leaf
int postOrder(Node* root) {
    if (!root) return INT_MIN;
    int lmax = postOrder(root->left);
    int rmax = postOrder(root->right);
    if (root->left) {
        root->had_left = true;
        root->left_max = lmax;
    }
    root->orig_leaf = (!root->left && !root->right);
    return max(root->val, max(lmax, rmax));
}

// Morris 中序遍历：标记哪些原始叶子有中序后继（应删除）
void markDeleteByMorris(Node* root) {
    Node* cur = root;
    Node* prev = nullptr;
    Node* last = nullptr;
    while (cur) {
        if (!cur->left) {
            // 访问 cur
            if (last && last->orig_leaf) last->del = true; // 有后继
            last = cur;
            cur = cur->right;
        } else {
            prev = cur->left;
            while (prev->right && prev->right != cur) prev = prev->right;
            if (!prev->right) {
                prev->right = cur;
                cur = cur->left;
            } else {
                prev->right = nullptr;
                if (last && last->orig_leaf) last->del = true;
                last = cur;
                cur = cur->right;
            }
        }
    }
    // 最后一个节点无后继，不删除
}

// 压缩后先序输出（跳过删除节点）
void preorderOutput(Node* root) {
    if (!root) return;
    stack<Node*> st;
    st.push(root);
    bool first = true;
    while (!st.empty()) {
        Node* p = st.top(); st.pop();
        if (p->del) continue;  // 被删除的叶子节点跳过
        if (!first) cout << ' ';
        first = false;
        cout << p->val << ' ';
        if (p->had_left) cout << p->left_max;
        else cout << '-';
        if (p->right) st.push(p->right);
        if (p->left) st.push(p->left);
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> pre(n);
    for (int i = 0; i < n; ++i) cin >> pre[i];

    Node* root = buildBST(pre);
    postOrder(root);
    markDeleteByMorris(root);
    preorderOutput(root);
    return 0;
}
