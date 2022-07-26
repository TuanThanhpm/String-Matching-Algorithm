#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;

using Coordinate = pair<int, int>;
using Case = pair<string, Coordinate>;
using List = vector<Case>;

// d is the number of characters in the input alphabet
#define d 256

void readFile(string filename, char list[][100], int &row, int &col)
{
    fstream fileIn;
    fileIn.open(filename, ios::in);
    if (!fileIn)
    {
        cout << "Can't open file " << filename << endl;
        return;
    }

    string temp;
    // xac dinh kich thuoc mang
    fileIn >> row;
    fileIn >> col;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            char c;
            fileIn >> c;
            list[i][j] = c;
        }
    }
    fileIn.close();
}

vector<string> createHorizontalMatrix(char list[][100], int row, int col)
{
    vector<string> LR;
    for (int i = 0; i < row; i++)
    {
        string str = "";
        for (int j = 0; j < col; j++)
        {
            str += list[i][j];
        }
        LR.push_back(str);
    }
    return LR;
}

vector<string> createVerticalMatrix(char list[][100], int row, int col)
{
    vector<string> TD;
    for (int i = 0; i < col; i++)
    {
        string str = "";
        for (int j = 0; j < row; j++)
        {
            str += list[j][i];
        }
        TD.push_back(str);
    }
    return TD;
}

void bruteForce(string pat, string txt, List &list, Case &cs)
{
    int N = txt.size();
    int M = pat.size();
    for (int j = 0; j <= N - M; j++)
    {
        int k;
        /* For current index i, check for pattern match */
        for (k = 0; k < M; k++)
            if (txt[j + k] != pat[k])
                break;
        if (k == M)
        { // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
            cs.second.second = j;
            list.push_back(cs);
        }
    }
}

// https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/?ref=lbp
void rabinKarp(string pat, string txt, int q, List &list, Case &cs)
{
    int M = pat.size();
    int N = txt.size();
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;

    // The value of h would be "pow(d, M-1)%q"
    for (i = 0; i < M - 1; i++)
        h = (h * d) % q;

    // Calculate the hash value of pattern and first
    // window of text
    for (i = 0; i < M; i++)
    {
        p = (d * p + pat[i]) % q;
        t = (d * t + txt[i]) % q;
    }

    // Slide the pattern over text one by one
    for (i = 0; i <= N - M; i++)
    {

        // Check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters one by one
        if (p == t)
        {
            /* Check for characters one by one */
            for (j = 0; j < M; j++)
                if (txt[i + j] != pat[j])
                    break;
            // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1]
            if (j == M)
            {
                cs.second.second = i;
                list.push_back(cs);
            }
        }
        // Calculate hash value for next window of text: Remove
        // leading digit, add trailing digit
        if (i < N - M)
        {
            t = (d * (t - txt[i] * h) + txt[i + M]) % q;
            // We might get negative value of t, converting it
            // to positive
            if (t < 0)
                t = (t + q);
        }
    }
}

// https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
void computeLPSArray(string pat, int M, int *lps);

