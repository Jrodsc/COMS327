#include <bits/stdc++.h>
#include <ncurses.h>

using namespace std;
using cd = complex<double>;
const double PI = acos(-1);
const double EPS = 1e-18;

template <typename T>
class Matrix{
    private:
        vector<vector<T> > M;
        size_t Col, Row;

    public:
        Matrix(vector<vector<T> > & m){
            M = m;
            Row = m.size();
            Col = m[0].size();
        }

        ~Matrix(){
        }

        int Gauss_Jordan(vector<T> & ans){
            vector<int> n_z(Col - 1, -1);
            int i, j, select, column = 0, row = 0;
            double c, sum;

            for(; row < Row && column < Col - 1; column++){
                select = row;

                for(i = row; i < Row; i++){
                    if(abs(M[i][column]) > abs(M[select][column]))
                        select = i;
                }

                if(abs(M[select][column]) < EPS)
                    continue;

                for(i = column; i < Col; i++){
                    swap(M[select][i], M[row][i]);
                }

                n_z[column] = row;

                for(i = 0; i < Row; i++){
                    if(i != row){
                        c = M[i][column] / M[row][column];
                        for(j = column; j < Col; j++)
                            M[i][j] -= M[row][j] * c;
                    }
                }
                row++;

            }

            ans.assign(Col - 1, 0);
            for(i = 0; i < Col - 1; i++){
                if(n_z[i] != -1){
                    ans[i] = M[n_z[i]][Col - 1] / M[n_z[i]][i];
                }
            }

            for(i = 0; i < Row; i++){
                sum = 0;

                for(j = 0; j < Col -1; j++)
                    sum += ans[j] * M[i][j];

                if(abs(sum - M[i][Col -1]) > EPS)
                    return 0;
            }

            for(i = 0; i < Col - 1; i++){
                if(n_z[i] == -1)
                    return -1;
            }

            return 1;


        }

};
template <typename T>
class Polynomio{
    private:

        void fft(vector<cd> & a, bool invert){
            int n = a.size();

            for(int i = 1, j = 0; i < n; i++){
                int bit = n >> 1;

                for(; j & bit; bit >>= 1)
                    j ^= bit;

                j ^= bit;

                if(i < j)
                    swap(a[i], a[j]);
            }

            for(int len = 2; len <= n; len <<= 1){
                double ang = 2 * PI / len * (invert ? -1 : 1);
                cd wlen(cos(ang), sin(ang));

                for(int i = 0; i < n; i += len){
                    cd w(1);

                    for(int j = 0; j < len / 2; j++){
                        cd u = a[i + j], v = a[i + j + len/2] * w;

                        a[i + j] = u + v;
                        a[i + j + len/2] = u - v;
                        w *= wlen;
                    }
                }
            }

            if(invert){
                for(cd & x : a)
                    x /= n;
            }
        }

        auto mult(vector<T> const & a, vector<T> const & b){
            cd x;
            vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
            size_t n = 1;

            while(n < fa.size() + fb.size())
                n <<= 1;

            vector<cd> c(n);

            fa.resize(n);
            fb.resize(n);

            fft(fa, 0);
            fft(fb, 0);


            for(int i = 0; i < (int)n; i++){
                c[i] = fa[i] * fb[i];
            }

            fft(c, 1);

            vector<T> cT;
            

            for(int i = 0; i < (int)a.size() + (int)b.size() - 1; i++){
                x = c[i];
                cT.push_back(real(x));
            }
        

            return cT;
        }

        void slow_DFT(vector<pair<T,T>> & a){
             
        }

        T P(T x){
            T ans = 0;

            for(int i = 0; i < n; i++){
                ans += A[i] * pow(x, i);
            }

            return ans;
        }

    public:

        vector<T> A;
        size_t n;
        Polynomio(){
        }

        Polynomio(vector<T> & a){
            A = a;
            n = A.size();
        }
        ~Polynomio(){}

        void printPoly(){
            for(auto x : A){
                cout << x << ' ';
            }
            cout << endl;
        }

        vector<cd> FFT(){
            vector<cd> t;

            for(int i = 0; i < n; i++){
                t.push_back((cd)A[i]);
            }

            fft(t, 0);

            return t;
        }

        vector<cd> IFFT(){
            vector<cd> t;

            for(int i = 0; i < n; i++){
                t.push_back((cd)A[i]);
            }

            fft(t, 1);

            return t;
        } 

        vector<cd> mult(Polynomio<T> &p){
            return mult(A, p.A);
        }

        Polynomio<T> operator*(Polynomio<T> &p){
            vector<T> tmp_v = mult(A, p.A);
            Polynomio<T> tmp(tmp_v);  
            return tmp;
        }

        Polynomio<T> deriv(){
            vector<T> p_prime;
            for(int i = 1; i < n; i++){
                p_prime.push_back(A[i] * i); 
            }
            Polynomio<T> p(p_prime);
            return p;
        }

