/*
 * slist.c
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to SORCERER -- SORCERER is in the public
 * domain.  An individual or company may do whatever they wish with
 * source code distributed with SORCERER or the code generated by
 * SORCERER, including the incorporation of SORCERER, or its output, into
 * commerical software.
 *
 * We encourage users to develop software with SORCERER.  However, we do
 * ask that credit is given to us for developing SORCERER.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like SORCERER and have developed a nice tool with the
 * output, please mention that you developed it using SORCERER.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * SORCERER 1.00B
 * Terence Parr
 * AHPCRC, University of Minnesota
 * 1992-1994
 */
#include <stdio.h>
#include <setjmp.h>

#ifdef PCCTS_USE_STDARG
#include <stdarg.h>
#else
#include <stdarg.h>
#endif

#include "CASTBase.h"
#include "sorlist.h"

/* Iterate over a list of elements; returns ptr to a new element
 * in list upon every call and NULL when no more are left.
 * Very useful like this:
 *
 *		cursor = mylist;
 *		while ( (p=slist_iterate(mylist,&cursor)) ) {
 *			/ * place with element p * /
 *		}
 *
 * The cursor must be initialized to point to the list to iterate over.
 */
void *
#ifdef __USE_PROTOS
slist_iterate(SList *list, SList **cursor)
#else
slist_iterate(list, cursor)
SList *list, **cursor;
#endif
{
	void *e;

	if ( list==NULL || cursor == NULL || *cursor==NULL ) return NULL;
	if ( list== *cursor ) { *cursor = (*cursor)->next; }
	e = (*cursor)->elem;
	(*cursor) = (*cursor)->next;
	return e;
}

/*
 * add an element to a list.
 *
 * Any non-empty list has a sentinel node whose 'elem' pointer is really
 * a pointer to the last element.  (i.e. length(list) = #elemIn(list)+1).
 * Elements are appended to the list.
 */
void
#ifdef __USE_PROTOS
slist_add( SList **list, void *e )
#else
slist_add( list, e )
SList **list;
void *e;
#endif
{
	SList *p, *tail;
	require(e!=NULL, "slist_add: attempting to add NULL list element");

	p = newSList;
	require(p!=NULL, "slist_add: cannot alloc new list node");
	p->elem = e;
	if ( *list == NULL )
	{
		SList *sentinel = newSList;
		require(sentinel!=NULL, "slist_add: cannot alloc sentinel node");
		*list=sentinel;
		sentinel->next = p;
		sentinel->elem = (char *)p;		/* set tail pointer */
	}
	else								/* find end of list */
	{
		tail = (SList *) (*list)->elem;	/* get tail pointer */
		tail->next = p;
		(*list)->elem = (char *) p;		/* reset tail */
	}
}

void
#ifdef __USE_PROTOS
slist_free(SList *list)
#else
slist_free(list)
SList *list;
#endif
{
	SList *p,*q;

	if ( list==NULL ) return;	/* empty list */
	for (p = list->next; p!=NULL; p=q)
	{
		q = p->next;
		free(p);
	}
}
