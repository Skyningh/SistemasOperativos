#include <iostream>
using namespace std;

float suma(float n1,float n2){
    return n1+n2;
}

float resta(float n1,float n2){
    return n1-n2;
}

float multiplicacion(float n1,float n2){
    return n1*n2;
}

float division(float n1,float n2){
    if(n2 == 0){
        return -1;
    } else{
        return n1/n2;
    }
}


int main(){
    int operacion = 1;
    float n1 = 1.0;
    float n2 = 2.0;
    switch(operacion){
        case(1): {
            return suma(n1,n2);
            break;
        }
        case(2): {
            return resta(n1,n2);
            break;
        }
        case(3): {
            return multiplicacion(n1,n2);
            break;
        }
        case(4): {
            return division(n1,n2);
            break;
        }
        default: {
            cerr << "La operacion no existe" << endl;
            return EXIT_FAILURE;
        }

    }
    return 0;
}