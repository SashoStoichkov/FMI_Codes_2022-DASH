#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <chrono>
#include <random>

using namespace std;

enum class Moves
{
    U = 0,
    D = 1,
    L = 2,
    R = 3,
    F = 4,
    B = 5,
    UPrime = 6,
    DPrime = 7,
    LPrime = 8,
    RPrime = 9,
    FPrime = 10,
    BPrime = 11,
    U2 = 12,
    D2 = 13,
    L2 = 14,
    R2 = 15,
    F2 = 16,
    B2 = 17,
    INVALID = 18
};

enum class Colors
{
    W = 0,
    G = 1,
    O = 2,
    R = 3,
    B = 4,
    Y = 5,
    IRRELEVANT = 6
};

enum class ColorMasks
{
    W = (1<<((int)Colors::W)),
    G = (1<<((int)Colors::G)),
    O = (1<<((int)Colors::O)),
    R = (1<<((int)Colors::R)),
    B = (1<<((int)Colors::B)),
    Y = (1<<((int)Colors::Y)),
};

template <int cubeSz>
struct Cube
{
    int8_t ***a;

    void alloc_a()
    {
        a = new int8_t**[6];
        for(int side = 0;side<6;side++)
        {
            a[side] = new int8_t*[cubeSz];
            for(int i = 0;i<cubeSz;i++) a[side][i] = new int8_t[cubeSz];
        }
    }

    Cube()
    {
        alloc_a();
        for(int col = 0;col<6;col++)
            for(int i = 0;i<cubeSz;i++)
                for(int j = 0;j<cubeSz;j++)
                    a[col][i][j] = col;
    }
    Cube(int8_t a[6][cubeSz][cubeSz])
    {
        for(int col = 0;col<6;col++)
            for(int i = 0;i<cubeSz;i++)
                for(int j = 0;j<cubeSz;j++)
                    this->a[col][i][j] = a[col][i][j];
    }
    Cube(const Cube& other)
    {
        alloc_a();
        for(int col = 0;col<6;col++)
            for(int i = 0;i<cubeSz;i++)
                for(int j = 0;j<cubeSz;j++)
                    this->a[col][i][j] = other.a[col][i][j];
    }

    Cube& operator =(const Cube &other)
    {
        alloc_a();
        for(int col = 0;col<6;col++)
            for(int i = 0;i<cubeSz;i++)
                for(int j = 0;j<cubeSz;j++)
                    this->a[col][i][j] = other.a[col][i][j];
    }

    void rotateClockwise(int8_t **m)
    {
        int8_t aux[cubeSz][cubeSz];
        for(int i = 0;i<cubeSz;i++)
            for(int j = 0;j<cubeSz;j++)
                aux[j][cubeSz-i-1] = m[i][j];

        for(int i = 0;i<cubeSz;i++)
            for(int j = 0;j<cubeSz;j++)
                m[i][j] = aux[i][j];
    }

    void rotateTwice(int8_t **m)
    {
        int8_t aux[cubeSz][cubeSz];
        for(int i = 0;i<cubeSz;i++)
            for(int j = 0;j<cubeSz;j++)
                aux[cubeSz-1-i][cubeSz-1-j] = m[i][j];

        for(int i = 0;i<cubeSz;i++)
            for(int j = 0;j<cubeSz;j++)
                m[i][j] = aux[i][j];
    }

