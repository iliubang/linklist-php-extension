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

#ifndef ZEND_FETCH_RESOURCE_NO_RETURN
#define ZEND_FETCH_RESOURCE_NO_RETURN(rsrc, rsrc_type, passed_id, default_id, resource_type_name, resource_type)    \
        (rsrc = (rsrc_type) zend_fetch_resource(passed_id TSRMLS_CC, default_id, resource_type_name, NULL, 1, resource_type))
#endif


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

#define PHP_LIST_DESC_NAME "php双向链表资源描述符"

static list_head *list_create();
static int list_add_head(list_head *head, zval *value);
static int list_add_tail(list_head *head, zval *value);
static int list_delete_index(list_head *head, int index);
static int list_fetch(list_head *head, int index, zval **retval); 
static int list_length(list_head *head);
static void list_destroy(list_head *head);


#define phpext_linklist_ptr &linklist_module_entry
extern zend_module_entry linklist_module_entry;

ZEND_MINIT_FUNCTION(linklist);
ZEND_MSHUTDOWN_FUNCTION(linklist);
ZEND_RINIT_FUNCTION(linklist);
ZEND_RSHUTDOWN_FUNCTION(linklist);
ZEND_MINFO_FUNCTION(linklist);

ZEND_FUNCTION(list_create);
ZEND_FUNCTION(list_add_head);
ZEND_FUNCTION(list_fetch_head);
ZEND_FUNCTION(list_add_tail);
ZEND_FUNCTION(list_fetch_tail);
ZEND_FUNCTION(list_fetch_index);
ZEND_FUNCTION(list_delete_index);
ZEND_FUNCTION(list_destroy);
ZEND_FUNCTION(list_element_nums);


#endif
