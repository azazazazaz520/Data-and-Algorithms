#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100000 + 5;
int pre[MAXN];
int leftc[MAXN], rightc[MAXN], parent_[MAXN];
int left_maxv[MAXN];
bool had_left[MAXN], orig_leaf[MAXN], del_[MAXN];
int n;

// ---------- 构建 BST (O(n)) ----------
int buildBST() {
    vector<int> st;
    st.reserve(n);
    int root = 0;
    st.push_back(0);
    for (int i = 1; i < n; ++i) {
        if (pre[i] < pre[st.back()]) {
            leftc[st.back()] = i;
            parent_[i] = st.back();
            st.push_back(i);
        } else {
            int last = -1;
            while (!st.empty() && pre[i] > pre[st.back()]) {
                last = st.back();
                st.pop_back();
            }
            rightc[last] = i;
            parent_[i] = last;
            st.push_back(i);
        }
    }
    return root;
}

// ---------- 后序计算 left_maxv、had_left、orig_leaf ----------
void computeLeftMax(int root) {
    vector<pair<int,int> > st;
    st.reserve(n*2);
    st.push_back(make_pair(root, 0));
    while (!st.empty()) {
        pair<int,int> top = st.back();
        st.pop_back();
        int u = top.first;
        int vis = top.second;
        if (u == -1) continue;
        if (!vis) {
            st.push_back(make_pair(u, 1));
            if (rightc[u] != -1) st.push_back(make_pair(rightc[u], 0));
            if (leftc[u] != -1)  st.push_back(make_pair(leftc[u], 0));
        } else {
            int mx = pre[u];
            if (leftc[u] != -1) {
                had_left[u] = true;
                mx = max(mx, left_maxv[leftc[u]]);
                left_maxv[u] = left_maxv[leftc[u]];
            }
            if (rightc[u] != -1) {
                mx = max(mx, left_maxv[rightc[u]]);
            }
            left_maxv[u] = mx;
            orig_leaf[u] = (leftc[u] == -1 && rightc[u] == -1);
        }
    }
}

// ---------- Morris 中序标记删除 ----------
void markDelete(int root) {
    int cur = root;
    int last = -1;
    while (cur != -1) {
        if (leftc[cur] == -1) {
            if (last != -1 && orig_leaf[last]) del_[last] = true;
            last = cur;
            cur = rightc[cur];
        } else {
            int prev = leftc[cur];
            while (rightc[prev] != -1 && rightc[prev] != cur)
                prev = rightc[prev];
            if (rightc[prev] == -1) {
                rightc[prev] = cur; // 建立线索
                cur = leftc[cur];
            } else {
                rightc[prev] = -1;  // 恢复
                if (last != -1 && orig_leaf[last]) del_[last] = true;
                last = cur;
                cur = rightc[cur];
            }
        }
    }
    if (last != -1) del_[last] = false; // 最后节点无后继，不删
}

// ---------- 先序输出 ----------
void preorderOut(int root) {
    vector<int> st;
    st.reserve(n);
    st.push_back(root);
    bool first = true;
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        if (del_[u]) continue;
        if (!first) cout << ' ';
        first = false;
        cout << pre[u] << ' ';
        if (had_left[u]) cout << left_maxv[leftc[u]];
        else cout << '-';
        if (rightc[u] != -1) st.push_back(rightc[u]);
        if (leftc[u] != -1)  st.push_back(leftc[u]);
    }
    cout << '\n';
}

// ---------- 主函数 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> pre[i];
        leftc[i] = rightc[i] = parent_[i] = -1;
        had_left[i] = orig_leaf[i] = del_[i] = false;
        left_maxv[i] = pre[i];
    }

    int root = buildBST();
    computeLeftMax(root);
    markDelete(root);
    preorderOut(root);
    return 0;
}
