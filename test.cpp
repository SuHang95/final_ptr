#include"su_final_ptr.h"
using namespace su;

int main(){
    final_ptr<char> a;
    final_ptr<char> b(new char('c'));
    final_ptr<char> c(new char('t'));
    a=b;
    a=c;
    return 0;
}
