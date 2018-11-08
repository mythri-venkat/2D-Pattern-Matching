#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

const int Alph_size = 26;

struct trie
{
    struct trie *children[Alph_size];
    bool end;
    struct trie *failnode;
    vector<string> output;
    int op = 0;
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


unordered_map<string, int> mp;
vector<pair<int,int>> result;
vector<int> prepat;

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
    pCrawl->op = mp[word];
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


void checkMatch(int row, int column, int index, string *text, string *pattern, struct trie *root, int m, int n)
{
    vector<int> F(2 * m - 1, 0);
    F[m-1] = index;
    for (int i = 0; i < m; i++)
    {
        if (mp[pattern[i]] != index)
        {
            continue;
        }
        int j = m - i + 1;
        int r;
        for (r = row - i ; r < row - i + m && r < n; r++)
        {
            if (F[j] == mp[pattern[r - row + i]])
            {
                continue;
            }
            struct trie *q = root;
            for (int c = column; c < column + m ; c++)
            {
                int idx = text[r][c]-'a';
                if (q->children[idx])
                {
                    q = q->children[idx];
                }
            }
            F[j] = q->op;
            if (F[j] != mp[pattern[r - row + i]])
            {
                break;
            }
        }
        //cout << r << ","<<row-i+m+1<<endl;
        if (r == row - i + m)
        {
            result.push_back(make_pair(r-m,column));
        }
    }
}


void match(struct trie *root, string *str, string *pattern, int n, int m)
{
    for (int k = m-1; k < n; k += m)
    {
        int i = 0;
        struct trie *cur = root;
        while (i < n)
        {
            int idx = str[k][i] - 'a';
            if (cur->children[idx] != NULL)
            {
                cur = cur->children[idx];

                if (cur->op != 0)
                {
                    checkMatch(k, i - m + 1, cur->op, str, pattern, root, m, n);
                    
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
    for(int i=0;i<result.size();i++){
        cout << result[i].first<<","<<result[i].second<<endl;
    }
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

int main()
{
    int m = 5, n = 10;
   // cin >> n >> m;
   
    string pat[] = {"aabba", "aaabb", "ababa", "aabba", "aaabb"};
    string text[] = {"aabbaaabba", "aaabbaaabb", "ababaababa", "aabbaaabba", "aaabbaaabb", "baaabbabab", "aababaabba", "aaabbaaabb", "baaabbaaab", "baaabbaaab"};
    struct trie *root = create_node();
    // string pat[m], text[n];
    // for (int i = 0; i < n; i++)
    // {
    //     cin >> text[i];
    // }
    // for (int i = 0; i < m; i++)
    // {
    //     cin >> pat[i];
    // }

    int x = 1;
    for (int i = 0; i < m; i++)
    {
        
        if (mp.find(pat[i]) == mp.end())
        {
            mp[pat[i]] = x++;
        }
        prepat.push_back(mp[pat[i]]);
    }
    for (auto it = mp.begin(); it != mp.end(); it++)
        build(root, it->first);
    build_fail(root);
    match(root, text, pat, n, m);
}