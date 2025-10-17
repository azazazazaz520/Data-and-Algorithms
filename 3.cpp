#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

// 使用BFS检查从节点0是否可以到达所有其他节点
bool checkConnectivity(const std::vector<std::vector<int>>& graph) {
    int N = graph.size();   // 节点数量
    if (N == 0) return true;  // 处理空图的情况
    std::vector<bool> visited(N, false);
    std::queue<int> q;
    
    // 从0号节点开始BFS
    q.push(0);
    visited[0] = true;
    
    while (!q.empty()) {
        int current = q.front();  
        q.pop();
        
        // 检查所有可达的邻居节点
        for (int neighbor : graph[current]) {
            if (!visited[neighbor]) {  // 如果邻居节点未被访问过
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    
    // 检查是否所有节点都被访问到
    for (bool v : visited) {
        if (!v) return false;
    }
    return true;
}

// 测试删除某条边后的连通性
bool testAfterDamage(std::vector<std::vector<int>> graph, int from, int to) {
    // 删除from到to的边
    for (auto it = graph[from].begin(); it != graph[from].end(); ++it) {
        if (*it == to) {
            graph[from].erase(it);
            break;
        }
    }
    
    return checkConnectivity(graph);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int N, M;
    std::cin >> N >> M;       // N个节点，可能被破坏的边数M
    
    std::vector<std::vector<int>> graph(N);  // 图节点存储
    
    // 读入每个节点能传递消息的节点
    for (int i = 0; i < N; i++) {
        int n;
        std::cin >> n;        // 表示该节点能传递消息的节点个数
        for (int j = 0; j < n; j++) {
            int to;
            std::cin >> to;   // 能传递消息的节点编号
            graph[i].push_back(to);
        }
    }
    
    // 首先输出原始图的连通性
    std::cout << (checkConnectivity(graph) ? 1 : 0) << '\n';
    
    // 处理每条可能被破坏的边
    for (int a = 0; a < M; a++) {
        int i, j;
        std::cin >> i >> j;   // 表示边i-j被破坏
        std::cout << (testAfterDamage(graph, i, j) ? 1 : 0) << '\n';
    }
    
    return 0;
}