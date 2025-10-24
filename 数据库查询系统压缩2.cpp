#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 从先序遍历构建BST
TreeNode* buildBSTFromPreorder(const vector<int>& preorder, int& index, int minVal, int maxVal) {
    if (index >= preorder.size()) return nullptr;
    
    int val = preorder[index];
    if (val < minVal || val > maxVal) return nullptr;
    
    TreeNode* root = new TreeNode(val);
    index++;
    
    root->left = buildBSTFromPreorder(preorder, index, minVal, val - 1);
    root->right = buildBSTFromPreorder(preorder, index, val + 1, maxVal);
    
    return root;
}

// 找到左子树的最大值（中序前驱）
int findLeftSubtreeMax(TreeNode* root) {
    if (!root || !root->left) return -1;
    
    TreeNode* curr = root->left;
    while (curr->right) {
        curr = curr->right;
    }
    return curr->val;
}

// 中序遍历找到后继节点
bool hasInorderSuccessor(TreeNode* root, TreeNode* target) {
    stack<TreeNode*> st;
    TreeNode* curr = root;
    TreeNode* prev = nullptr;
    bool found = false;
    
    while (curr || !st.empty()) {
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        
        curr = st.top();
        st.pop();
        
        if (found) return true; // 找到后继
        if (curr == target) found = true;
        
        prev = curr;
        curr = curr->right;
    }
    
    return false; // 没有后继（是最后一个节点）
}

// 压缩BST，返回新树的先序遍历结果
void compressAndTraverse(TreeNode* root, vector<pair<int, int>>& result) {
    if (!root) return;
    
    stack<TreeNode*> st;
    st.push(root);
    
    while (!st.empty()) {
        TreeNode* node = st.top();
        st.pop();
        
        // 检查是否需要删除（叶子节点且有中序后继）
        if (!node->left && !node->right && hasInorderSuccessor(root, node)) {
            continue; // 跳过该节点（删除）
        }
        
        // 确定存储的值
        int storedValue = -1; // -1 表示不存储任何值
        if (node->left) {
            storedValue = findLeftSubtreeMax(node);
        }
        
        result.push_back({node->val, storedValue});
        
        // 先右后左入栈，保证先序遍历顺序
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<int> preorder(n);
    for (int i = 0; i < n; i++) {
        cin >> preorder[i];
    }
    
    // 构建BST
    int index = 0;
    TreeNode* root = buildBSTFromPreorder(preorder, index, 0, 10000000);
    
    // 压缩并获取先序遍历结果
    vector<pair<int, int>> newPreorder;
    compressAndTraverse(root, newPreorder);
    
    // 输出结果
    for (int i = 0; i < newPreorder.size(); i++) {
        cout << newPreorder[i].first;
        if (newPreorder[i].second != -1) {
            cout << " " << newPreorder[i].second;
        } else {
            cout << " -";
        }
        if (i < newPreorder.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;
    
    return 0;
}