#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

const int Alph_size = 26;

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
        int index = word[i] - 'a';
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
                if (fail->children[i] != NULL)
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
                        child->failnode = temp;
                        isroot = true;
                    }
                    else
                    {
                        child->failnode = temp->failnode;
                        isroot = false;
                    }
                    for (int j = 0; j < temp->output.size(); j++)
                    {
                        child->output.push_back(temp->output[j]);
                    }
                }
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
            int idx = str[z][i] - 'a';
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

int main()
{
    int m = 5, n = 10;
    cin >> n >> m;
    //string pat[] = {"aabba", "aaabb", "ababa", "aabba", "aaabb"};
    //string text[] = {"aabbaaabba", "aaabbaaabb", "ababaababa", "aabbaaabba", "aaabbaaabb", "baaabbabab", "aababaabba", "aaabbaaabb", "baaabbaaab", "baaabbaaab"};
    struct trie *root = create_node();
    string pat[m], text[n];
    vector<vector<int>> r(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        cin >> text[i];
    }
    for (int i = 0; i < m; i++)
    {
        cin >> pat[i];
    }

    int x = 1;
    int prepat[m];
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