    void R()
    {
        //perform a clockwise rotation on the red side
        rotateClockwise(a[(int)Colors::R]);

        //G -> W -> B -> Y
        for(int i = 0;i<cubeSz;i++)
        {
            swap(a[(int)Colors::Y][i][cubeSz-1], a[(int)Colors::B][cubeSz-1-i][0]);
            swap(a[(int)Colors::B][cubeSz-1-i][0], a[(int)Colors::W][i][cubeSz-1]);
            swap(a[(int)Colors::W][i][cubeSz-1], a[(int)Colors::G][i][cubeSz-1]);
        }
    }
    void L()
    {
        //perform a clockwise rotation on the orange side
        rotateClockwise(a[(int)Colors::O]);

        //Y -> B -> W -> G
        for(int i = 0;i<cubeSz;i++)
        {
            swap(a[(int)Colors::W][cubeSz-1-i][0], a[(int)Colors::G][cubeSz-1-i][0]);
            swap(a[(int)Colors::B][i][cubeSz-1], a[(int)Colors::W][cubeSz-1-i][0]);
            swap(a[(int)Colors::Y][cubeSz-1-i][0], a[(int)Colors::B][i][cubeSz-1]);
        }
    }
    void U()
    {
        //perform a clockwise rotation on the white side
        rotateClockwise(a[(int)Colors::W]);

        //R -> G -> O -> B
        for(int j = 0;j<cubeSz;j++)
        {
            swap(a[(int)Colors::B][0][j], a[(int)Colors::O][0][j]);
            swap(a[(int)Colors::O][0][j], a[(int)Colors::G][0][j]);
            swap(a[(int)Colors::G][0][j], a[(int)Colors::R][0][j]);
        }
    }
    void D()
    {
        //perform a clockwise rotation on the yellow side
        rotateClockwise(a[(int)Colors::Y]);

        //R -> G -> O -> B
        for(int j = 0;j<cubeSz;j++)
        {
            swap(a[(int)Colors::G][cubeSz-1][j], a[(int)Colors::R][cubeSz-1][j]);
            swap(a[(int)Colors::O][cubeSz-1][j], a[(int)Colors::G][cubeSz-1][j]);
            swap(a[(int)Colors::B][cubeSz-1][j], a[(int)Colors::O][cubeSz-1][j]);
        }
    }
    void F()
    {
        //perform a clockwise rotation on the green side
        rotateClockwise(a[(int)Colors::G]);

        //W -> R -> Y -> O
        for(int j = 0;j<cubeSz;j++)
        {
            swap(a[(int)Colors::O][j][cubeSz-1], a[(int)Colors::Y][0][j]);
            swap(a[(int)Colors::Y][0][j], a[(int)Colors::R][cubeSz-1-j][0]);
            swap(a[(int)Colors::R][cubeSz-1-j][0], a[(int)Colors::W][cubeSz-1][cubeSz-1-j]);
        }
    }
    void B()
    {
        //perform a clockwise rotation on the blue side
        rotateClockwise(a[(int)Colors::B]);

        //O -> Y -> R -> W
        for(int j = 0;j<cubeSz;j++)
        {
            swap(a[(int)Colors::W][0][j], a[(int)Colors::R][j][cubeSz-1]);
            swap(a[(int)Colors::R][j][cubeSz-1], a[(int)Colors::Y][cubeSz-1][cubeSz-1-j]);
            swap(a[(int)Colors::Y][cubeSz-1][cubeSz-1-j], a[(int)Colors::O][cubeSz-1-j][0]);
        }
    }

