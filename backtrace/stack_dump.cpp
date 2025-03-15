#include <iostream>
#include <execinfo.h>

void stack_dump(int depth, std::ostream& stream) {
    void* buffer[depth + 1];  
    int number_of_ptr = backtrace(buffer, depth + 1);  

    char** symbols = backtrace_symbols(buffer, number_of_ptr); 
    if(symbols == NULL){
        stream << ("error in backtrace symbols");
        return;
    }
    
    for (int i = 1; i < number_of_ptr && i <= depth; ++i) {  
        stream << i << ": " << symbols[i] << std::endl;  
    }

    free(symbols);  
}

void function1() {
    stack_dump(6, std::cout);
} 

void function2() {
    function1();
}

void function3() {
    function2();
}

int main() {
    function3();  
    return 0;
}
