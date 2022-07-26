#include <iostream>
#include <algorithm>
#include <cstring>
#include  <unordered_map>
#include <vector>

using namespace std;
using Coordinate = pair<int, int>;
using Case = pair<string,Coordinate>;
using List = vector<Case>;

//https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
void computeLPSArray(string pat, int M, int* lps);
 
// Prints occurrences of txt[] in pat[]
void KMPSearch(string pat, string txt, List &list, Case &c)
{
    int M = pat.size();
    int N = txt.size();
 
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int *lps = new int[M];
 
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);
 
    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    while ((N - i) >= (M - j)) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
        }
 
        if (j == M) {
            printf("Found pattern at index %d ", i - j);
            c.second.first = 9;
            c.second.second = i-j;
            list.push_back(c);
            j = lps[j - 1];
        }
 
        // mismatch after j matches
        else if (i < N && pat[j] != txt[i]) {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
}
 
// Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(string pat, int M, int* lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
 
    lps[0] = 0; // lps[0] is always 0
 
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0) {
                len = lps[len - 1];
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
 
// Driver program to test above function
int main()
{
    string txt = "ABABCABABCDABABCABAB";
    string pat = "ABABCABAB";
    List list;
    Case c;
    c.first = "LR";
    KMPSearch(pat, txt,list,c);
    for(auto& x : list){
        cout<<x.first<<" "<<x.second.first<<" "<<x.second.second<<endl;
    }
    return 0;
}