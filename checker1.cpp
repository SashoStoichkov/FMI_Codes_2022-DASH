#include <iostream>
#include <fstream>

using namespace std;

int** hash2Cross(int h)
{
    int **m = new int*[3];
    for(int i = 0;i<3;i++)
        m[i] = new int[3];

    for(int i = 2;i>=0;i--)
    {
        for(int j = 2;j>=0;j--)
        {
            if(i==1 || j==1)
            {
                m[i][j] = h%6;
                h /= 6;
            }
            else
                m[i][j] = 6;
        }
    }

    return m;
}

int main()
{
    int h;
    cin >> h;

    int **m = hash2Cross(h);
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
            cout << m[i][j] << " ";
        cout << '\n';
    }

}
