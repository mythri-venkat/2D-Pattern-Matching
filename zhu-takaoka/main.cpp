#include <iostream>
#include <string>
#include <vector>
using namespace std;

int d=256;
int q=49157;

vector<long long> pathash(string * text,int n,int m){
    vector<long long> texthash(n);
    for(int j=0;j<n;j++){
        texthash[j]=0;
        for(int i=0;i<m;i++){
            int ord = (int)text[i][j];
            texthash[j]=(texthash[j]*d+ord)%q;
        }
    }

    return texthash; 
}

vector<vector<long long>> texthashdash(string * text,int n,int m,int dm){
    vector<vector<long long>> texthash(n,vector<long long>(n,0));
    for(int j=0;j<n;j++){
        texthash[m-1][j]=0;
        for(int i=0;i<m;i++){
            int ord=(int)text[i][j];
            texthash[m-1][j]=(texthash[m-1][j]*d+ord)%q;
        }
    }
    for(int i=m;i<n;i++){
        for(int j=0;j<n;j++){
            int ord1=(int)text[i-m][j];
            int ord2=(int)text[i][j];
            texthash[i][j]=((texthash[i-1][j]+d*q-ord1*dm)*d+ord2)%q;
            if(texthash[i][j]<0){
                texthash[i][j]+=q;
            }
        }
    }
    return texthash;
}



void preprocess(vector<long long> pat,vector<int> &preproc, int len)
{

    preproc[0] = -1;
    int i = 1, j = 0;
    while (i < len)
    {
        if (pat[i] == pat[j])
        {
            preproc[i] = preproc[j];
            i++;
            j++;
        }
        else
        {
            preproc[i] = j;
            j = preproc[j];
            while (j >= 0 && pat[i] != pat[j])
            {
                j = preproc[j];
            }
            i++;
            j++;
        }
    }
    preproc[i] = j;
}

void searchKMP(vector<vector<long long>> &r, vector<long long> &pat, int m, int n)
{
    vector<int> preproc(m+1);
    preprocess(pat,preproc,m);
    
    vector<pair<int,int>> result;
    for (int z = 0; z < n; z++)
    {
        int i = 0, j = 0;
        while (i < n)
        {
            if(r[z][i]==pat[j]){
                i++;
                j++;
                if(j==m){
                    result.push_back(make_pair(z-m+1,i-m));
                    j=preproc[j];
                }
            }
            else{
                j=preproc[j];
                if(j<0){
                    i++;
                    j++;
                }
            }
        }
    }
    
    for(int i=0;i<result.size();i++){
        cout << result[i].first<<","<<result[i].second<<endl;
    }
}


void algorithm(string * text,string * pattern,int n,int m){
    
    int dm=1;
    for(int j=0;j<m-1;j++){
        dm=(d*dm)%q;
    }
    vector<vector<long long>> texthash = texthashdash(text,n,m,dm);
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<n;j++){
    //         cout << texthash[i][j]<<" ";
    //     }
    //     cout << endl;
    // }
    
    vector<long long> vecpathash = pathash(pattern,m,m);
    // for(int i=0;i<m;i++){
    //     cout << vecpathash[i]<<" ";
    // }
    searchKMP(texthash,vecpathash,m,n);

}


int main()
{
    int m = 5, n = 10;
    cin >> n >> m;
    // string pat[] = {"aabba", "aaabb", "ababa", "aabba", "aaabb"};
    // string text[] = {"aabbaaabba", "aaabbaaabb", "ababaababa", "aabbaaabba", "aaabbaaabb", "baaabbabab", "aababaabba", "aaabbaaabb", "baaabbaaab", "baaabbaaab"};
    
    string pat[m], text[n];

    for (int i = 0; i < n; i++)
    {
        cin >> text[i];
    }
    for (int i = 0; i < m; i++)
    {
        cin >> pat[i];
    }
     
    algorithm(text,pat,n,m);
}