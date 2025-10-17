#include <iostream>
#include <vector>
#include <queue>
#include <bitset>

using namespace std;

const int MAXN = 70000;

vector<vector<int>> graph;
vector<pair<int, int>> queries;

// BFS
bool bfs(int N, int forbidden_u = -1, int forbidden_v = -1) {
    vector<bool> visited(N, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    int count = 1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : graph[u]) {
            if (u == forbidden_u && v == forbidden_v) continue;
            if (!visited[v]) {
                visited[v] = true;
                count++;
                q.push(v);
            }
        }
    }
    
    return count == N;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    graph.resize(N);
    
    // 读取图
    for (int i = 0; i < N; i++) {
        int n;
        cin >> n;
        graph[i].resize(n);
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }
    
    // 读取查询
    queries.resize(M);
    for (int i = 0; i < M; i++) {
        cin >> queries[i].first >> queries[i].second;
    }
    
    // 检查原始连通性
    bool base = bfs(N);
    cout << (base ? 1 : 0) << '\n';
    
    // 如果不连通，所有查询都输出0
    if (!base) {
        for (int i = 0; i < M; i++) {
            cout << 0 << '\n';
        }
        return 0;
    }
    
    // 对于每个查询，直接检查
    for (int i = 0; i < M; i++) {
        int u = queries[i].first;
        int v = queries[i].second;
        
        // 快速检查：如果v有多个入边，删除一条可能不影响
        bool hasMultiplePaths = false;
        for (int j = 0; j < N && !hasMultiplePaths; j++) {
            if (j == u) continue;
            for (int neighbor : graph[j]) {
                if (neighbor == v) {
                    hasMultiplePaths = true;
                    break;
                }
            }
        }
        
        if (hasMultiplePaths) {
            // 如果有多个路径，还需要进一步检查
            cout << (bfs(N, u, v) ? 1 : 0) << '\n';
        } else {
            // 如果只有一条路径，删除后肯定不连通
            cout << 0 << '\n';
        }
    }
    
    return 0;
}