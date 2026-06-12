class Solution {
public:
    vector<int> assignEdgeWeights(vector<vector<int>>& edges, vector<vector<int>>& queries) {
        int n = edges.size() + 1;
        const int MOD = 1e9 + 7;
        int LOG = 1;
        while ((1 << LOG) < n) LOG++;
        LOG++;
        
        vector<vector<int>> adj(n + 1);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        vector<int> depth(n + 1, 0);
        vector<vector<int>> up(LOG, vector<int>(n + 1, 1));
        
        vector<bool> visited(n + 1, false);
        vector<int> queue_;
        queue_.push_back(1);
        visited[1] = true;
        up[0][1] = 1;
        for (int idx = 0; idx < (int)queue_.size(); idx++) {
            int u = queue_[idx];
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    depth[v] = depth[u] + 1;
                    up[0][v] = u;
                    queue_.push_back(v);
                }
            }
        }
        
        for (int k = 1; k < LOG; k++) {
            for (int v = 1; v <= n; v++) {
                up[k][v] = up[k-1][up[k-1][v]];
            }
        }
        
        auto lca = [&](int u, int v) -> int {
            if (depth[u] < depth[v]) swap(u, v);
            int diff = depth[u] - depth[v];
            for (int k = 0; k < LOG; k++) {
                if (diff & (1 << k)) {
                    u = up[k][u];
                }
            }
            if (u == v) return u;
            for (int k = LOG - 1; k >= 0; k--) {
                if (up[k][u] != up[k][v]) {
                    u = up[k][u];
                    v = up[k][v];
                }
            }
            return up[0][u];
        };
        
        vector<long long> pow2(n + 1);
        pow2[0] = 1;
        for (int i = 1; i <= n; i++) {
            pow2[i] = (pow2[i-1] * 2) % MOD;
        }
        
        vector<int> ans;
        ans.reserve(queries.size());
        for (auto& q : queries) {
            int u = q[0], v = q[1];
            if (u == v) {
                ans.push_back(0);
                continue;
            }
            int l = lca(u, v);
            int dist = depth[u] + depth[v] - 2 * depth[l];
            ans.push_back((int)pow2[dist - 1]); // dist >= 1 here since u != v
        }
        
        return ans;
    }
};