        Polynomio<T> integr(){
            vector<T> s_p;
            s_p.push_back(0);
            for(int i = 0; i < n; i++){
                s_p.push_back(A[i]/(i + 1));
            }

            Polynomio<T> p(s_p);
            return p;
        }

        Polynomio<T> eu_div(Polynomio<T> & p){
            int m = p.n;

            if(n < m) return *this;
            return *this;
        }
};

template <typename T>
Polynomio<T> IFT(vector<pair<T, T> > point){
    int j, i, n = point.size();
    T x, y;

    vector<vector<T> > m(n, vector<T>(n + 1));

    for(i = 0; i < n; i++){
        y = point[i].second; 
        x = 1;
        
        for(j = 0; j < n; j++, x *= point[i].first){
            m[i][j] = x;
        }

        m[i][n] = y;
    }

    Matrix<T> mat(m);

    vector<T> coeff;

    mat.Gauss_Jordan(coeff);

    Polynomio<T> poly(coeff);

    return poly;
}

template <typename T>
istream & operator>>(istream & i, pair<T, T> p){
    i >> p.first >> p.second;
    return i;
}

template <typename T>
int read_v(vector<T> &v, size_t n){
    T x;
    v.clear();
    for(size_t i = 0; i < n; i++){
        cin >> x;
        v.push_back(x);
    }
    return n;
}

template <typename T>
int read_v_p(vector<pair<T, T> > &v, size_t n){
    T x, y;
    v.clear();
    for(size_t i = 0; i < n; i++){
        cin >> x >> y;
        v.push_back(make_pair(x,y));
    }
    return n;
}

Polynomio<int> a_i;
Polynomio<int> b_i;
Polynomio<int> c_i;

Polynomio<long double> a_ld;
Polynomio<long double> b_ld;
Polynomio<long double> c_ld;

Polynomio<cd> a_cd;
Polynomio<cd> b_cd;
Polynomio<cd> c_cd;

vector<pair<int,int> > ii;
vector<pair<long double,long double> > dd;
vector<pair<cd,cd> > cc;

size_t n,m,l;

void read_one(int tp_op, bool pol){
    system("clear");
    cout << "Please enter the size of the " << ((pol) ? "polynomio" : "set") << "\n";
    
    cin >> n;

    cout << "Now enter its " << ((pol) ? "coefficients" : "values") << " separated by a single space\n";
    cout << "If you are using complex numbers use the (a, b) format, when:\n";
    cout << "x = a + bi\n";
    cout << endl;

    switch(tp_op){
        case 1:
            a_i.n = read_v(a_i.A, n);
            break;
        case 2:
            a_ld.n = read_v(a_ld.A, n);
            break;
        case 3:
            a_cd.n = read_v(a_cd.A, n);
            break;
    }
}

void read_pair(int tp_op, int n){
    system("clear");
    cout << "Please enter the size of the set\n";
    
    cin >> n;

    cout << "Now enter its values separated by a single space\n";
    cout << "If you are using complex numbers use the (a, b) format, when:\n";
    cout << "x = a + bi\n";
    cout << endl;

    ////ii.clear();
    //dd.clear();
    //cd.clear();

    switch(tp_op){
        case 1:
            read_v_p(ii, n);
            break;
        case 2:
            read_v_p(dd, n);
            break;
        case 3:
            break;
    }

    for(pair<int,int> x : ii){
        cout << x.first << ' ' << x.second << endl;
    }
    cout << endl;
}

cd translate(int j, int n){
    cd ang = (2 * PI * j)/n;
     
    return cos(ang) + sqrt(-1) * sin(ang);
}
cd translateP(int j, int n){
    cd ang = (2 * PI * j)/n;
     
    return cos(ang) - sqrt(-1) * sin(ang);
}

