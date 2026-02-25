class Solution {
  public:
    int findUltimateParent(int node,vector<int>& parent)
    {
        if(node==parent[node])
        {
            return node;
        }
        else
        {
            return parent[node]=findUltimateParent(parent[node],parent);
        }
    }
    int spanningTree(int V, vector<vector<int>>& edges){
    
        priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,greater<pair<int,pair<int,int>>>>mnn;
        int u,v,w,E=edges.size(),ans=0,Pu,Pv,count=0,a,b;
        vector<int>parent(V);
        vector<int>size(V,1);
        for(int i=0;i<E;i++)
        {
            u=edges[i][0];
            v=edges[i][1];
            w=edges[i][2];
            mnn.push({w,{u,v}});
        }
        for(int i=0;i<V;i++)
        {
            parent[i]=i;
        }
        while(mnn.size()!=0&&count!=V-1)
        {
            u=mnn.top().second.first;
            v=mnn.top().second.second;
            w=mnn.top().first;
            mnn.pop();
            Pu=findUltimateParent(u,parent);
            Pv=findUltimateParent(v,parent);
            if(Pu==Pv)
            {
                continue;
            }
            a=size[Pu];
            b=size[Pv];
            if(a>b)
            {
                parent[Pv]=Pu;
                size[Pu]=size[Pu]+b;
                size[Pv]=0;
            }
            else
            {
                parent[Pu]=Pv;
                size[Pv]=size[Pv]+a;
                size[Pu]=0;
            }
            ans=ans+w;
            count++;
            
        }
        return ans;
    }
};