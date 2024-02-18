#ifndef _mylib_h_
#define _mylib_h_

#ifdef __cplusplus
extern "C" {
#endif

struct mylib_st;

int mylib_init(struct mylib_st**);
int mylib_term(struct mylib_st*);
#ifdef __cplusplus
}
#endif
#endif