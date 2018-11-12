#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

const int Alph_size = 10;
const char Alph_start = '0';

struct trie
{
    struct trie *children[Alph_size];
    bool end;
    struct trie *failnode;
    vector<string> output;
};

struct trie *create_node(void)
{
    struct trie *newnode = new trie;

    newnode->end = false;
    newnode->failnode = NULL;
    for (int i = 0; i < Alph_size; i++)
        newnode->children[i] = NULL;

    return newnode;
}

void build(struct trie *root, string word)
{
    struct trie *pCrawl = root;
    for (int i = 0; i < word.length(); i++)
    {
        int index = word[i] - Alph_start;
        if (!pCrawl->children[index])
        {
            pCrawl->children[index] = create_node();
        }

        pCrawl = pCrawl->children[index];
    }
    pCrawl->output.push_back(word);
    // mark last node as leaf
    pCrawl->end = true;
}

void build_fail(struct trie *root)
{
    queue<struct trie *> q;
    struct trie *fail = root;
    for (int i = 0; i < Alph_size; i++)
    {
        if (root->children[i] != NULL)
        {
            root->children[i]->failnode = fail;
            
            q.push(root->children[i]);
        }
    }
    struct trie *cur;
    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        fail = cur->failnode;
        for (int i = 0; i < Alph_size; i++)
        {
            struct trie *child = cur->children[i];
            bool isroot = true;
            if (child != NULL)
            {
                if (fail->children !=NULL && fail->children[i] != NULL)
                {
                    child->failnode = fail->children[i];

                    for (int j = 0; j < fail->children[i]->output.size(); j++)
                    {
                        child->output.push_back(fail->children[i]->output[j]);
                    }
                    isroot = false;
                }
                else
                {
                    struct trie *temp = fail;
                    while (temp->children[i] == NULL && temp->failnode != NULL)
                    {
                        temp = temp->failnode;
                    }

                    if (temp->children[i] == NULL && temp->failnode == NULL)
                    {
                        child->failnode = root;
                        isroot = true;
                    }
                    else if(temp->children[i] != NULL)
                    {
                        child->failnode = temp->children[i];
                        isroot = false;
                    }
                    else{
                        child->failnode = temp->failnode;
                    }
                    for (int j = 0; j < temp->output.size(); j++)
                    {
                        child->output.push_back(temp->output[j]);
                    }
                }
                //cout << (child->failnode == NULL)<<" ";
                q.push(child);
            }
        }
    }
}

unordered_map<string, int> mp;

void match(struct trie *root, string *str, int n, int m, vector<vector<int>> &r)
{
    for (int z = 0; z < n; z++)
    {
        int i = 0;
        struct trie *cur = root;
        while (i < n)
        {
            int idx = str[z][i] - Alph_start;
            if (cur->children[idx] != NULL)
            {
                cur = cur->children[idx];

                for (int j = 0; j < cur->output.size(); j++)
                {
                    if (cur->output[j].length() == m)
                    {
                        int x = i - cur->output[j].length() + 1;
                        r[z][i] = mp[cur->output[j]];
                    }
                }
                i++;
            }
            else
            {
                if (cur->failnode != NULL)
                    cur = cur->failnode;
                else
                    i++;
            }
        }
    }
    /* for (int z = 0; z < n; z++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << r[z][j] << " ";
        }
        cout << endl;
    } */
}

void preprocess(int *pat, int *preproc, int len)
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

void searchKMP(vector<vector<int>> &r, int *pat, int m, int n)
{
    int preproc[m + 1];
    preprocess(pat,preproc,m);
    vector<pair<int,int>> result;
    for (int z = 0; z < n; z++)
    {
        int i = 0, j = 0;
        while (i < n)
        {
            if(r[i][z]==pat[j]){
                i++;
                j++;
                if(j==m){
                    result.push_back(make_pair(i-m,z-m+1));
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

void baker_bird_algo(string * text,string  * pat,int n,int m){
    int x = 1;
    int prepat[m];
    struct trie *root = create_node();
    
    vector<vector<int>> r(n, vector<int>(n, 0));
    for (int i = 0; i < m; i++)
    {
        if (mp.find(pat[i]) == mp.end())
        {
            mp[pat[i]] = x++;
        }
        prepat[i] = mp[pat[i]];
    }
    for (auto it = mp.begin(); it != mp.end(); it++)
        build(root, it->first);
    build_fail(root);
    match(root, text, n, m, r);
    searchKMP(r,prepat,m,n);
}

int main()
{
    int m = 5, n = 10, t = 0;
    cin.sync_with_stdio(false);
    cin >> t;
    // string pat[] = {"aabba", "aaabb", "ababa", "aabba", "aaabb"};
    // string text[] = {"aabbaaabba", "aaabbaaabb", "ababaababa", "aabbaaabba", "aaabbaaabb", "baaabbabab", "aababaabba", "aaabbaaabb", "baaabbaaab", "baaabbaaab"};
    string str="";
    for (int k = 0; k < t; k++)
    {
        
        cin >> n >> m;
        string pat[m], text[n];
        for (int i = 0; i < n; i++)
        {
            cin >> text[i];
        }
        for (int i = 0; i < m; i++)
        {
            cin >> pat[i];
        }
        cout << "testcase:"<<k+1<<endl;
        clock_t start=clock();
        
        baker_bird_algo(text, pat, n, m);
        
        double duration = 1000*(clock()-start)/(double)CLOCKS_PER_SEC;
        cout << endl;

        str+=to_string(duration)+"\n";
    }

    int fd = open("timing.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    write(fd,str.c_str(),str.length());

}