    void R2()
    {
        //perform a double rotation on the red side
        rotateTwice(a[(int)Colors::R]);

        for(int i = 0;i<cubeSz;i++)
        {
            swap(a[(int)Colors::W][i][cubeSz-1], a[(int)Colors::Y][i][cubeSz-1]);
            swap(a[(int)Colors::G][i][cubeSz-1], a[(int)Colors::B][cubeSz-1-i][0]);
        }
    }
    void L2()
    {
        //perform a double rotation on the orange side
        rotateTwice(a[(int)Colors::O]);

        for(int i = 0;i<cubeSz;i++)
        {
            swap(a[(int)Colors::W][i][0], a[(int)Colors::Y][i][0]);
            swap(a[(int)Colors::G][i][0], a[(int)Colors::B][cubeSz-1-i][cubeSz-1]);
        }
    }
    void U2()
    {
        //perform a double rotation on the white side
        rotateTwice(a[(int)Colors::W]);

        for(int i = 0;i<cubeSz;i++)
        {
            swap(a[(int)Colors::R][0][i], a[(int)Colors::O][0][i]);
            swap(a[(int)Colors::G][0][i], a[(int)Colors::B][0][i]);
        }
    }
    void D2()
    {
        //perform a double rotation on the yellow side
        rotateTwice(a[(int)Colors::Y]);

        for(int i = 0;i<cubeSz;i++)
        {
            swap(a[(int)Colors::R][cubeSz-1][i], a[(int)Colors::O][cubeSz-1][i]);
            swap(a[(int)Colors::G][cubeSz-1][i], a[(int)Colors::B][cubeSz-1][i]);
        }
    }
    void F2()
    {
        //perform a double rotation on the green side
        rotateTwice(a[(int)Colors::G]);

        for(int i = 0;i<cubeSz;i++)
        {
            //cout << (int)Colors::Y << " % " << 0 << " " << cubeSz-1-i << "\n";
            //cout << (int)Colors::O << " | " << cubeSz-1-i << " " << cubeSz-1 << "\n";

            swap(a[(int)Colors::W][cubeSz-1][i], a[(int)Colors::Y][0][cubeSz-1-i]);
            swap(a[(int)Colors::R][i][0], a[(int)Colors::O][cubeSz-1-i][cubeSz-1]);
        }
    }
    void B2()
    {
        //perform a double rotation on the blue side
        rotateTwice(a[(int)Colors::B]);

        for(int i = 0;i<cubeSz;i++)
        {
            //cout << (int)Colors::Y << " % " << 0 << " " << cubeSz-1-i << "\n";
            //cout << (int)Colors::O << " | " << cubeSz-1-i << " " << cubeSz-1 << "\n";

            swap(a[(int)Colors::W][0][i], a[(int)Colors::Y][cubeSz-1][cubeSz-1-i]);
            swap(a[(int)Colors::R][i][cubeSz-1], a[(int)Colors::O][cubeSz-1-i][0]);
        }
    }


    void print() const
    {
        for(int col = 0;col<6;col++)
        {
            cout << " --- " << col << " --- " << '\n';
            for(int i = 0;i<cubeSz;i++)
            {
                for(int j = 0;j<cubeSz;j++)
                    cout << (int)a[col][i][j] << " ";
                cout << '\n';
            }
        }
    }

    bool isDone(int8_t **goal, int side)
    {
        for(int i = 0;i<cubeSz;i++)
            for(int j = 0;j<cubeSz;j++)
                if(a[side][i][j]!=goal[i][j] && goal[i][j]!=(int)Colors::IRRELEVANT) return false;

        return true;
    }

    void RPrime()
    {
        ///perform a counterclockwise rotation on the red side
        for(int i=0;i<3;i++)
        {
            this->R();
        }

    }
    void LPrime()
    {
        ///perform a counterclockwise rotation on the orange side
        for(int i=0;i<3;i++)
        {
            this->L();
        }
    }
    void FPrime()
    {
        ///perform a counterclockwise rotation on the green side
        for(int i=0;i<3;i++)
        {
            this->F();
        }
    }
    void DPrime()
    {
        ///perform a counterclockwise rotation on the yellow side
        for(int i=0;i<3;i++)
        {
            this->D();
        }
    }
    void UPrime()
    {
        ///perform a counterclockwise rotation on the white side
        for(int i=0;i<3;i++)
        {
            this->U();
        }
    }
    void BPrime()
    {
        ///perform a counterclockwise rotation on the blue side
        for(int i=0;i<3;i++)
        {
            this->B();
        }
    }

    ~Cube()
    {
        for(int side = 0;side<6;side++)
        {
            for(int i = 0;i<3;i++)
            {
                delete[] a[side][i];
            }

            delete[] a[side];
        }
        delete[] a;
    }

