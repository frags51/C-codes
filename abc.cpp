#include<bits/stdc++.h>
#include <unordered_map>
using namespace std;

int xorPairCount(int arr[], int n, int x)
{
    int result = 0; // Initialize result

    unordered_set<int> s;

    for (int i=0; i<n ; i++)
    {

        if (s.find(x^arr[i]) != s.end())
            result++;

        s.insert(arr[i]);
    }

    return result;
}

int main()
{

    int n ,x;
    cin >> n;
    cin >> x;
    int arr[n];
    for(int i=0; i<n; i++) cin >> arr[i];
    cout << xorPairCount(arr, n, x) << endl;
    return 0;
}
