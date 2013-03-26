#ifdef SWIG
%module LibSample
%include "cpointer.i"
%pointer_functions(int, intp);
/* %pointer_functions(void, voidp); */
%{
#include "libsample.h"
%}
#endif

#ifndef _LIBSAMPLE_H_
#define _LIBSAMPLE_H_

struct _node {
    void            *data;
    struct _node    *next;
};

extern struct _node * list_new    (void);
extern void           list_free   (struct _node *);
extern struct _node * list_insert (struct _node *, void *);
extern struct _node * list_next   (struct _node *);


extern int list_node_getint (struct _node *);



/* int            (*cmp)    (struct _node *, struct _node *); */
/* struct _node * (*getnext)(); */

extern int smpl_sum(int a, int b);
extern int smpl_sub(int a, int b);

#endif
