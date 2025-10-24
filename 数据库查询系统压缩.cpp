#include <bits/stdc++.h>
using namespace std;

/*
  思路：
  - 用数组索引表示节点：pre[i] 为节点值，i 为节点 id。
  - 用栈法根据先序重建 BST 的 left_child/right_child（存下标）。
  - 迭代中序遍历（基于 left_child/right_child）来标记 has_succ（中序后继是否存在）。
  - 迭代后序计算每节点子树最大值 subtree_max，以便得到 left_max = subtree_max[left_child].
  - 标记 to_delete = (orig_leaf && has_succ).
  - 先序遍历输出（跳过 to_delete 节点），并输出该节点的 stored value:
      如果原始有左子树 -> 输出 left_max；否则输出 '-'.
*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> pre(n);
    for (int i = 0; i < n; ++i) cin >> pre[i];
    if (n == 0) return 0;

    // 构建 BST 的 left_child 和 right_child（用索引）
    vector<int> left_child(n, -1), right_child(n, -1), parent(n, -1);
    {
        vector<int> st;
        st.reserve(n);
        st.push_back(0);
        for (int i = 1; i < n; ++i) {
            if (pre[i] < pre[st.back()]) {
                left_child[st.back()] = i;
                parent[i] = st.back();
                st.push_back(i);
            } else {
                int last = -1;
                while (!st.empty() && pre[i] > pre[st.back()]) {
                    last = st.back();
                    st.pop_back();
                }
                // last != -1
                right_child[last] = i;
                parent[i] = last;
                st.push_back(i);
            }
        }
    }

    // 迭代中序遍历，标记 has_succ：若某节点在中序序列中不是最后一个，则 has_succ = true
    vector<unsigned char> has_succ(n, 0);
    {
        // iterative inorder
        vector<int> st;
        st.reserve(n);
        int cur = 0; // root index is 0
        // find root: index with parent == -1 (should be 0)
        int root = 0;
        for (int i = 0; i < n; ++i) if (parent[i] == -1) { root = i; break; }
        cur = root;
        int prev = -1;
        while (!st.empty() || cur != -1) {
            while (cur != -1) {
                st.push_back(cur);
                cur = left_child[cur];
            }
            cur = st.back(); st.pop_back();
            if (prev != -1) has_succ[prev] = 1; // prev 在中序中有后继（即当前 cur）
            prev = cur;
            cur = right_child[cur];
        }
        // last node prev has no successor -> has_succ[prev] stays 0
    }

    // 迭代后序计算 subtree_max
    vector<int> subtree_max(n, INT_MIN);
    {
        int root = 0;
        for (int i = 0; i < n; ++i) if (parent[i] == -1) { root = i; break; }
        // iterative postorder using stack of (node, visited_flag)
        vector<pair<int,char>> st;
        st.reserve(n*2);
        st.emplace_back(root, 0);
        while (!st.empty()) {
            auto [u, vis] = st.back(); st.pop_back();
            if (u == -1) continue;
            if (!vis) {
                st.emplace_back(u, 1);
                if (right_child[u] != -1) st.emplace_back(right_child[u], 0);
                if (left_child[u]  != -1) st.emplace_back(left_child[u], 0);
            } else {
                int mx = pre[u];
                if (left_child[u]  != -1) mx = max(mx, subtree_max[left_child[u]]);
                if (right_child[u] != -1) mx = max(mx, subtree_max[right_child[u]]);
                subtree_max[u] = mx;
            }
        }
    }

    // 标记原始叶子、是否有左子树、是否删除(to_delete)
    vector<unsigned char> orig_leaf(n, 0), had_left(n, 0), to_delete(n, 0);
    for (int i = 0; i < n; ++i) {
        orig_leaf[i] = (left_child[i] == -1 && right_child[i] == -1) ? 1 : 0;
        had_left[i] = (left_child[i] != -1) ? 1 : 0;
        if (orig_leaf[i] && has_succ[i]) to_delete[i] = 1;
    }

    // 先序遍历输出（跳过 to_delete）。用栈迭代
    int root = 0;
    for (int i = 0; i < n; ++i) if (parent[i] == -1) { root = i; break; }

    vector<int> st;
    st.reserve(n);
    st.push_back(root);
    bool firstPrinted = false;
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        if (to_delete[u]) {
            // it's an original leaf and has inorder successor -> removed
            // since it's an original leaf, it has no children so nothing to push
            continue;
        }
        // output node u
        if (firstPrinted) cout << ' ';
        firstPrinted = true;
        cout << pre[u] << ' ';
        if (had_left[u]) cout << subtree_max[left_child[u]];
        else cout << '-';

        // push right then left so that left is processed before right (preorder)
        if (right_child[u] != -1) st.push_back(right_child[u]);
        if (left_child[u]  != -1) st.push_back(left_child[u]);
    }
    cout << '\n';
    return 0;
}
