class Solution {
  public:
  bool ispossible(vector<int> &arr,int mid,int k) {
      int sum =0;
      int painters = 0;
      bool ispossible = true;
      for(int i=0;i<arr.size();i++) {
          sum+= arr[i];
          if(sum>mid) {
              sum = arr[i];
              painters++;
          }
      }
      if(painters<k)
      return true;
      return false;
  }
    int minTime(vector<int>& arr, int k) {
        int n =  arr.size();
        int mx=0;
        int ans;
        for(int i=0;i<n;i++) {
            mx +=arr[i];
        }
        int mn = *max_element(arr.begin(),arr.end());
        while(mn<=mx) {
            int mid = mn + (mx-mn)/2;
            if(ispossible(arr, mid, k)) {
                ans = mid;
                mx = mid -1;
            } else {
                mn = mid+1;
            }
        }
        return ans;
    }
};