#include <bits/stdc++.h>
using namespace std;

struct Flight {
    int id;
    int u, v;
    int depart, arrive;
};

struct State {
    long long total;          // 总用时
    vector<int> seq;          // 航班编号序列
    int last_city;            // 当前城市
    int last_arrive;          // 最后到达时间
    int first_depart;         // 首段起飞时间
};

// 优先队列比较器：总用时升序，若相等则按航班编号序列字典序
struct Cmp {
    bool operator()(const State &a, const State &b) const {
        if (a.total != b.total) return a.total > b.total;
        const auto &A = a.seq, &B = b.seq;
        size_t n = min(A.size(), B.size());
        for (size_t i = 0; i < n; ++i)
            if (A[i] != B[i]) return A[i] > B[i];
        return A.size() > B.size();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N, P, Q, K;
    cin >> M >> N >> P >> Q >> K;
    vector<Flight> flights(N);
    for (int i = 0; i < N; ++i) {
        cin >> flights[i].u >> flights[i].v >> flights[i].depart >> flights[i].arrive;
        flights[i].id = i;
    }

    // 构建邻接表（每个城市的出发航班，按出发时间排序）
    vector<vector<int>> out(M);
    for (int i = 0; i < N; ++i) out[flights[i].u].push_back(i);
    for (int i = 0; i < M; ++i)
        sort(out[i].begin(), out[i].end(), [&](int a, int b) {
            if (flights[a].depart != flights[b].depart)
                return flights[a].depart < flights[b].depart;
            return a < b;
        });

    // 优先队列初始化
    priority_queue<State, vector<State>, Cmp> pq;

    // 初始状态：从出发城市 P 起飞的所有航班
    for (int fid : out[P]) {
        const auto &f = flights[fid];
        State s;
        s.seq = {fid};
        s.last_city = f.v;
        s.last_arrive = f.arrive;
        s.first_depart = f.depart;
        s.total = f.arrive - f.depart;
        pq.push(s);
    }

    vector<State> ans;
    ans.reserve(K);

    // 搜索最短 K 条路径
    while (!pq.empty() && (int)ans.size() < K) {
        State cur = pq.top();
        pq.pop();

        if (cur.last_city == Q) {
            ans.push_back(cur);
            continue;
        }

        // 扩展：找出所有可衔接航班（出发时间 >= 当前到达时间）
        auto &cand = out[cur.last_city];
        auto it = lower_bound(cand.begin(), cand.end(), cur.last_arrive,
                              [&](int fid, int t) { return flights[fid].depart < t; });

        for (; it != cand.end(); ++it) {
            const auto &nf = flights[*it];
            State nxt;
            nxt.seq = cur.seq;
            nxt.seq.push_back(nf.id);
            nxt.last_city = nf.v;
            nxt.last_arrive = nf.arrive;
            nxt.first_depart = cur.first_depart;
            nxt.total = nf.arrive - cur.first_depart;
            pq.push(move(nxt));
        }
    }

    // 输出结果
    for (int i = 0; i < K; ++i) {
        if (i < (int)ans.size()) {
            const auto &s = ans[i];
            cout << s.total << " " << P;
            for (size_t j = 0; j < s.seq.size(); ++j)
                cout << "-" << s.seq[j] << "-" << flights[s.seq[j]].v;
            cout << "\n";
        } else {
            cout << "-1\n";
        }
    }

    return 0;
}