    void performMoves(string s)
    {
        int sz=s.size();
        for(int i=0;i<sz;i++)
        {
            if(s[i]=='F')
            {
                //cout<<"F";
                if(i!=sz-1)
                {
                    if(s[i+1]=='\'')
                    {
                        //cout<<"P";
                        this->FPrime();
                    }
                    else
                    if(s[i+1]=='2')
                    {
                        //cout<<"2";
                        this->F2();
                    }
                    else
                        this->F();
                }
                else
                    this->F();

                //cout<<endl;
            }
            if(s[i]=='D')
            {
                //cout<<"D";
                if(i!=sz-1)
                {
                    if(s[i+1]=='\'')
                    {
                        //cout<<"P";
                        this->DPrime();
                    }
                    else
                    if(s[i+1]=='2')
                    {
                        //cout<<"2";
                        this->D2();
                    }
                    else
                        this->D();
                }
                else
                    this->D();

                //cout<<endl;
            }
            if(s[i]=='U')
            {
                //cout<<"U";
                if(i!=sz-1)
                {
                    if(s[i+1]=='\'')
                    {
                        //cout<<"P";
                        this->UPrime();
                    }
                    else
                    if(s[i+1]=='2')
                    {
                        //cout<<"2";
                        this->U2();
                    }
                    else
                        this->U();
                }
                else
                    this->U();

                //cout<<endl;
            }
            if(s[i]=='B')
            {
                //cout<<"B";
                if(i!=sz-1)
                {
                    if(s[i+1]=='\'')
                    {
                        //cout<<"P";
                        this->BPrime();
                    }
                    else
                    if(s[i+1]=='2')
                    {
                        //cout<<"2";
                        this->B2();
                    }
                    else
                        this->B();
                }
                else
                    this->B();

                //cout<<endl;
            }
            if(s[i]=='R')
            {
                //cout<<"R";
                if(i!=sz-1)
                {
                    if(s[i+1]=='\'')
                    {
                        //cout<<"P";
                        this->RPrime();
                    }
                    else
                    if(s[i+1]=='2')
                    {
                        //cout<<"2";
                        this->R2();
                    }
                    else
                        this->R();
                }
                else
                    this->R();

                //cout<<endl;
            }
            if(s[i]=='L')
            {
                //cout<<"L";
                if(i!=sz-1)
                {
                    if(s[i+1]=='\'')
                    {
                        //cout<<"P";
                        this->LPrime();
                    }
                    else
                    if(s[i+1]=='2')
                    {
                        //cout<<"2";
                        this->L2();
                    }
                    else
                        this->L();
                }
                else
                    this->L();

                //cout<<endl;
            }

        }
    }

    void performMoves(const vector <Moves> &v)
    {
        for(Moves m: v)
        {
            if(m==Moves::U) U();
            else if(m==Moves::UPrime) UPrime();
            else if(m==Moves::U2) U2();
            else if(m==Moves::D) D();
            else if(m==Moves::DPrime) DPrime();
            else if(m==Moves::D2) D2();
            else if(m==Moves::R) R();
            else if(m==Moves::RPrime) RPrime();
            else if(m==Moves::R2) R2();
            else if(m==Moves::L) L();
            else if(m==Moves::LPrime) LPrime();
            else if(m==Moves::L2) L2();
            else if(m==Moves::F) F();
            else if(m==Moves::FPrime) FPrime();
            else if(m==Moves::F2) F2();
            else if(m==Moves::B) B();
            else if(m==Moves::BPrime) BPrime();
            else if(m==Moves::B2) B2();
        }
    }

    void performMoves(const vector <int> &v)
    {
        for(int m: v)
        {
            if(m==(int)Moves::U) U();
            else if(m==(int)Moves::UPrime) UPrime();
            else if(m==(int)Moves::U2) U2();
            else if(m==(int)Moves::D) D();
            else if(m==(int)Moves::DPrime) DPrime();
            else if(m==(int)Moves::D2) D2();
            else if(m==(int)Moves::R) R();
            else if(m==(int)Moves::RPrime) RPrime();
            else if(m==(int)Moves::R2) R2();
            else if(m==(int)Moves::L) L();
            else if(m==(int)Moves::LPrime) LPrime();
            else if(m==(int)Moves::L2) L2();
            else if(m==(int)Moves::F) F();
            else if(m==(int)Moves::FPrime) FPrime();
            else if(m==(int)Moves::F2) F2();
            else if(m==(int)Moves::B) B();
            else if(m==(int)Moves::BPrime) BPrime();
            else if(m==(int)Moves::B2) B2();
        }
    }
};

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

