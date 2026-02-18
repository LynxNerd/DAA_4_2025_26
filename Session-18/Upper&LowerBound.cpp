#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int lowerBound(int arr[], int n, int target) {
    int st = 0;
    int end = n - 1;
    int ans = n;
    while (st <= end) {
        int mid = st + (end - st) / 2;
        if (arr[mid] >= target) {
            ans = mid;
            end = mid - 1;
        } 
        else {
            st = mid + 1; 
        }
    }
    return ans;
}
int upperBound(int arr[], int n, int target) {
    int st = 0;
    int end = n - 1;
    int ans = n;  
    while (st <= end) {
        int mid = st + (end - st) / 2;
        if (arr[mid] > target) {
            ans = mid;
            end = mid - 1; 
        } 
        else {
            st = mid + 1; 
        }
    }
    return ans;
}
int main() {
    int n;
    cout<<"Enter size of array: ";
    cin>> n;
    int arr[n];
    cout<<"Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >>arr[i];
    }
    sort(arr,arr + n);
    int target;
    cout << "Enter number to find bounds: ";
    cin >> target;
    int lb = lowerBound(arr,n,target);
    int ub = upperBound(arr,n,target);
    cout << "Lower Bound Index: " <<lb<< endl;
    cout << "Upper Bound Index: " <<ub;

    return 0;
}