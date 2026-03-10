class Solution {
  public:
    vector<int> maxOfSubarrays(vector<int>& arr, int k) {
        // code here
        priority_queue<pair<int,int>> pq;
        vector<int> ans;
        int n = arr.size();
        for(int i=0;i<n;i++) {
            pq.push({arr[i], i});
            while(pq.top().second<=i-k) {
                pq.pop();
            }
            if(i>=k-1) {
                ans.push_back(pq.top().first);
            }
        }
        return ans;
    }
};