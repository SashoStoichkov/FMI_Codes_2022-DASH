#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <queue>
#include <map>
#include <set>

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

    void RPrime()
    {
        //perform a counterclockwise rotation on the red side
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

    bool isDone(int8_t goal[cubeSz][cubeSz], int side)
    {
        for(int i = 0;i<cubeSz;i++)
            for(int j = 0;j<cubeSz;j++)
                if(a[side][i][j]!=goal[i][j] && goal[i][j]!=(int)Colors::IRRELEVANT) return false;

        return true;
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
};



int relevantColorMask1 = (int)ColorMasks::W + (int)ColorMasks::G + (int)ColorMasks::O +
                         (int)ColorMasks::R + (int)ColorMasks::B + (int)ColorMasks::Y;

bool cubeCmp1(const Cube<3> &A, const Cube<3> &B)
{
    for(int side = 0;side<6;side++)
    {
        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j<3;j++)
            {
                //int a = ((((relevantColorMask>>A.a[side][i][j])&1)==1)?A.a[side][i][j]:(int8_t)Colors::IRRELEVANT);
                //int b = ((((relevantColorMask>>B.a[side][i][j])&1)==1)?B.a[side][i][j]:(int8_t)Colors::IRRELEVANT);

                if(A.a[side][i][j]!=B.a[side][i][j]) return A.a[side][i][j]<B.a[side][i][j];
            }
        }
    }

    return false;
}

struct MoveList
{
    MoveList *last;
    Moves lastMove;

    MoveList() : last(nullptr), lastMove(Moves::INVALID) {}
    MoveList(MoveList *_last, Moves _lastMove) : last(_last), lastMove(_lastMove) {}
};

struct State
{
    Cube<3> c;
    int depth;
    Moves lastMove;
    MoveList *curr;

    State(){}
    State(Cube<3> _c, Moves _lastMove, int _depth) : c(_c), lastMove(_lastMove), depth(_depth) {}
    State(Cube<3> _c, Moves _lastMove, int _depth, MoveList *last) : c(_c), lastMove(_lastMove), depth(_depth)
    {
        curr = new MoveList(last, lastMove);
    }
};

/*
struct StateCmp1
{
    bool operator ()(const State &s1, const State &s2)
    {
        if(s1.depth!=s2.depth) return s1.depth < s2.depth;
        if(s1.lastMove!=s2.lastMove) return s1.lastMove < s2.lastMove;

        return cubeCmp1(s1.c, s2.c);
    }
};
*/

bool operator <(const State &s1, const State &s2)
{
    if(s1.lastMove!=s2.lastMove) return ((int)s1.lastMove) < ((int)s2.lastMove);
    return cubeCmp1(s1.c, s2.c);
}

int encodeSide(int8_t **m)
{
    int ans = 0;
    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            if(i==1 || j==1)
                ans = (ans*6 + m[i][j]);
        }
    }

    return ans;
}



const int MAXHASH = 1e5;
int fewestMoves[MAXHASH+5];
Moves fewestMovesList[MAXHASH+5][10];

map <State, int> memo;

