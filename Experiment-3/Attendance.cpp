#include <bits/stdc++.h>
using namespace std;
int stableWindow(int n, const vector<char>& arr) {
    unordered_map<int, int> mp;
    int sum = 0, maxLen = 0;
    mp[0] = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] == 'P')
            sum++;
        else if (arr[i] == 'A')
            sum--;
        if (mp.find(sum) != mp.end()) {
            maxLen = max(maxLen, i - mp[sum]);
        } else {
            mp[sum] = i;
        }
    }
    return maxLen;
}

int main() {
    int n;
    cin >> n;
    vector<char> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    cout << stableWindow(n, arr);
    return 0;
}