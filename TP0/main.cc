/*****************************************************************************
 * TP0 - Algoritmos y Programación II                                        *
 * Alumnos:                                                                  *
 *          Kevin                                                            *
 *          Pablo                                                            *
 *          Juan                                                             *
 *          Potenza, Nicolás A.     - #97024                                 *
 *                                                                           *
 *****************************************************************************/

/***** Bibliotecas Estándar **************************************************/
#include <string>
#include <cstdlib>
#include <iostream>
#include <cmath>

/***** Bibliotecas Propias ***************************************************/
#include "cmdline.h"
#include "bignum.h"

/***** Elementos Globales ****************************************************/
using namespace std;

/***** Funcion para pruebas(BORRAR)****************************************************/
void print_bignum(const bignum &bn){

    if (bn.sign == true){
        cout << '-';
    }
    
    for (size_t i = 0; i < bn.size; i++){
        cout << bn.digits[i];
    }

    cout << endl;
    
}


/***** Función main **********************************************************/
int main(int argc, char const *argv[]){
    std::string;
    string a = "-145";
    string b = "323";

    bignum x(a),y(b),z;
    

    print_bignum(x);
    print_bignum(y);
    print_bignum(z);

    z = x;
    
    print_bignum(z);

	return 0;
}