#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cstring>

#define STR_LIMIT 5000
//matrix size limits
#define MAX_M 1000
#define MAX_N 1000

int state[MAX_M][MAX_N];
/* this matrix keeps track of the operations
x        Deletion
c        Insertion of a character c
-        Keep
[c->d]   Change (c to d)
*/
char operation[MAX_M][MAX_N];

// c_i, c_r and c_s stand for insertion, removal and substituion costs, respectively
int c_i = 1, c_r = 1, c_s = 1;

using std::string;

/*
    Bottom-up implementation returning a string with the operations.
    Strongly based on @edsomjr's implementation with some improvements 
    on readability and minor tweaks.
*/
int edit_distance(const string& str_1, const string& str_2)
{
    int m = str_1.size();
    int n = str_2.size();

    for (int i = 0; i <= m; i++)
    {
        state[i][0] = i * c_r;
        operation[i][0] = 'r';
    }

    for (int j = 1; j <= n; j++)
    {
        state[0][j] = j*c_i;
        operation[0][j] = 'i';
    }

    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
        {
            int insertion = state[i][j - 1] + c_i;
            int deletion = state[i - 1][j] + c_r;
            int change = state[i - 1][j - 1] + c_s * (str_1[i - 1] == str_2[j - 1] ? 0 : 1);

            state[i][j] = std::min(insertion, deletion);
            state[i][j] = std::min(state[i][j], change);

            if (insertion <= deletion and insertion <= change)
                operation[i][j] = 'i';
            else if (deletion <= change)
                operation[i][j] = 'r';
            else
                operation[i][j] = 's';
        }

        return state[m][n];
}

string show_operations(const string& str_1, const string& str_2)
{
    int i = str_1.size(), j = str_2.size();
    std::stack<string> S;
    char operations_buffer[STR_LIMIT];

    while (i != 0 or j != 0)
    {
        switch (operation[i][j]) {
            case 'i':
                sprintf(operations_buffer, "add \"%c\" to index %d\n", str_2[j-1], (j-1));
                j--;
                break;

            case 'r':
                sprintf(operations_buffer, "remove \"%c\"\n", str_1[i-1]);
                i--;
                break;

            case 's':
                if (str_1[i-1] == str_2[j-1])
                    sprintf(operations_buffer, "keep");
                else
                    sprintf(operations_buffer, "swap \"%c\" to \"%c\"\n", str_1[i-1], str_2[j-1]);
                
                i--;j--;
            }

            if (strncmp(operations_buffer, "keep", 5) != 0)
                S.push(operations_buffer);
    }

    std::ostringstream os;

    while (not S.empty())
    {
        os << S.top();
        S.pop();
    }

    return os.str();
}

int main()
{
    string str_1, str_2;
    std::cout << "Insert two strings in the format \"str1 str2\": ";
    std::cin >> str_1 >> str_2;

    int result = edit_distance(str_1, str_2);

    std::cout << "\nThe edit distance is: " << result << std::endl;
    std::cout << "\nThe edit script is: \n" << show_operations(str_1, str_2) << std::endl;
}