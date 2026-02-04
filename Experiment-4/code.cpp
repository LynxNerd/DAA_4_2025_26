#include <bits/stdc++.h>
using namespace std;

int main() {
    int k;
    int n;
    cin >> k >> n;
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int i = 0; i < n; i++) {
        int score;
        cin >> score;
        minHeap.push(score);
        if (minHeap.size() > k) {
            minHeap.pop();
        }
        if (minHeap.size() < k) {
            cout << -1 << endl;
        } else {
            cout << minHeap.top() << endl;
        }
    }
    return 0;
}