#include <stdio.h>
const int N = 5;

typedef struct {
    char data[4096];
    int key;
} item;

item array[] = {
    {"bill", 3},
    {"neil", 4},
    {"john", 2},
    {"rick", 5},
    {"alex", 1},
};

void mostrar_array(item *a, int n){
    
    for(int i = 0; i < n; i++)
        printf("array[%d] = {%s, %d}\n", i, array[i].data, array[i].key);


}
void sort(item *a, int n) {
    int i = 0, j = 0;
    int s = 1;
    item* p = NULL; // NULL == (void *)

    for(; (i < n) & ( s != 0 ) ; i++) {
        s = 0;
        p = a;
        j = n-1;
        do {
            //printf("%d  %d  ",p->key , (p+1)->key);
            if( p->key > (p+1)->key) {
                item t = *p;
                p  = (p+1);
                *(p+1) = t;
                s ++;
                
            }
        } while ( --j >= 0 );
    }

}


int main() {

    mostrar_array(array, N);
    sort(array,5);
    mostrar_array(array, N);
    

    return 0;
}
