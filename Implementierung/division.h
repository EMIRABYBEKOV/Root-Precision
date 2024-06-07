#include <stdio.h>
#include "kmul.h"


/**
 * @brief This function compares two bignums
 * @param a bignum
 * @param b bignum
 * @return 1 -> if b is greater than or equal to a, esle 0
 */
char compare(struct bignum* a, struct bignum* b){
    //compare length
    if(b->len > a->len){
        return 1;
    }else if(b->len < a->len){
        return 0;
    }else{
        //if a and b have same length, iterate and compare 
        for(int i = 0; i < b->len; i++){
            if(b->arr[i] > a->arr[i]){
                //b is greater then a
                return 1;
            }else if(b->arr[i] < a->arr[i]){
                //b is samller then a
                return 0;
            }
        }
        //b is equal to a
        return 1;
    }
}


/**
 * @brief This function makes logical shift to the left of entire bignum by one bit
 * @param num bignum
 * @return shifted to the left bignum
 */
struct bignum* shift_left(struct bignum* num){
    //if bignum is empty
    if(num->len == 0){
        return num;
    }
    //check if bignum needs extra length
    int newlen = num->len;
    if(num->arr[0] > 127){
        newlen += 1;
    }
    //create new array and add 1 to the start if required
    unsigned char* res_arr = (unsigned char*)malloc(newlen * sizeof(unsigned char));
    int index = 0;
    if(newlen > num->len){
        res_arr[index] = 1;
        index += 1;
    }
    //iterate over the binary representation of the number
    for(int i = index; i < newlen; i++){
        //make left shift for every char by 1 bit
        res_arr[i] = (unsigned char)(num->arr[i - index] * 2);
        //if the right number in binary has in the first place 1, then it is transferred to the left
        if(num->arr[i - index + 1] > 127 && ((i - index + 1) < num->len)){
            res_arr[i] = res_arr[i] + 1;
        }
    }
    //collect the result in a bignum and return it
    struct bignum* res = (struct bignum*)malloc(sizeof(struct bignum));
    res->len = newlen;
    res->arr = res_arr;
    return res;
}


/**
 * @brief This function calculates the length of a unsigned char in binary form
 * @param number unsigned char
 * @return int value
 */
int length_in_binary(unsigned char number) {

    if (number == 0) {
        return 1;
    }
    int length = 0;
    //iterate over binary unsigned char and calculate
    while (number != 0) {
        number >>= 1;
        length++;
    }
    return length;
}

/**
 * @brief This function implements the method of long division of two bignums
 * @param a dividend(bignum)
 * @param b divisor(bignum)
 * @param n length of the required numbers after the decimal point in binary form
 * @details JUST FOR b > a (only for numbers that calculate the square root of two)
 * @return bignum that contains remainder in binary fixed point number form
 */
struct bignum* divide(struct bignum* a, struct bignum* b, size_t n)
{        

        //bring numbers to exact length
        a = reduce(a);
        b = reduce(b);
        //create bignum for result with initial values
        unsigned char* res_arr = (unsigned char*)malloc(1 * sizeof(unsigned char));
        res_arr[0] = 0;
        struct bignum* result =(struct bignum*) malloc(sizeof(struct bignum));
        result->len = 1;
        result->arr = res_arr;
        //variable with which we will not go beyond n
        size_t index = 0;
        //since we have the length in bytes, we consider the initial length of the first bytes, 
        int old_l_a = length_in_binary(a->arr[0]) + (a->len - 1) * 8;
        int old_l_b = length_in_binary(b->arr[0]) + (b->len - 1) * 8;
        //consider how much we need to shift the dividend in order to align with the divisor
        for(int i = 0; i < (old_l_b - old_l_a) && (index < n); i++){
            a = shift_left(a);
            index += 1;
        }
        //now length of dividend is equal to divisor
        a->len = b->len;
        //loop the long division algorithm as much as needed
        while(index <= n){
            //add 0 to ramainder by shifting
            result = shift_left(result);
            //if b is greater than a then add 1 to remainder 
            if(compare(b, a)){
                result->arr[result->len - 1] = result->arr[result->len - 1] | (unsigned char)1;
                //substract b from a
                a = sub(a, b);
            }
            //shift the divident to the left
            a = shift_left(a);
            //calculate remainder
            index += 1;
        }

        return result;
}

/**
 * @brief This function shifts the bignum left by n bytes
 * @param a bignum
 * @param n by how many bytes should the number be shifted
 * @return shifted to the left bignum
 * @note there bignum is an implementation of decimal numbers after decimal point where each char is a number between 0 and 9
 */
