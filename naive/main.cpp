#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void naive_algo(string *text, string *pat, int n, int m)
{
    for (int i = 0; i < n-m+1; i++)
    {
        for (int j = 0; j < n-m+1; j++)
        {
            bool found = true;
            for (int k = 0; k < m; k++)
            {
                for (int l = 0; l < m; l++)
                {
                    if (text[i + k][j + l] != pat[k][l])
                    {
                        found = false;
                        
                    }
                }
               
            }
            if(found){
                cout << i <<","<<j<<endl;
            }
        }
    }
}

int main()
{
    int m = 5, n = 10, t = 0;
    cin.sync_with_stdio(false);
    cin >> t;
    // string pat[] = {"aabba", "aaabb", "ababa", "aabba", "aaabb"};
    // string text[] = {"aabbaaabba", "aaabbaaabb", "ababaababa", "aabbaaabba", "aaabbaaabb", "baaabbabab", "aababaabba", "aaabbaaabb", "baaabbaaab", "baaabbaaab"};
    string str = "";
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
        cout << "testcase:" << k + 1 << endl;
        clock_t start = clock();

        naive_algo(text, pat, n, m);

        double duration = 1000 * (clock() - start) / (double)CLOCKS_PER_SEC;
        cout << endl;

        str += to_string(duration) + "\n";
    }

    int fd = open("timing.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, str.c_str(), str.length());
}