int encodeSide(int8_t **m)
{
    int ans = 0;
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            if(i==1 || j==1)
            {
                ans = (ans*6 + m[i][j]);
            }
        }
    }

    return ans;
}

void initOppositeMoves(int col, vector <Moves> &oppositeMoves)
{
    if(col==0)
    {
        oppositeMoves.push_back(Moves::D);
        oppositeMoves.push_back(Moves::DPrime);
        oppositeMoves.push_back(Moves::D2);
    }
    else if(col==1)
    {
        oppositeMoves.push_back(Moves::B);
        oppositeMoves.push_back(Moves::BPrime);
        oppositeMoves.push_back(Moves::B2);
    }
    else if(col==2)
    {
        oppositeMoves.push_back(Moves::R);
        oppositeMoves.push_back(Moves::RPrime);
        oppositeMoves.push_back(Moves::R2);
    }
    else if(col==3)
    {
        oppositeMoves.push_back(Moves::L);
        oppositeMoves.push_back(Moves::LPrime);
        oppositeMoves.push_back(Moves::L2);
    }
    else if(col==4)
    {
        oppositeMoves.push_back(Moves::F);
        oppositeMoves.push_back(Moves::FPrime);
        oppositeMoves.push_back(Moves::F2);
    }
    else if(col==5)
    {
        oppositeMoves.push_back(Moves::U);
        oppositeMoves.push_back(Moves::UPrime);
        oppositeMoves.push_back(Moves::U2);
    }
}

Moves getOpposite(Moves m)
{
    if(m==Moves::R) return Moves::RPrime;
    if(m==Moves::RPrime) return Moves::R;
    if(m==Moves::R2) return Moves::R2;
    if(m==Moves::L) return Moves::LPrime;
    if(m==Moves::LPrime) return Moves::L;
    if(m==Moves::L2) return Moves::L2;
    if(m==Moves::U) return Moves::UPrime;
    if(m==Moves::UPrime) return Moves::U;
    if(m==Moves::U2) return Moves::U2;
    if(m==Moves::D) return Moves::DPrime;
    if(m==Moves::DPrime) return Moves::D;
    if(m==Moves::D2) return Moves::D2;
    if(m==Moves::F) return Moves::FPrime;
    if(m==Moves::FPrime) return Moves::F;
    if(m==Moves::F2) return Moves::F2;
    if(m==Moves::B) return Moves::BPrime;
    if(m==Moves::BPrime) return Moves::B;
    if(m==Moves::B2) return Moves::B2;
}

vector <int> crossMoves[(int)1e4];

void loadDatabase()
{
    ifstream fin("database.txt");

    int h;
    while(fin >> h)
    {
        int cnt;
        fin >> cnt;

        crossMoves[h].resize(cnt);
        for(int i = 0;i<cnt;i++)
            fin >> crossMoves[h][i];
    }

    fin.close();
}

vector <Moves> convertToMovesVector(vector <int> v)
{
    vector <Moves> ans;
    for(int x: v) ans.push_back((Moves)x);

    return ans;
}

struct Character
{
    int num;
    string s;
    void init_characters();
};
void Character::init_characters()
{
    if(num==0)s="U";
    if(num==1)s="D";
    if(num==2)s="L";
    if(num==3)s="R";
    if(num==4)s="F";
    if(num==5)s="B";
    if(num==6)s="U'";
    if(num==7)s="D'";
    if(num==8)s="L'";
    if(num==9)s="R'";
    if(num==10)s="F'";
    if(num==11)s="B'";
    if(num==12)s="U2";
    if(num==13)s="D2";
    if(num==14)s="L2";
    if(num==15)s="R2";
    if(num==16)s="F2";
    if(num==17)s="B2";
    if(num==18)s="Invalid";

};
void assign_values(Character* ch)
{
    for(int i=0;i<=18;i++)
    {
        ch[i].num=i;
        ch[i].init_characters();
    }
}

