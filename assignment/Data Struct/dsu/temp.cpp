#include <iostream>
using namespace std;
struct node{
  int x;
  int y;
  int distance;
};


void merge(node arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    node L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2){
        if (L[i].distance <= R[j].distance){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(node arr[], int l, int r)
{
    if (l < r){
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}
     
int find(int x,int *par){
  if(x!=par[x]) par[x]=find(par[x],par);
  return par[x];
}
void unione(int x,int y,int *par,int *rank){
  int fpar1=find(x,par);
  int fpar2=find(y,par);
  par[x]=fpar1;
  par[y]=fpar2;
  if(rank[fpar1]>rank[fpar2]) par[fpar2]=fpar1;
  else if(rank[fpar1]<rank[fpar2]) par[fpar1]=fpar2;
  else {
    par[fpar2]=fpar1;
    rank[fpar1]++;
  }
}

void Kruskals(node *sorted,int n,int m){
  mergeSort(sorted,0,m-1);
  int par[n+1];
  int rank[n+1];
  for(int i=1;i<=n;i++){
    par[i]=i;
    rank[i]=0;
  }
  int count=1;
  for(int i=0;count<n;i++){
    int x=sorted[i].x;
    int y=sorted[i].y;
    if(find(sorted[i].x,par)==find(sorted[i].y,par))continue;
    unione(sorted[i].x,sorted[i].y,par,rank);
    cout<<x<<" "<<y<<" "<<sorted[i].distance<<"\n";
    count++;
  }
}

int main(){
  int n,m;
  label :
  cin>>n;
  if(cin.fail()){
    cout<<"Integers dumbass ,integers\n";
    cin.clear();
    std::cin.ignore(256,'\n');
    goto label;
  }
  cin>>m;
  if(cin.fail()){
     cout<<"Integers dumbass ,integers\n";
     cin.clear();
     std::cin.ignore(256,'\n');
     goto label;
  }
  if(m<n-1){
    cout<<"Haha , you ass ,gettin a pay-cut for each edge you enter ? Increase the edges ass\n";
    goto label;
  }
  node* listH[n+1];
  node* listT[n+1];
  for(int i=1;i<=n;i++){
    listH[i]=NULL;
    listT[i]=NULL;
  }
  node toSort[m];
  for(int i=1;i<=m;i++){
    int x,y,weight;
    cin>>x>>y>>weight;

    if(x>n || y>n) {
        cout<<"Vertices out of range !\n";
        goto label;
    }
    if(x==y) {
        cout<<"Sorry , no self loops assignable\n";
        goto label;
    }
    if(cin.fail()){
      cout<<"Integers dumbass ,integers\n";
      i--;
      cin.clear();
      std::cin.ignore(256,'\n');
      goto label;
    }
    toSort[i-1].x=x;
    toSort[i-1].y=y;
    toSort[i-1].distance=weight;
  }
  Kruskals(toSort,n,m);
}

  
