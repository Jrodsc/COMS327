#include <iostream>

using namespace std;

class exception327 : public std::exception{
    virtual const char *what() const noexcept {
        return "exception327, fool!\n";
    }

template <class T>
T max327(T t1, T t2){
    return t1 > t2 ? t1 : t2;
}

template <class T>
class stack{
    private:
        class stack_node{
            public:
                T data;
                stack_node * next;

                stack_node(T t) : data(t), next(0) {}
        };
        stack_node * head;        
        int stack_size;
    public:
        stack() : head(0), stack_size(0) {}
        ~stack(){
            stack_node * tmp;

            while((tmp = head)){
                head = head -> next;
                delete tmp;
            }
        }
        void push(const T &t){
            stack_node * tmp = new stack_node(t);
            tmp -> next = head;
            head = tmp;
            stack_size++;
        }

        //Keyword "throw()": Funciton does not throw an exception
        //Keyword "throw(type)": Functions throws an exception of type "type"
        //(Beginning C++11) Keyword "noexcept": Functions does not throw
        //
        //Beginning C++17: throw() is no longer part of the language
        //                 noexcept(false) -> function throws an exception
        T pop() noexcept(false) {
            if(!head)
                throw exception327();
            T t = head -> data;
            stack_node * tmp = head;
            head = head -> next;
            
            delete tmp;

            stack_size--;
            return t;
        }
        T top() noexcept(false) {
            if(!head)
                throw "Attempt to access an empty stack";
            else
                return head -> data;
        }
        int size() noexcept {
            return stack_size;
        }
        bool is_empty() noexcept {
            return !stack_size;
        }

};

int main(int argc, char *argv[]){
    /*string s,t;

    cout << max327(3, 6) << endl;
    cout << max327(9, 6) << endl;
    
    s = "Hello";
    t = "Hi";

    cout << max327(s, t) << endl;
    cout << max327("foo", "bar") << endl;
    */

    stack<int> si;
    int i;

    for(i = 0; i < 100; i++)
        si.push(i);

    while(!si.is_empty()){
        try{
            cout << si.pop() << endl; 
        }
        catch(const char * s){
            cerr << s << endl;
        }
    }

    stack<string> ss;

    ss.push("u");
    ss.push("o");
    ss.push("i");
    ss.push("e");
    ss.push("a");
    
    while(!ss.is_empty()){
        try{
            cout << ss.pop() << endl; 
        }
        catch (int i){
        }
        //.
        //.
        //.
        catch(exception e){
            throw;
        }
        catch(const char * s){
            cerr << s << endl;
        }
        /*
         * catch(...){
         * }
         */
    }
    return 0;
}