struct bignum* shift_left_decimals(struct bignum* a, size_t n){
    //create new array with new length of unsigned chars
    unsigned char* res_arr = (unsigned char*)malloc((a->len + n) * sizeof(unsigned char));
    //copy to new array
    for(int i = 0; i < a->len; i++){
        res_arr[i] = a->arr[i];
    }
    //fill the rest with zeros
    for(size_t i = a->len; i < a->len + n; i++){
        res_arr[i] = 0;
    }
    //collect the result in a bignum and return it
    struct bignum* res = (struct bignum*)malloc(sizeof(struct bignum));
    res->len = a->len + n;
    res->arr = res_arr;
    return res;
}

/**
 * @brief This function shifts the bignum right by n bytes
 * @param a bignum
 * @param n by how many bytes should the number be shifted
 * @return shifted to the right bignum
 * @note there bignum is an implementation of decimal numbers after decimal point where each char is a number between 0 and 9
 */
struct bignum* shift_right_decimals(struct bignum* a, size_t n){
    //create new array with new length of unsigned chars
    unsigned char* res_arr = (unsigned char*)malloc((a->len + n) * sizeof(unsigned char));
    //fill with zeros
    for(size_t i = 0; i < n; i++){
        res_arr[i] = 0;
    }
    //fill the rest with bignums array
    for(size_t i = n; i < a->len + n; i++){
        res_arr[i] = a->arr[i - n];
    }
    //collect the result in a bignum and return it
    struct bignum* res = (struct bignum*)malloc(sizeof(struct bignum));
    res->len = a->len + n;
    res->arr = res_arr;
    return res;
}


/**
 * @brief This function multiplies the number stored in the bignum by 5
 * @param num bignum
 * @return multiplied bignum
 * @note there bignum is an implementation of decimal numbers after decimal point where each char is a number between 0 and 9
 */
struct bignum* multiply_decimal_5(struct bignum* num){
    //create new array for result
    unsigned char* prom_arr =(unsigned char*)malloc(num->len * sizeof(unsigned char));
    //always count overflow
    int overflow = 0;
    //Iterate over the decimal representation of the number, multiplying each digit by one while considering overflow
    for(int i = num->len - 1; i >= 0; i--){
        int prom = num->arr[i] * 5 + overflow;
        if(prom > 9){
            overflow = prom / 10;
            prom_arr[i] = prom - overflow * 10;
        }else{
            overflow = 0;
            prom_arr[i] = prom;
        }
    }
    //collect the result in a bignum and return it
    struct bignum* res = (struct bignum*)malloc(sizeof(struct bignum));
    //if number needs extra length in case overflow
    if(overflow){
        //create new array with extra length
        unsigned char* res_arr = (unsigned char*)malloc((num->len + 1) * sizeof(unsigned char));
        //copy multiplied array
        memcpy(res_arr + 1, prom_arr, num->len * sizeof(unsigned char));
        //free allocated memory
        free(prom_arr);
        //add overflow and return th e result
        res_arr[0] = overflow;
        res->arr = res_arr;
        res->len = num->len + 1;
        return res;
    }
    res->arr = prom_arr;
    res->len = num->len;
    return res;
}

/**
 * @brief This function adds two numbers stored in the bignum
 * @param a bignum
 * @param b bignum
 * @return result after addition 
 * @note there bignum is an implementation of decimal numbers after decimal point where each char is a number between 0 and 9
 */
struct bignum* add_decimals(struct bignum* a, struct bignum* b){
    //create new array for result
    unsigned char* prom_arr =(unsigned char*)malloc(a->len * sizeof(unsigned char));
    //always count overflow
    int overflow = 0;
    //add every digits char by char while considering overflow
    for(int i = a->len - 1; i >= 0; i--){
        int prom = a->arr[i] + b->arr[i] + overflow;
        if(prom > 9){
            overflow = 1;
            prom_arr[i] = prom - 10;
        }else{
            overflow = 0;
            prom_arr[i] = prom;
        }
    }
    //collect the result in a bignum and return it
    struct bignum* res = (struct bignum*)malloc(sizeof(struct bignum));
    //if number needs extra length in case overflow
    if(overflow){
        //create new array with extra length
        unsigned char* res_arr = (unsigned char*)malloc((a->len + 1) * sizeof(unsigned char));
        //copy array after addition
        memcpy(res_arr + 1, prom_arr, a->len * sizeof(unsigned char));
        //free allocated memory
        free(prom_arr);
        res_arr[0] = overflow;
        res->arr = res_arr;
        res->len = a->len + 1;
        return res;
    }
    res->arr = prom_arr;
    res->len = a->len;
    return res;

}

