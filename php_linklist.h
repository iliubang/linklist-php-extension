#ifndef _LINKLIST_H
#define _LINKLIST_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#define PHP_LINKLIST_VERSION "0.1-dev"
#define PHP_LINKLIST_AUTHOR  "liubang"

#define phpext_linklist_ptr &linklist_module_entry
extern zend_module_entry linklist_module_entry;


typedef struct _list_node list_node;
struct _list_node {
	zval *value;
	list_node *prev;
	list_node *next;
};

typedef struct _list_head list_head;
struct _list_head {
	int size;
	list_node *head;
	list_node *tail;
};

list_head *list_create();
int list_add_head(list_head *head, zval *value);
int list_add_tail(list_head *head, zval *value);
int list_delete_index(list_head *head, int index);
int list_fetch(list_head *head, int index, zval **retval); 
int list_length(list_head *head);
void list_destroy(list_head *head);


ZEND_MINIT_FUNCTION(linklist);
ZEND_MSHUTDOWN_FUNCTION(linklist);
ZEND_RINIT_FUNCTION(linklist);
ZEND_RSHUTDOWN_FUNCTION(linklist);
ZEND_MINFO_FUNCTION(linklist);


#endif