pair<char,int> getNum(Moves m)
{
    if(m==Moves::R)return {'R',1};
    if(m==Moves::RPrime)return {'R',-1};
    if(m==Moves::R2)return {'R',2};
    if(m==Moves::L) return {'L',1};
    if(m==Moves::LPrime) return {'L',-1};
    if(m==Moves::L2) return {'L',2};
    if(m==Moves::U) return {'U',1};
    if(m==Moves::UPrime) return {'U',-1};
    if(m==Moves::U2) return {'U',2};
    if(m==Moves::D) return {'D',1};
    if(m==Moves::DPrime) return {'D',-1};
    if(m==Moves::D2) return {'D',2};
    if(m==Moves::F) return {'F',1};
    if(m==Moves::FPrime) return {'F',-1};
    if(m==Moves::F2) return {'F',2};
    if(m==Moves::B) return {'B',1};
    if(m==Moves::BPrime) return {'B',-1};
    if(m==Moves::B2) return {'B',2};

}

string add_to_output(pair<char,int>p)
{
    string s="";
    int val=(p.second+4*100)%4;
    if(p.first=='U')
    {
        if(val==1)
        {
            s="U";
        }
        if(val==2)
        {
            s="U2";
        }
        if(val==3)
        {
            s="U'";
        }
    }
    if(p.first=='D')
    {
        if(val==1)
        {
            s="D";
        }
        if(val==2)
        {
            s="D2";
        }
        if(val==3)
        {
            s="D'";
        }
    }
    if(p.first=='R')
    {
        if(val==1)
        {
            s="R";
        }
        if(val==2)
        {
            s="R2";
        }
        if(val==3)
        {
            s="R'";
        }
    }
    if(p.first=='L')
    {
        if(val==1)
        {
            s="L";
        }
        if(val==2)
        {
            s="L2";
        }
        if(val==3)
        {
            s="L'";
        }
    }
    if(p.first=='F')
    {
        if(val==1)
        {
            s="F";
        }
        if(val==2)
        {
            s="F2";
        }
        if(val==3)
        {
            s="F'";
        }
    }
    if(p.first=='B')
    {
        if(val==1)
        {
            s="B";
        }
        if(val==2)
        {
            s="B2";
        }
        if(val==3)
        {
            s="B'";
        }
    }

    return s;
}

///----------------------------------------------
string test_string(vector<int>v)
{
    string output="", temp="";
    Character ch[20];
    assign_values(ch);
    int sz=v.size();
    pair<char,int>p,p2;

    vector <pair <char, int>> aux;
    for(int i=0;i<v.size();i++)
    {
        if(aux.empty()==false)
        {
            aux.back().second = (aux.back().second + 4*100)%4;
            if(aux.back().second==0) aux.pop_back();
        }

        p2=getNum((Moves)v[i]);
        if(aux.empty()==false && aux.back().first==p2.first)
            aux.back().second += p2.second;
        else
        {
            aux.push_back(p2);
        }
    }

    if(aux.empty()==false)
    {
        aux.back().second = (aux.back().second + 4*100)%4;
        if(aux.back().second==0) aux.pop_back();
    }

    for(pair <char, int> item: aux)
    {
        if(output!="") output += " ";
        output += add_to_output(item);
    }

    return output;
}

