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

#define PHP_LINKLIST_DESCRIPTOR_NAME "List Resource"
#define LIUBANG_LINKLIST_PROPERTY_NAME "_linklist"

#define LIUBANG_ME(c, m, a, f) {m, PHP_MN(c), a, (zend_uint) (sizeof(a)/sizeof(struct _zend_arg_info)-1), f},


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

static void php_linklist_descriptor_dotr(zend_rsrc_list_entry *rsrc TSRMLS_DC);

static list_head *list_create();
static int list_add_head(list_head *head, zval *value);
static int list_add_tail(list_head *head, zval *value);
static int list_delete_index(list_head *head, int index);
static int list_fetch(list_head *head, int index, zval **retval);
static int list_length(list_head *head);
static void list_destroy(list_head **head);

PHP_METHOD(lb_linklist, __construct);
PHP_METHOD(lb_linklist, add_head);
PHP_METHOD(lb_linklist, fetch_head);
PHP_METHOD(lb_linklist, add_tail);
PHP_METHOD(lb_linklist, fetch_tail);
PHP_METHOD(lb_linklist, fetch_index);
PHP_METHOD(lb_linklist, delete_index);
PHP_METHOD(lb_linklist, element_nums);
PHP_METHOD(lb_linklist, __destruct);


PHP_MINIT_FUNCTION(linklist);
PHP_MSHUTDOWN_FUNCTION(linklist);
PHP_RINIT_FUNCTION(linklist);
PHP_RSHUTDOWN_FUNCTION(linklist);
PHP_MINFO_FUNCTION(linklist);

#endif
