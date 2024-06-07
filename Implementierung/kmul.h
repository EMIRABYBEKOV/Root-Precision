//Implementierung der Karazuba-Multiplikation
#include "type.h"
#include <stdlib.h>
#include <string.h>
#include <byteswap.h>

//Hilfsfunktion
void extend(struct bignum* x, struct bignum* y){
//erweitert eines der bignums, sodass beide danach gleiche länge haben
if(x->len == y->len){
    return;
}
int n = 0;
if(x->len < y->len){
    n = y->len - x->len;
    unsigned char* arr = (unsigned char*)calloc(y->len, sizeof(char));
    for (int i = 0; i < x->len; i++) {
        arr[i+n] = x->arr[i];
    }   
    x->len = y->len;
    x->arr = arr;
}
else{
    n = x->len - y->len;
    unsigned char* arr = (unsigned char*)calloc(x->len, sizeof(char));
    for (int i = 0; i < y->len; i++) {
        arr[i+n] = y->arr[i];
    }   
    y->len = x->len;
    y->arr = arr;
}
}

//Hilfsfunktion
struct bignum* add(struct bignum* x, struct bignum* y){
 
    //Bringe x und y auf gleiche Länge
    extend(x,y);
    //berechne die neue Länge (x/y .len +1 wegen evntl. Overflow)
    int newLen = y->len +1;
    //Overflow(startet immer bei 0)
    int o = 0;
    //erstelle Zielarray mit nur Nullen
    unsigned char* arr = (unsigned char*)calloc(newLen, sizeof(char));

    //addiere x+y char by char und bestimme o
    //Schleife bis x.len und eins weiter(wegen Overflow)weil rest bleibt gleich
    int i = 0;
    while(i < y->len || i< x->len){
        if(i >= y->len){
            arr[newLen -1 -i] = x->arr[x->len-1 -i] + o;
        }
        else if(i >= x->len){
            arr[newLen -1 -i] = y->arr[y->len-1-i] + o;
        }
        else{
            arr[newLen -1 -i] = y->arr[y->len-1-i] + x->arr[x->len-1 -i] + o;
        }
       
        // falls das gilt und nur dann besteht ein Overflow
        if(o == 0){
            if(arr[newLen-1-i] < y->arr[y->len-1-i]){
                o = 1;
            }
            else{
                o = 0;
            }
        }
        else{
            if((arr[newLen-1-i] <= y->arr[y->len-1-i] && x->arr[x->len-1-i] != 0)|| y->arr[y->len-1-i] + x->arr[x->len-1-i] == 255){
                o = 1;
            }
            else{
                o = 0;
            }
        }
        i++;
    }
        if( x->arr[0] == 255 && y->arr[0] == 235){
   }
    // setze eventl. Overflow
    arr[0] = o;
    //erstelle das neue bignum
    struct bignum* sum = malloc(sizeof(struct bignum));
    sum->len = newLen;
    sum -> arr = arr;
    return sum;
}


//Hilfsfunktion
struct bignum* sub(struct bignum* x, struct bignum* y){
    //y ist bei jeder verwendung kleiner x
    extend(x,y);
    //Da y.len > x.len(vorausgesetzt bei verwendung)
    int newLen = x->len;
    //Overflow(startet immer bei 0)
    int o = 0;
    //erstelle Zielarray mit nur Nullen
    unsigned char* arr = (unsigned char*)calloc(newLen, sizeof(char));
    //subtrahiere x-y-o char by char und bestimme o
    //Schleife bis y.len, da y.len < x.len und y < x 
    int i = 0;
    while(i < y->len){
        arr[newLen -1 -i] = x->arr[x->len-1-i] - y->arr[y->len-1 -i] - o;
        //setze o wieder auf 0
        // falls das gilt und nur dann besteht ein Overflow
        if((arr[newLen-1-i] > x->arr[x->len-1-i] - o ||(x->arr[x->len-1-i] -o == 0xff && o == 1 ))){
            o = 1;
        }
        else{
            o = 0;
        }
        i++;
    }
    // setze eventl. Overflow
    if(o == 1){
    arr[newLen -1 -i] = x->arr[newLen -1-i] - o;
    }
    //erstelle das neue bignum
    struct bignum* sum = malloc(sizeof(struct bignum));
    sum->len = newLen;
    sum -> arr = arr;
    return sum;
}
//Hilfsfunktion
struct bignum* shift(struct bignum* x, int m){
    //linksshift um m byte
    //muss len anpassen, int könnte nicht reichen
    int newLen = x->len +m;
    //neues array 
    unsigned char* arr = (unsigned char*)calloc(newLen, sizeof(char));
    for(int i = 0;i < x->len; i++){
        arr[i] = x->arr[i];
    }
    struct bignum* shift = malloc(sizeof(struct bignum));
    shift->len = newLen;
    shift -> arr = arr;
    return shift;
}
//Hilfsfunktion
struct bignum* reduce(struct bignum* x){
//löscht führende nuller
int n = x->len;
while (x->arr[x->len -n] == 0 && n > 1){
    n--;
}
struct bignum* new = malloc(sizeof(struct bignum));
    new->len = n;
    new -> arr = x->arr + x->len - n;
    return new;
}

