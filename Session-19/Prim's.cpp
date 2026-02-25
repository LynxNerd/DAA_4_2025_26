class Solution {
  public:
    int spanningTree(int V, vector<vector<int>>& edges) {
        vector<vector<pair<int,int>>>list(V);
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>mnn;
        vector<int>visited(V,-1);
        int E=edges.size(),ans=0,count=0,u,v,w,node,n;
        for(int i=0;i<E;i++)
        {
            u=edges[i][0];
            v=edges[i][1];
            w=edges[i][2];
            list[u].push_back({v,w});
            list[v].push_back({u,w});
        }
        mnn.push({0,0});
        while(mnn.size()!=0&&count!=V)
        {
            w=mnn.top().first;
            node=mnn.top().second;
            mnn.pop();
            if(visited[node]==0)
            {
                continue;
            }
            ans=ans+w;
            count++;
            visited[node]=0;
            n=list[node].size();
            for(int i=0;i<n;i++)
            {
                if(visited[list[node][i].first]==-1)
                {
                    w=list[node][i].second;
                    v=list[node][i].first;
                    mnn.push({w,v});
                }
            }
        }
        return ans;
    }
};