/**
 * @brief This function converts and print the given bignum, which represents numbers after the decimal point in binary fixpoint form, 
 * into either decimal, binary or hexadecimal format
 * @param num bignum with numbers after decimal point in binary fixpoint form
 * @param b choose output: 1 -> binary, 2 -> hex, 3 -> decimal
 * @param n exact lentgth of bignum in binary representation (bignum->len is length in bytes)
 * @note to convert to decimal, the function creates a bignum where each char will represent a decimal number from 0 to 9
 */
void print_bignum(struct bignum* num, int b, size_t n){
    //print 1, because the integer part of the square root of two is always equal to 1, in every representation
    printf("1."); 
    //print in binary
    if(b == 1){
        printf("0");
        if(n != 1){
            //check because bignums length is not exact(is in bytes)
            int l = length_in_binary(num->arr[0]);
            //print first bits of first char
            for (int j = l - 1; j >= 0; j--) {
                printf("%d", (num->arr[0] >> j) & 1);
            }
            //iterate over rest numbers and print bit by bit
            for (int i = 1; i < num->len; i++) {
                for (int j = 7; j >= 0; j--) {
                    printf("%d", (num->arr[i] >> j) & 1);
                }
            }
        }
    //print in hexadecimal
    }else if(b == 2){
        //if requiered length if first bits are not aligned
        int index = 0;
        if((n * 4) % 8 == 4){
            //print just half of the byte
            printf("%X", num->arr[index] & 0x0F);
            index += 1;
        }
        //iterate over aligned numbers and print in hexadecimal
        for(int i = index; i < num->len; i++){
                printf("%02X", num->arr[i]);
        }
    //print in decimal
    //in the case in decimal notation, the function converts from binary to decimal
    //and needs to store the decimal number, which will be stored as described in the functions @note above
    }else{
        if(n != 1){
            //create initial bignum for storing the decimal number
            unsigned char* arr = (unsigned char*)malloc(1 * sizeof(unsigned char));
            arr[0] = 0;
            struct bignum* res = (struct bignum*)malloc(sizeof(struct bignum));
            res->len = 1;
            res->arr = arr;
            //create a helper bignum that will represent each binary number in decimal point form
            struct bignum* help_v = (struct bignum*)malloc(sizeof(struct bignum));
            help_v->len = 1;
            unsigned char* h_arr = (unsigned char*)malloc(1 * sizeof(unsigned char));
            //the initial state is 0.5 beacuse in binary it is 0.1
            h_arr[0] = 5;
            help_v->arr = h_arr;
            //check because bignums length is not exact(is in bytes)
            int l = length_in_binary(num->arr[0]);
            //iterate over first char of binary represenattion
            for (int j = l; j >= 0; j--) {
                unsigned char cur = (num->arr[0] >> j) & (unsigned char)1;
                //if bit is 1
                if(cur){
                    //add to the result number after decimal point in the decimal format
                    res = shift_left_decimals(add_decimals(res, help_v), 1);
                }else{
                    //if bit is 0 the just continue
                    res = shift_left_decimals(res, 1);
                }
                //verify the correctness in the form of a number after the decimal point.
                int len = help_v->len;
                //multiply by 5
                help_v = multiply_decimal_5(help_v);
                //if needs zeros at the begining
                if(help_v->len == len){
                    help_v = shift_right_decimals(help_v, 1);
                }
            }
            //iterate over rest chars of binary represenattion
            for(int i = 1; i < num->len; i++){
                for (int j = 7; j >= 0; j--) {
                    unsigned char cur = (num->arr[i] >> j) & (unsigned char)1;
                    //if bit is 1
                    if(cur){
                        //add to the result number after decimal point in the decimal format
                        res = shift_left_decimals(add_decimals(res, help_v), 1);
                    }else{
                        res = shift_left_decimals(res, 1);
                    }
                    //verify the correctness the form of a number after the decimal point.
                    int len = help_v->len;
                    //multiply by 5
                    help_v = multiply_decimal_5(help_v);
                    //if needs zeros at the begining
                    if(help_v->len == len){
                        help_v = shift_right_decimals(help_v, 1);
                    }  
                }
            }
            //iterate over result, that is decimal represintation of a number after decimal point
            for(int i = 0; i < n; i++){
                printf("%i", res->arr[i]);
            }
        }else{
            printf("4");
        }
    }
}