Moves currMoves[10];
int recCross(Cube<3> &c, int depth, Moves lastMove, int8_t goal[3][3], const int MAX_DEPTH = 7)
{
    if(depth!=0)
        currMoves[depth-1] = lastMove;

    for(int i = 0;i<6;i++)
    {
        int h = encodeSide(c.a[i]);

        if(depth<fewestMoves[h])
        {
            fewestMoves[h] = depth;
            for(int j = 0;j<depth;j++) fewestMovesList[h][j] = currMoves[j];
        }
    }

    //if(c.isDone(goal, goal[1][1])==true) return 0;
    if(depth>=MAX_DEPTH)
    {


        return 69;
    }

    /*
    auto it = memo.find(State(c, lastMove, 0));
    if(it!=memo.end())
    {
        return it->second;
    }
    */

    int ans = 69;
    if(lastMove!=Moves::R && lastMove!=Moves::RPrime && lastMove!=Moves::R2
       && lastMove!=Moves::L && lastMove!=Moves::LPrime && lastMove!=Moves::L2)
    {
        c.R();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::R, goal));

        c.R();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::R2, goal));

        c.R();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::RPrime, goal));

        c.R();
    }
    if(lastMove!=Moves::L && lastMove!=Moves::LPrime && lastMove!=Moves::L2)
    {
        c.L();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::L, goal));

        c.L();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::L2, goal));

        c.L();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::LPrime, goal));

        c.L();
    }
    if(lastMove!=Moves::U && lastMove!=Moves::UPrime && lastMove!=Moves::U2
       && lastMove!=Moves::D && lastMove!=Moves::DPrime && lastMove!=Moves::D2)
    {
        c.U();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::U, goal));

        c.U();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::U2, goal));

        c.U();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::UPrime, goal));

        c.U();
    }
    if(lastMove!=Moves::D && lastMove!=Moves::DPrime && lastMove!=Moves::D2)
    {
        c.D();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::D, goal));

        c.D();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::D2, goal));

        c.D();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::DPrime, goal));

        c.D();
    }
    if(lastMove!=Moves::F && lastMove!=Moves::FPrime && lastMove!=Moves::F2
       && lastMove!=Moves::B && lastMove!=Moves::BPrime && lastMove!=Moves::B2)
    {
        c.F();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::F, goal));

        c.F();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::F2, goal));

        c.F();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::FPrime, goal));

        c.F();
    }
    if(lastMove!=Moves::B && lastMove!=Moves::BPrime && lastMove!=Moves::B2)
    {
        c.B();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::B, goal));

        c.B();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::B2, goal));

        c.B();
        ans = min(ans, 1 + recCross(c, depth+1, Moves::BPrime, goal));

        c.B();
    }

    //if(ans<30) memo[State(c, lastMove, 0)] = ans;
    return ans;
}

vector <Moves> recover(MoveList *l)
{
    vector <Moves> res;

    while(l!=nullptr)
    {
        res.push_back(l->lastMove);
        l = l->last;
    }

    reverse(res.begin(), res.end());
    return res;
}

