class Solution {
  public:
  bool possible(vector<int> &stalls, int mid,int k) {
      int n = stalls.size();
      int prev = 0;
      int cow =1;
      for(int i=1;i<n;i++) {
          if(stalls[i]-stalls[prev]>=mid) {
              cow++;
              prev = i;
          }
      }
      if(cow>=k) {
          return true;
      }
      return false;
  }
    int aggressiveCows(vector<int> &stalls, int k) {
        int n = stalls.size();
        int mx = *max_element(stalls.begin(),stalls.end());
        int mn = *min_element(stalls.begin(),stalls.end());
        int st =1;
        int end = mx-mn;
        sort(stalls.begin(),stalls.end());
        int ans;
        while(st<=end) {
            int mid = st + (end-st)/2;
            if(possible(stalls,mid,k)) {
                ans = mid;
                st = mid+1;
            } else {
                end = mid-1;
            }
        }
        return ans;
    }
};