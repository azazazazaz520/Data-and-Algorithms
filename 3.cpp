#include <iostream>
#include <vector>


bool checkConnectivity(const std::vector<std::vector<int>>& graph, int N,
                       int blockedFrom = -1, int blockedTo = -1) {
    if (N == 0) return true;

    
    std::vector<char> visited(N, 0);

    
    std::vector<int> q;
    q.reserve(N);
    int qhead = 0;

    // 从 0 开始
    visited[0] = 1;
    q.push_back(0);
    int cnt = 1;

    while (qhead < (int)q.size()) {
        int cur = q[qhead++];
        for (int nei : graph[cur]) {
            if (cur == blockedFrom && nei == blockedTo) continue; // 跳过被损坏的边
            if (!visited[nei]) {
                visited[nei] = 1;
                q.push_back(nei);
                if (++cnt == N) return true; // 提前结束
            }
        }
    }

    return cnt == N;
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
    std::cout << (checkConnectivity(graph, N) ? 1 : 0) << '\n';

    // 处理每条可能被破坏的边
    for (int a = 0; a < M; a++) {
        int i, j;
        std::cin >> i >> j;   // 表示边i->j被破坏
        std::cout << (checkConnectivity(graph, N, i, j) ? 1 : 0) << '\n';
    }
    
    return 0;
}