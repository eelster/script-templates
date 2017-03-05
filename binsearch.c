#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define N 100


int get_entry_index(int *array, int arraysize, int valtoseek) {
    int low, high, ind;
    int i,j;

    // basic debug info
    printf("Input array: ");
    for (i=0;i<arraysize;i++) {
        printf (" %d", array[i]);
    }
    printf("\n");
    printf("Called with %d, %d, %d\n", array[0], arraysize, valtoseek);


    // low = 0
    // high = max-1

    // need to check at beginning - to see if we hit a corner case, otherwise might miss it
    // if array[low]==val -> return low
    // if array[high]==val -> return high

    // while (high-low>1)
    //   candidate_index = (low+high)/2
    //   if array[candidate_index]==val -> return candidate_index
    //   else if array[candidate_index] > val -> high = candidate_index
    //   else if array[candidate_index] < val -> low = candidate_index
    // 
    // not found -> return -1 


    low = 0;
    high = arraysize-1;

    if (array[low] == valtoseek) { return low; }
    if (array[high] == valtoseek) { return high; }

    while (high-low>1) {
        assert(high>=low);
        ind = (low+high)/2;

        if (array[ind] == valtoseek) { 
            return ind; 
        } else if (array[ind] > valtoseek) {
            high = ind;
        } else { 
            low = ind;
        }
    }
    // if not found - return -1
    // note we only got here if the candidate never matched - and low/high are always prior candidates so they were checked too
    return -1;
}

int main(int argc, int **argv) {

    int array[N];
    int i,j,loc,valtoseek;

    for (i=1;i<N;i++) {
        for (j=0;j<=i;j++) {
            array[j] = rand() % 100;
            if (j>0) {
                // make sure we have an incrementing array
                array[j] += array[j-1];
            }
        }
        
        valtoseek = rand() % (100*i);
        loc = get_entry_index(array, i, valtoseek);
        printf("Got %d\n", loc);
        assert((loc == -1) || (array[loc] == valtoseek));

        valtoseek = array[rand() % i];
        loc = get_entry_index(array, i, valtoseek);
        printf("Got %d\n", loc);
        assert(array[loc] == valtoseek);
    }
    
    return 0;
} 