string grid2MovesString(int8_t **m)
{
    vector <Moves> finalMoves = convertToMovesVector(crossMoves[encodeSide(m)]);

    Cube<3> c;
    c.performMoves(crossMoves[encodeSide(m)]);

    //c.print();

    vector <Moves> oppositeMoves;
    initOppositeMoves(m[1][1], oppositeMoves);

    vector <vector <Moves>> sexyMoves;
    for(int i = 0;i<18;i++)
    {
        Moves m1 = (Moves)i;
        for(Moves m2: oppositeMoves)
        {
            Moves m3 = getOpposite(m1);
            sexyMoves.push_back(vector <Moves>{m1, m2, m3});
        }
    }

    vector <vector<Moves>> allInsertMoves;
    for(auto item: sexyMoves)
        allInsertMoves.push_back(item);
    for(vector <Moves> sexy: sexyMoves)
    {
        for(Moves m: oppositeMoves)
        {
            vector <Moves> add = {m};
            for(Moves x: sexy) add.push_back(x);

            allInsertMoves.push_back(add);
        }
    }
    for(vector <Moves> sexy: sexyMoves)
    {
        for(Moves m: oppositeMoves)
        {
            vector <Moves> add = sexy;
            add.push_back(m);

            allInsertMoves.push_back(add);
        }
    }
    for(vector <Moves> sexy1: sexyMoves)
    {
        for(vector <Moves> sexy2: sexyMoves)
        {
            for(Moves m: oppositeMoves)
            {
                vector <Moves> add = sexy1;
                add.push_back(m);
                for(Moves x: sexy2) add.push_back(x);

                allInsertMoves.push_back(add);
            }
        }
    }

    /*
    sort(allInsertMoves.begin(), allInsertMoves.end(),
    [](const vector <Moves> &A, const vector <Moves> &B)
    {
        return A.size() < B.size();
    });
    */

    int side = (int)m[1][1];
    while(c.isDone(m, side)==false)
    {
        bool correct[3][3];
        for(int i = 0;i<3;i++)
            for(int j = 0;j<3;j++)
                correct[i][j] = (c.a[side][i][j]==m[i][j]);

        bool found = false;
        for(const vector <Moves> &v: allInsertMoves)
        {
            Cube<3> newC(c);
            newC.performMoves(v);

            bool newCorrect[3][3];
            for(int i = 0;i<3;i++)
                for(int j = 0;j<3;j++)
                    newCorrect[i][j] = (newC.a[side][i][j]==m[i][j]);

            bool fail = false, improvement = false;
            for(int i = 0;i<3;i++)
            {
                for(int j = 0;j<3;j++)
                {
                    if(correct[i][j]==true && newCorrect[i][j]==false) fail = true;
                    if(correct[i][j]==false && newCorrect[i][j]==true) improvement = true;
                }
            }

            if(fail==false && improvement==true)
            {
                c = newC;
                for(Moves x: v) finalMoves.push_back(x);

                found = true;
                break;
            }
        }

        if(found==false)
        {
            return "SOLUTION NOT FOUND";
        }
    }

    //for(Moves x: finalMoves) cout << (int)x << " ";
    //cout << '\n';

    vector <int> finalMovesInt;
    for(Moves x: finalMoves) finalMovesInt.push_back((int)x);

    return test_string(finalMovesInt);
}

void stressTest()
{
    mt19937 rnd((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

    int8_t **m = new int8_t*[3];
    for(int i = 0;i<3;i++)
        m[i] = new int8_t[3];

    while(true)
    {
        for(int i = 0;i<3;i++)
            for(int j = 0;j<3;j++)
                m[i][j] = rnd()%6;

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j<3;j++)
                cout << (int)m[i][j] << " ";
            cout << '\n';
        }

        Cube <3> c;
        string moves = grid2MovesString(m);
        cout << moves << '\n';

        c.performMoves(moves);
        if(c.isDone(m, m[1][1])==false)
        {
            cout << "FAK" << '\n';
            system("PAUSE");
        }

        //cin.get();
    }
}

int main()
{
    loadDatabase();

    ifstream fin("output.txt");
    ofstream fout("solution.txt");

    int **input = new int*[3];
    for(int i = 0;i<3;i++) input[i] = new int[3];

    int8_t **m = new int8_t*[3];
    for(int i = 0;i<3;i++) m[i] = new int8_t[3];

    while(fin >> input[0][0])
    {
        m[0][0] = input[0][0];
        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j<3;j++)
            {
                if(i==0 && j==0) continue;

                fin >> input[i][j];
                m[i][j] = input[i][j];
            }
        }

        string moves = grid2MovesString(m);
        if(moves=="") moves = "No moves required";

        fout << moves << '\n';
    }

    fin.close();
    fout.close();
}