//Ein naiver Ansatz einer Multiplikation ohne Karazuba
struct bignum* kmul_v1(struct bignum* x, struct bignum* y) {
    //kürze unnötige Null-Bytes
    y = reduce(y);
    //Erstelle das Ergebis-bignum
    struct bignum* new = malloc(sizeof(struct bignum));
    new->len = 1;
    unsigned char* arr = (unsigned char*)calloc(1, sizeof(char));
    new -> arr = arr;
    int len = y->len;
    int set;
    //Das aktuelle Bit des Bytes
    int bit = 0;
    int byte = y->arr[len-1];
    struct bignum* xtemp = malloc(sizeof(struct bignum));
        xtemp->len = x->len;
        xtemp -> arr = x->arr;
    //Gebe 0 zurück, falls y = 0
    if(len == 1 && byte == 0){
        free(xtemp);
         //Gebe das Ergebnis ohne führende Null-Byte zurück
        return reduce(new);
    }
    //Iteriere über die len Byte von y
    while(len > 0){
        //Bestimme niedrigstes Bit
        set = byte%2;
        //Shifte das aktuelle Byte um nächstes Bit zu betrachten
        byte = byte >> 1;
        if(set == 1){
            //Addiere xtemp auf das Ergebnis
            struct bignum* a = add(xtemp,new);
            a = reduce(a);
            new->len = a->len;
            new->arr = a->arr;
        }
        //Shifte xtemp um ein Bit nach links
        xtemp = add(xtemp,xtemp);
        xtemp = reduce(xtemp);
        bit++;
        //Falls über alle Bit iteriert wurde, nimm das nächset(falls vorhanden)
        if(bit == 8){
            bit = 0;
            len--;
            if(len != 0){
                byte = y->arr[len-1];
            }
        }
    }
free(xtemp);
//Gebe das Ergebnis ohne führende Null-Byte zurück
return reduce(new);
}



//Hauptfunktion(effizient)
struct bignum* kmul(struct bignum* x, struct bignum* y) {
    //keine negativen Zahlen oder len 0 erlaut
    //setzte Länge von x und y gleich
    x = reduce(x);
    y = reduce(y);
    extend(x,y);
    //len <= 64bit => result <= 128bit
    if(x->len <= 8){
        
        //schreibe die Zahlen in Arrays und bereite ein Zielarray vor
        //berechne das Ergebnis
        __uint128_t xtemp = *(x->arr);
        __uint128_t ytemp = *(y->arr);
        for(int i = 1;i < x->len;i++){
            xtemp = xtemp << 8;
            xtemp += *(x->arr+i);
            ytemp = ytemp << 8;
            ytemp += *(y->arr+i);
        }
        __uint128_t result = xtemp * ytemp;
        //tausche Byte-Reihenfolge wegen little Endian
        unsigned char* end = (unsigned char *)calloc(16, sizeof(char));
        for(int i = 0;i < 2*x->len;i++){
            end[15-i] += *(unsigned char*)&result;
            result = result >> 8;
        }
        //erstelle ein neues struct mit länge 16(128bit) und lösche unnötige Null-Bytes
        struct bignum* sum = malloc(sizeof(struct bignum));
        sum->len = 16;
        sum -> arr = (unsigned char*) end;
        return reduce(sum);
    }
    //Bestimme die Länge der x0,x1,y0 und y1
    int r = x->len%2;
    int len = x->len/2 +r;
    //Erstelle Arrays für x1 und y1, die eventuell ein führendes Null-Byte haben
    unsigned char* arrx = (unsigned char*)calloc(len, sizeof(char));
    unsigned char* arry = (unsigned char*)calloc(len, sizeof(char)); 
    for (int i = 0; i < len-r; i++) {
        arrx[i+r] = x->arr[i];
        arry[i+r] = y->arr[i];
    }
    //Konstruiere bignums
    struct bignum* x1 = malloc(sizeof(struct bignum));
    x1->len = len;
    x1->arr = arrx;
    struct bignum* y1 = malloc(sizeof(struct bignum));
    y1->len = len;
    y1->arr = arry;
    struct bignum* x0 = malloc(sizeof(struct bignum));
    x0->len = len;
    x0->arr = x->arr +len-r;
    struct bignum* y0 = malloc(sizeof(struct bignum));
    y0->len = len;
    y0->arr = y->arr +len-r;
    // berechne laut Formel die Summe mit Rekursion
    struct bignum* s1 = kmul(x0,y0);
    struct bignum* sum1 = add(x0,x1);
    struct bignum* sum2 = add(y0,y1);
    struct bignum* s2 = kmul(sum1,sum2);
    free(sum1);
    free(sum2);
    //free(sum1->arr);
    //free(sum2->arr);
    struct bignum* s3 = kmul(x1,y1);
    free(x1);
    free(y1);
    //Erstelle temporäre bignums um Speicher freigeben zu können
    struct bignum* temp1 = sub(s2 ,s1);
    struct bignum* temp2 = s2;
    s2 = sub(temp1,s3);
    free(temp2);
    //free(temp2->arr);
    free(temp1);
    //free(temp1->arr);
    temp1 = s2;
    s2 = shift(s2,len);
    free(temp1);
    //free(temp1->arr);
    temp1 = s3;
    s3 = shift(s3,2*len);
    free(temp1);
    //free(temp1->arr);
    temp1 = add(s1,s2);
    struct bignum* result = add(temp1,s3);
   
    free(temp1);
    //free(temp1->arr);
    return reduce(result);
}