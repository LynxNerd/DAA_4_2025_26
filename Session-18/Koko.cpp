class Solution {
public:
    int minEatingSpeed(vector<int>& nums, int h) {
            long long n = nums.size(),sum;
            int mx= *max_element(nums.begin(),nums.end());
            int st =1;
            int end = mx;
            int ans;
            while(st<=end) {
                int mid = st + (end-st)/2;
                sum =0;
                for(int i=0;i<n;i++) {
                    if(nums[i]%mid != 0) {
                        sum += nums[i]/mid +1;
                    } else {
                        sum += nums[i]/mid;
                    }
                }
                if(sum <= h) {
                    ans = mid;
                    end = mid-1;
                } else {
                    st = mid+1;
                }
            }
            return ans;
    }
};