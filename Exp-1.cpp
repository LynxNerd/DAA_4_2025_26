// Recurrence formula: 3* n/2 + nlogn
// Time Complexity: O(n ^ log3 base 2)
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;




void complexRec(int n,int &count) {
    


   if (n <= 2) {
       count ++; // changed here 
       return;
   }


   int p = n;
   while (p > 0) {
       vector<int> temp(n);
       for (int i = 0; i < n; i++) {
           temp[i] = i ^ p;
           count++; // changed here
       }
       p >>= 1;
   }


   vector<int> small(n);
   for (int i = 0; i < n; i++) {
       small[i] = i * i;
       count++; // changed here 
   }


   if (n % 3 == 0) {
       reverse(small.begin(), small.end());
       count++; //changed here 
   } else {
       reverse(small.begin(), small.end());
       count++; // changed here 
   }
    

  complexRec(n / 2,count); // changed here 
   complexRec(n / 2,count); // changed here
   complexRec(n / 2,count); // changed here 
   
   
}
int main() {
    int count =0;
    int n;
    cout<<"Enter n: ";
    cin>>n;
    complexRec(n,count);
    cout<<"No. of operations: "<<count<<endl;
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end-start);
    cout<<duration.count();
    
    
   
}
