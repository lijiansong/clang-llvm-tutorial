

int main(void) {
    int *i;
    int j;
    int *k;

    if (i != &j) {
        k = i + 4;
    }
    else {
        k = i + 8;
    }
    
    
    //int j = 0;
/*
    i = &j;
    *i = j;
    */
/*
int * j = &i;
    int * k;

    j = k;
*/
    return *i;

}