// Prints occurrences of txt[] in pat[]
void KMP(string pat, string txt, List &list, Case &cs)
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
    while ((N - i) >= (M - j))
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }
        if (j == M)
        {
            cs.second.second = i - j;
            list.push_back(cs);
            j = lps[j - 1];
        }
        // mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
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
void computeLPSArray(string pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int len = 0;

    lps[0] = 0; // lps[0] is always 0

    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0)
            {
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

// Brute - force
void BFsearch(vector<string> LR, vector<string> TD, vector<string> word)
{
    int countWord{};
    for (int i = 0; i < word.size(); i++)
    {
        string pat = word[i];
        List list;
        Case cs;
        // Access horizontal matrix
        for (int i = 0; i < LR.size(); i++)
        {
            cs.first = "LR";
            cs.second.first = i;
            cs.second.second = 0;
            bruteForce(pat, LR[i], list, cs);
        }
        // Access vertical matrix
        for (int i = 0; i < TD.size(); i++)
        {
            cs.first = "TD";
            cs.second.first = i;
            cs.second.second = 0;
            bruteForce(pat, TD[i], list, cs);
        }

        if (list.size() == 0)
        {
            cout << pat << " (0,0) NF" << endl;
        }
        else
        {
            countWord++;
            for (int i = 0; i < list.size(); i++)
            {
                if (list[i].first == "LR")
                    cout << pat << " (" << list[i].second.first << "," << list[i].second.second << ") " << list[i].first << endl;
                else if (list[i].first == "TD")
                    cout << pat << " (" << list[i].second.second << "," << list[i].second.first << ") " << list[i].first << endl;
            }
        }
    }
    cout << "The number of words: " << countWord << endl;
}

// rabin - karp
void RKsearch(vector<string> LR, vector<string> TD, vector<string> word)
{
    int countWord{};
    int q = INT_MAX;
    for (int i = 0; i < word.size(); i++)
    {
        string pat = word[i];
        List list;
        Case cs;
        // Access horizontal matrix
        for (int i = 0; i < LR.size(); i++)
        {
            cs.first = "LR";
            cs.second.first = i;
            cs.second.second = 0;
            rabinKarp(pat, LR[i], q, list, cs);
        }
        // Access vertical matrix
        for (int i = 0; i < TD.size(); i++)
        {
            cs.first = "TD";
            cs.second.first = i;
            cs.second.second = 0;
            rabinKarp(pat, TD[i], q, list, cs);
        }

        if (list.size() == 0)
        {
            cout << pat << " (0,0) NF" << endl;
        }
        else
        {
            countWord++;
            for (int i = 0; i < list.size(); i++)
            {
                if (list[i].first == "LR")
                    cout << pat << " (" << list[i].second.first << "," << list[i].second.second << ") " << list[i].first << endl;
                else if (list[i].first == "TD")
                    cout << pat << " (" << list[i].second.second << "," << list[i].second.first << ") " << list[i].first << endl;
            }
        }
    }
    cout << "The number of words: " << countWord << endl;
}

void KMPsearch(vector<string> LR, vector<string> TD, vector<string> word)
{
    int countWord{};
    for (int i = 0; i < word.size(); i++)
    {
        string pat = word[i];
        List list;
        Case cs;
        // Access horizontal matrix
        for (int i = 0; i < LR.size(); i++)
        {
            cs.first = "LR";
            cs.second.first = i;
            cs.second.second = 0;
            KMP(pat, LR[i], list, cs);
        }
        // Access vertical matrix
        for (int i = 0; i < TD.size(); i++)
        {
            cs.first = "TD";
            cs.second.first = i;
            cs.second.second = 0;
            KMP(pat, TD[i], list, cs);
        }

        if (list.size() == 0)
        {
            cout << pat << " (0,0) NF" << endl;
        }
        else
        {
            countWord++;
            for (int i = 0; i < list.size(); i++)
            {
                if (list[i].first == "LR")
                    cout << pat << " (" << list[i].second.first << "," << list[i].second.second << ") " << list[i].first << endl;
                else if (list[i].first == "TD")
                    cout << pat << " (" << list[i].second.second << "," << list[i].second.first << ") " << list[i].first << endl;
            }
        }
    }
    cout << "The number of words: " << countWord << endl;
}

int main()
{
    char list[100][100];
    int row, col;
    // Read file and store data
    readFile("input.txt", list, row, col);
    // Store matrix which follows the direction
    vector<string> LR = createHorizontalMatrix(list, row, col);
    vector<string> TD = createVerticalMatrix(list, row, col);
    vector<string> word = {"def", "abc", "att"};
    // Brute force
    cout << "Brute force algorithm: " << endl;
    BFsearch(LR, TD, word);
    cout << "------------------------------" << endl;
    // rabin - karp
    cout << "Rabin - karp algorithm: " << endl;
    RKsearch(LR, TD, word);
    cout << "------------------------------" << endl;
    // Knuth-Morris-Pratt
    cout << "Knuth-Morris-Pratt algorithm: " << endl;
    KMPsearch(LR, TD, word);
}