int main(){
    
    int opt, tp_op;

    string s, t;

    do{
        system("clear");
        cout << "\tHello, with this calculator you can do:\n";
        cout << "\t\t1. Polynomial multiplication\n";
        cout << "\t\t2. Find the DFT of a polynomial\n";
        cout << "\t\t3. Find the Inverse DFT from a set of complex values\n";
        cout << "\t\t4. Find the n-deg polynomio from a set of n points\n";
        cout << "\t\t5. Find the derivative of a polynomio\n";
        cout << "\t\t6. Find the integral of a polynomio\n";
        cout << "\t\t7. Find the of ocurrences of the pattern P in the string S\n";
        cout << "\t\ttbc\n";
        cout << "\nEnter the number of the operation following of the Enter key" << endl;

        cin >> opt;

        if(opt > 0 && opt < 5){
            cout << "Select the data type of your polynomios\n";
            cout << "\t1. Integer\n\t2. Double\n\t3. Complex\n";
        }
        
        cin >> tp_op;

        switch(opt){
            case 1:
                system("clear");
                cout << "Please enter the size of both polynomios\n";
                
                cin >> n >> m;

                cout << "Now enter their coefficients separated by a single space\n";
                cout << "If you are using complex numbers use the (a, b) format, when:\n";
                cout << "x = a + bi\n";
                cout << "Polynomio 1:";
                cout << endl;

                switch(tp_op){
                    case 1:
                        a_i.n = read_v(a_i.A, n);
                        break;
                    case 2:
                        a_ld.n = read_v(a_ld.A, n);
                        break;
                    case 3:
                        a_cd.n = read_v(a_cd.A, n);
                        break;
                }

                
                cout << "Polynomio 2:";
                cout << endl;

                switch(tp_op){
                    case 1:
                        b_i.n = read_v(b_i.A, n);
                        break;
                    case 2:
                        b_ld.n = read_v(b_ld.A, n);
                        break;
                    case 3:
                        b_cd.n = read_v(b_cd.A, n);
                        break;
                }

                cout << "The result is:\n";

                switch(tp_op){
                    case 1:
                        c_i = a_i * b_i;
                        c_i.printPoly();
                        break;
                    case 2:
                        c_ld = a_ld * b_ld;
                        c_ld.printPoly();
                        break;
                    case 3:
                        c_cd = a_cd * b_cd;
                        c_cd.printPoly();
                        break;
                }

                break;
            case 2:
                read_one(tp_op, true);

                cout << "The result is:\n";
                switch(tp_op){
                    case 1:
                        c_cd.A = a_i.FFT();
                        break;
                    case 2:
                        c_cd.A = a_ld.FFT();
                        break;
                    case 3:
                        c_cd.A = a_cd.FFT();
                        break;
                }
                c_cd.n = c_cd.A.size();
                c_cd.printPoly();
                break;
            case 3:
                read_one(tp_op, false);

                cout << "The result is:\n";
                switch(tp_op){
                    case 1:
                        c_cd.A = a_i.IFFT();
                        break;
                    case 2:
                        c_cd.A = a_ld.IFFT();
                        break;
                    case 3:
                        c_cd.A = a_cd.IFFT();
                        break;
                }
                c_cd.n = c_i.A.size();
                c_cd.printPoly();
                break;
            case 4:
                read_pair(tp_op, false);

                cout << "The result is:\n";

                switch(tp_op){
                    case 1:
                        c_i = IFT(ii);
                        c_i.printPoly();
                        break;
                    case 2:
                        c_ld = IFT(dd);
                        c_ld.printPoly();
                        break;
                }
                break;
            case 5:
                read_one(tp_op, true);

                cout << "The result is:\n";

                switch(tp_op){
                    case 1:
                        c_i = a_i.deriv();
                        c_i.printPoly();
                        break;
                    case 2:
                        c_ld = a_ld.deriv();
                        c_ld.printPoly();
                        break;
                }
                
                break;
            case 6:
                read_one(tp_op, true);

                cout << "The result is:\n";

                switch(tp_op){
                    case 1:
                        c_i = a_i.integr();
                        c_i.printPoly();
                        break;
                    case 2:
                        c_ld = a_ld.integr();
                        c_ld.printPoly();
                        break;
                }
                break;
            case 7:
                cin >> s >> t;
                vector<cd> a,b;

                for(int i = 0; i < (int)s.size(); i++){
                    a.push_back(translate(s[i] - 'a', s.size())); 
                }

                for(int i = (int)t.size()-1; i >= 0 ; i--){
                    b.push_back(translateP(t[i] - 'a', t.size())); 
                }
                a_cd = Polynomio<cd>(a);
                b_cd = Polynomio<cd>(b);

                c_cd = a_cd * b_cd;
                
                vector<int> ans;

                for(int i = 0; i < (int)c_cd.A.size(); i++){
                    if((int)t.size() == real(c_cd.A[i]))
                        ans.push_back(i);
                }

                cout << ans.size() << endl;

                for(int x : ans){
                    cout << x << ' ';
                }
                cout << endl;
                break;
        }
        cin >> tp_op; 
        system("clear");
    }while(opt != 0);

    getch();


    
/*    vector<int> a(5);*/
    /*vector<int> b(5);*/

    /*for(int i = 0; i < 5; i++){*/
        /*a[i] = i + 1;*/
        /*b[4 - i] = i + 1;*/
    /*}*/
    /*Polynomio<int> p1(a), p2(b);*/

    /*Polynomio<int> p3 = p1 * p2;*/


    /*p3.printPoly();*/
    vector<pair<long double, long double> > points{make_pair(5.0, 2.0), make_pair(2.0, 1.0), make_pair(0.0, 4.0), make_pair(-4.0, 0.0), make_pair(-9.0, 7.0)};
    Polynomio<long double> p = IFT(points);
    
    p.printPoly();

  /*  int n;*/

    /*cin >> n;*/

/*    for(int i = 0; i < n; i++){*/
        /*cin >> x;*/
        /*a.push_back(x);*/
    /*}*/

    /*for(cd x : a)*/
        /*cout << x << ' ';*/
    /*cout << '\n';*/

    /*//fft(a, 0);*/

    /*for(cd x : a)*/
        /*cout << x << ' ';*/
    /*cout << endl;*/

    return 0;
}
