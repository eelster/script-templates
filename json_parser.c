#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define MAX_STR 2048


struct entry_s {
    char *key;
    char *value;
    struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;


/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

    hashtable_t *hashtable = NULL;
    int i;

    if( size < 1 ) return NULL;

    /* Allocate the table itself. */
    if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
        return NULL;
    }

    /* Allocate pointers to the head nodes. */
    if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
        return NULL;
    }
    for( i = 0; i < size; i++ ) {
        hashtable->table[i] = NULL;
    }

    hashtable->size = size;

    return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

    unsigned long int hashval;
    int i = 0;

    /* Convert our string to an integer */
    while( hashval < ULONG_MAX && i < strlen( key ) ) {
        hashval = hashval << 8;
        hashval += key[ i ];
        i++;
    }

    return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, char *value ) {
    entry_t *newpair;

    if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
        return NULL;
    }

    if( ( newpair->key = strdup( key ) ) == NULL ) {
        return NULL;
    }

    if( ( newpair->value = strdup( value ) ) == NULL ) {
        return NULL;
    }

    newpair->next = NULL;

    return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, char *value ) {
    int bin = 0;
    entry_t *newpair = NULL;
    entry_t *next = NULL;
    entry_t *last = NULL;

    bin = ht_hash( hashtable, key );

    next = hashtable->table[ bin ];

    while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
        last = next;
        next = next->next;
    }

    /* There's already a pair.  Let's replace that string. */
    if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

        free( next->value );
        next->value = strdup( value );

        /* Nope, could't find it.  Time to grow a pair. */
    } else {
        newpair = ht_newpair( key, value );

        /* We're at the start of the linked list in this bin. */
        if( next == hashtable->table[ bin ] ) {
            newpair->next = next;
            hashtable->table[ bin ] = newpair;
            
            /* We're at the end of the linked list in this bin. */
        } else if ( next == NULL ) {
            last->next = newpair;
            
            /* We're in the middle of the list. */
        } else  {
            newpair->next = next;
            last->next = newpair;
        }
    }
}

/* Retrieve a key-value pair from a hash table. */
char *ht_get( hashtable_t *hashtable, char *key ) {
    int bin = 0;
    entry_t *pair;

    bin = ht_hash( hashtable, key );

    /* Step through the bin, looking for our value. */
    pair = hashtable->table[ bin ];
    while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
        return NULL;

    } else {
        return pair->value;
    }
}


void ht_print(hashtable_t *hashtable) {
    entry_t *pair;
    int i;

    for (i=0; i < hashtable->size; i++) {
        pair = hashtable->table[i];
        while (pair != NULL) {
            if (pair->key != NULL) {
                printf("Key == %s ", pair->key);
            }
            if (pair->value != NULL) {
                printf("Value == %s\n", pair->value);
            }
            pair = pair->next;
        }
    }
    
}


hashtable_t* parse_json_hashtable(FILE* fp) {
    hashtable_t *hashtable = ht_create( 65536 );
    int char_read;
    char buff[MAX_STR];
    char last_key[MAX_STR];
    char last_val[MAX_STR];
    int in_string = 0;
    int i;
    int reading_key = 1;

    while (char_read = fgetc(fp)) {

        if (in_string > 0) {
            if (char_read == '"') {
                in_string = 0;
                // printf("Got key %s\n", buff);
            } else {
                buff[in_string-1] = char_read;
                in_string++;

                if (reading_key) {
                    for (i=0;i<MAX_STR;i++) {
                        last_key[i] = buff[i];
                    }
                } else {
                    for (i=0;i<MAX_STR;i++) {
                        last_val[i] = buff[i];
                    }
                    ht_set(hashtable, last_key, last_val);
                }
            }
        } else {

            if (char_read == '{') {
                ht_print(parse_json_hashtable(fp));
            } else if (char_read == '}') {
                return hashtable;
            } else if (char_read == '"') {
                in_string = 1;
                for (i=0;i<MAX_STR;i++) { 
                    buff[i] = 0;
                }
            } else if (char_read == ':') {
                reading_key = 0;
            } else if (char_read == ',') {
                reading_key = 1;
            } else {
                printf("ERROR! unhandled char %c\n", char_read);
            }
        }
    }

    return hashtable;
}



hashtable_t* parse_json(FILE* fp) {
    hashtable_t *hashtable = ht_create( 65536 );
    int char_read;
    char buff[MAX_STR];
    char last_key[MAX_STR];
    char last_val[MAX_STR];
    int in_string = 0;
    int reading_key = 1;
    int i;

    while (char_read = fgetc(fp)) {

        if (in_string > 0) {
            if (char_read == '"') {
                in_string = 0;
                printf("Got key %s\n", buff);
            } else {
                buff[in_string-1] = char_read;
                in_string++;

                if (reading_key) {
                    for (i=0;i<MAX_STR;i++) {
                        last_key[i] = buff[i];
                    }
                } else {
                    for (i=0;i<MAX_STR;i++) {
                        last_val[i] = buff[i];
                    }
                    ht_set(hashtable, last_key, last_val);
                }
            }
        } else {

            if (char_read == '{') {
                ht_print(parse_json_hashtable(fp));
            } else if (char_read == '}') {
                printf("ERROR! missing opening bracket\n");
                return hashtable;
            } else if (char_read == '"') {
                in_string = 1;
                for (i=0;i<MAX_STR;i++) { 
                    buff[i] = 0;
                }
            } else if (char_read == ':') {
                reading_key = 0;
            } else if (char_read == ',') {
                reading_key = 1;
            } else {
                printf("ERROR! unhandled char %c\n", char_read);
            }
        }
    }

    return hashtable;
}


int main(int argc, int **argv) {
    FILE *jsonfp;
    hashtable_t *hashtable = NULL;

    jsonfp = fopen("pmv_pmsb_log.json", "r");

    if (jsonfp == NULL) {
        printf("Cannot open file\n");
        return(1);
    }

    hashtable = parse_json(jsonfp);

    fclose(jsonfp);
    return(0);
}