set <State> used;
int bfs(Cube <3> c, int8_t goal[3][3])
{
    queue <State> q;

    State s = State(c, Moves::INVALID, 0);
    s.curr = nullptr;
    q.push(s);

    int lastDepth = 0;
    while(q.empty()==false)
    {
        State s = q.front();
        q.pop();

        if(q.size()>5e6)
            cout << "ok" << '\n';

        if(s.depth!=lastDepth)
        {
            cout << s.depth << " --- " << q.size() << '\n';
            lastDepth = s.depth;
        }
        if(s.c.isDone(goal, goal[1][1])==true)
        {
            /*
            vector <Moves> res = recover(s.curr);

            cout << res.size() << '\n';
            for(Moves m: res) cout << (int)m << " ";
            cout << '\n';

            return s.depth;
            */
        }

        if(s.lastMove!=Moves::R && s.lastMove!=Moves::RPrime && s.lastMove!=Moves::R2
           && s.lastMove!=Moves::L && s.lastMove!=Moves::LPrime && s.lastMove!=Moves::L2)
        {
            s.c.R();
            q.push(State(s.c, Moves::R, s.depth+1, s.curr));

            s.c.R();
            q.push(State(s.c, Moves::R2, s.depth+1, s.curr));

            s.c.R();
            q.push(State(s.c, Moves::RPrime, s.depth+1, s.curr));

            s.c.R();
        }

        if(s.lastMove!=Moves::L && s.lastMove!=Moves::LPrime && s.lastMove!=Moves::L2)
        {
            s.c.L();
            q.emplace(s.c, Moves::L, s.depth+1, s.curr);

            s.c.L();
            q.emplace(s.c, Moves::L2, s.depth+1, s.curr);

            s.c.L();
            q.emplace(s.c, Moves::LPrime, s.depth+1, s.curr);

            s.c.L();
        }

        if(s.lastMove!=Moves::U && s.lastMove!=Moves::UPrime && s.lastMove!=Moves::U2
           && s.lastMove!=Moves::D && s.lastMove!=Moves::DPrime && s.lastMove!=Moves::D2)
        {
            s.c.U();
            q.emplace(s.c, Moves::U, s.depth+1, s.curr);

            s.c.U();
            q.emplace(s.c, Moves::U2, s.depth+1, s.curr);

            s.c.U();
            q.emplace(s.c, Moves::UPrime, s.depth+1, s.curr);

            s.c.U();
        }
        if(s.lastMove!=Moves::D && s.lastMove!=Moves::DPrime && s.lastMove!=Moves::D2)
        {
            s.c.D();
            q.emplace(s.c, Moves::D, s.depth+1, s.curr);

            s.c.D();
            q.emplace(s.c, Moves::D2, s.depth+1, s.curr);

            s.c.D();
            q.emplace(s.c, Moves::DPrime, s.depth+1, s.curr);

            s.c.D();
        }
        if(s.lastMove!=Moves::F && s.lastMove!=Moves::FPrime && s.lastMove!=Moves::F2
           && s.lastMove!=Moves::B && s.lastMove!=Moves::BPrime && s.lastMove!=Moves::B2)
        {
            s.c.F();
            q.emplace(s.c, Moves::F, s.depth+1, s.curr);

            s.c.F();
            q.emplace(s.c, Moves::F2, s.depth+1, s.curr);

            s.c.F();
            q.emplace(s.c, Moves::FPrime, s.depth+1, s.curr);

            s.c.F();
        }
        if(s.lastMove!=Moves::B && s.lastMove!=Moves::BPrime && s.lastMove!=Moves::B2)
        {
            s.c.B();
            q.emplace(s.c, Moves::B, s.depth+1, s.curr);

            s.c.B();
            q.emplace(s.c, Moves::B2, s.depth+1, s.curr);

            s.c.B();
            q.emplace(s.c, Moves::BPrime, s.depth+1, s.curr);

            s.c.B();
        }
    }

    return 420;
}

int main()
{
    //StateCmp1 cmp1;
    //memo = map <State, int, StateCmp1>(cmp1);

    for(int h = 0;h<=MAXHASH;h++)
        fewestMoves[h] = 69;

    Cube<3> c;
    int8_t goal[3][3];
    /*
    {
        {4, 0, 1},
        {4, 0, 1},
        {4, 0, 1}
    };
    */
    //R U2 D L2 F
    /*
    {
        {4, 2, 2},
        {0, 3, 3},
        {0, 1, 5}
    };
    */
    /*
    {
        {4, 2, 6},
        {0, 3, 3},
        {6, 1, 6}
    };
    */
    /*
    {
        {4, 1, 3},
        {5, 1, 3},
        {5, 0, 0}
    };
    */
    /*
    {
        {4, 1, 0},
        {1, 2, 0},
        {0, 3, 2}
    };
    */
    /*
    {
        {6, 1, 6},
        {1, 2, 0},
        {6, 3, 6}
    };
    */
    /*
    {
        {6, 1, 6},
        {1, 2, 0},
        {6, 0, 6}
    };
    */
    /*
    {
        {6, 1, 6},
        {3, 0, 2},
        {6, 4, 6},
    };
    */

    cout << recCross(c, 0, Moves::INVALID, goal) << '\n';
    //cout << bfs(c, goal) << '\n';

    /*
    c.R();
    c.U2();
    c.D();
    c.L2();
    c.F();

    c.print();
    */

    ofstream database("database.txt");
    for(int h = 0;h<=MAXHASH;h++)
    {
        if(fewestMoves[h]<10)
        {
            database << h << " " << fewestMoves[h];
            for(int i = 0;i<fewestMoves[h];i++) database << " " << (int)fewestMovesList[h][i];
            database << '\n';
        }
    }
}
