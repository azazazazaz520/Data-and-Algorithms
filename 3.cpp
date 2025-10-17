#include <iostream>
#include <vector>
int main() {
    std::ios::sync_with_stdio(false);
    int N, M;
    std::vector<std::vector<int>> vec;      //图节点存储
    std::cin >> N >> M;       //N个节点，可能被破坏的边数M
    vec.resize(N);   //
    for(int i = 0;i < N-1;i++){
        int n = 0;   //该节点能传递消息的节点个数
        std::cin >> n;        //表示该节点能传递消息的节点个数
        for(int j = 0;j < n;j++){
            int to = 0;//能传递消息的节点编号
            std::cin >> to;
            vec[i].push_back(to);
        }
    }
    for(int i = 0;i < M;i++){
        
    }
}