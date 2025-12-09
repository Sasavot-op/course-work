#include "util.h"


size_t my_strlen(const char *s){
    size_t i = 0;
    if(!s) return 0;
    while(s[i]) i++;
    return i;
}

char *my_strdup(const char *s){
    if(!s) return NULL;
    size_t n = my_strlen(s);
    char *p = malloc(n+1);
    if(!p) return NULL;
    for(size_t i=0;i<=n;i++) p[i]=s[i];
    return p;
}

char *my_strcpy_alloc(const char *s){
    return my_strdup(s);
}

int my_strcmp(const char *a, const char *b){
    if(a==NULL && b==NULL) return 0;
    if(a==NULL) return -1;
    if(b==NULL) return 1;
    size_t i=0;
    while(a[i] && b[i]){
        if((unsigned char)a[i] < (unsigned char)b[i]) return -1;
        if((unsigned char)a[i] > (unsigned char)b[i]) return 1;
        i++;
    }
    if(a[i]) return 1;
    if(b[i]) return -1;
    return 0;
}

int my_strncmp(const char *a, const char *b, size_t n){
    if(n==0) return 0;
    size_t i=0;
    while(i<n && a[i] && b[i]){
        if((unsigned char)a[i] < (unsigned char)b[i]) return -1;
        if((unsigned char)a[i] > (unsigned char)b[i]) return 1;
        i++;
    }
    if(i==n) return 0;
    if(a[i]) return 1;
    if(b[i]) return -1;
    return 0;
}

char *int_to_str(int v){
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%d", v);
    if(n < 0) return NULL;
    return my_strdup(buf);
}
