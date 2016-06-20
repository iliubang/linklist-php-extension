#include "php_linklist.h"

static int le_php_list;
static int freed = 0;

void list_destroy_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	if (!freed) {
		list_head *list;

		list = (list_head *)rsrc->ptr;
		list_destroy(list);

		freed = 1;
	}
}

list_head *list_create()
{
	list_head *head;

	head = (list_head *)emalloc(sizeof(list_head));
	if (head) {
		head->size = 0;
		head->head = NULL;
		head->tail = NULL;
	}
	return head;
}

int list_add_head(list_head *head, zval *value)
{
	list_node *node;
	node = (list_node *)emalloc(sizeof(list_node));
	if (!node)
		return 0;
	node->value = value;
	node->prev = NULL;
	node->next = head->head;
	if (head->head)
		head->head->prev = node;
	head->head = node;

	if (!head->tail)
		head->tail = head->head;
	head->size++;

	return 1;
}

int list_add_tail(list_head *head, zval *value)
{
	list_node *node;
	node = (list_node *)emalloc(sizeof(list_node));
	if (!node)
		return 0;
	node->value = value;
	node->prev = head->tail;
	node->next = NULL;
	if (head->tail)
		head->tail->next = node;
	head->tail = node;
	if (!head->head)
		head->head = head->tail;
	head->size++;
	return 1;
}

int list_delete_index(list_head *head, int index)
{
	list_node *curr;
	if (index < 0) {
		index = (-index) - 1;
		curr = head->tail;
		while(index > 0) {
			curr = curr->prev;
			index--;
		}
	} else {
		curr = head->head;
		while(index > 0) {
			curr = curr->next;
			index--;
		}
	}
	if (!curr || index > 0)	return 0;
	if (curr->prev) {
		curr->prev->next = curr->next;
	} else {
		head->head = curr->next;
	}
	if (curr->next) {
		curr->next->prev = curr->prev;
	} else {
		head->tail = curr->prev;
	}
	efree(curr);
	return 1;
}

int list_fetch(list_head *head, int index, zval **retval)
{
	list_node *node;
	if (index > 0) {
		node = head->head;
		while(node && index > 0) {
			node = node->next;
			index--;
		}
	} else {
		node = head->tail;
		index = (-index) - 1;
		while(node && index > 0) {
			node = node->prev;
			index--;
		}
	}
	if (!node || index > 0) return 0;
    PHPWRITE(Z_STRVAL_P(node->value), Z_STRLEN_P(node->value));
	*retval = node->value;
	return 1;
}

int list_length(list_head *head)
{
	if (head)
		return head->size;
	else
		return 0;
}

void list_destroy(list_head *head)
{
	list_node *curr, *next;
	curr = head->head;
	while(curr)
	{
		next = curr->next;
		efree(curr);
		curr = next;
	}
	efree(head);
}

static zend_function_entry linklist_functions[] = {
	ZEND_FE(list_create, NULL)
	ZEND_FE(list_add_head, NULL)
	ZEND_FE(list_add_tail, NULL)
	ZEND_FE(list_fetch_head, NULL)
	ZEND_FE(list_fetch_tail, NULL)
	ZEND_FE(list_fetch_index, NULL)
	ZEND_FE(list_delete_index, NULL)
	ZEND_FE(list_destroy, NULL)
	ZEND_FE(list_element_nums, NULL)
	ZEND_FE_END
};


ZEND_FUNCTION(list_create)
{
	list_head *list;
	list = list_create();
	if (!list) {
        php_printf("list_create error!\n");
		RETURN_NULL();
    } else {
		ZEND_REGISTER_RESOURCE(return_value, list, le_php_list);
    }
}

ZEND_FUNCTION(list_add_head)
{
	zval *value;
	zval *lrc;
	list_head *list;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &lrc, &value) == FAILURE) {
		RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        php_printf("list_add_head fetch resource error!\n");
        RETURN_FALSE;
    }
	if(list_add_head(list, value)) {
        PHPWRITE(Z_STRVAL_P(value), Z_STRLEN_P(value));
        php_printf("\n");
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

ZEND_FUNCTION(list_fetch_head)
{
    zval *retval, *lrc;
    list_head *list;
    int res;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &lrc) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        php_printf("list_fetch_head fetch resource error!\n");
        RETURN_FALSE;
    }

    res = list_fetch(list, 0, &retval);

    if (!res) {
        php_printf("list_fetch_head fetch resource error\n");
        RETURN_NULL();
    } else {
        RETURN_ZVAL(retval, 1, 0);
    }
}

ZEND_FUNCTION(list_add_tail)
{
    zval *value;
    zval *lrc;
    list_head *list;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &lrc, &value) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        php_printf("list_add_tail fetch resource error!\n");
        RETURN_FALSE;
    }
    list_add_tail(list, value);
    RETURN_TRUE;
}

ZEND_FUNCTION(list_fetch_tail)
{
    zval *lrc, *retval;
    list_head *list;
    int res;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &lrc) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        RETURN_FALSE;
    }
    res = list_fetch(list, list_length(list) - 1, &retval);
    if (!res) {
        RETURN_NULL();
    }
    RETURN_ZVAL(retval, 1, 0);
}

ZEND_FUNCTION(list_fetch_index)
{
    zval *lrc, *retval;
    list_head *list;
    long index;
    int res;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &lrc, &index) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        RETURN_FALSE;
    }
    res = list_fetch(list, index, &retval);
    if (!res) {
        RETURN_NULL();
    }
    RETURN_ZVAL(retval, 1, 0);
}

ZEND_FUNCTION(list_delete_index)
{
    zval *lrc;
    list_head *list;
    long index;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &lrc, &index) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        RETURN_FALSE;
    }
    if (list_delete_index(list, index)) {
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

ZEND_FUNCTION(list_destroy)
{
    zval *lrc;
    list_head *list;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &lrc) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        RETURN_FALSE;
    }
    if (!freed){
        list_destroy(list);
        freed = 1;
    }
    RETURN_TRUE;
}

ZEND_FUNCTION(list_element_nums)
{
    zval *lrc;
    list_head *list;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &lrc) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, "List Resource", le_php_list);
    if (!list) {
        RETURN_FALSE;
    }
    RETURN_LONG(list_length(list));
}


ZEND_MINIT_FUNCTION(linklist)
{
    le_php_list = zend_register_list_destructors_ex(
        list_destroy_handler, NULL, "List Resource", module_number);
	return SUCCESS;
}

ZEND_MSHUTDOWN_FUNCTION(linklist)
{
    return SUCCESS;
}

ZEND_RINIT_FUNCTION(linklist)
{
    return SUCCESS;
}

ZEND_RSHUTDOWN_FUNCTION(linklist)
{
    return SUCCESS;
}

ZEND_MINFO_FUNCTION(linklist)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "php link list support", "enable");
    php_info_print_table_row(2, "Version", PHP_LINKLIST_VERSION);
    php_info_print_table_row(2, "Author", PHP_LINKLIST_AUTHOR);
    php_info_print_table_end();
}

zend_module_entry linklist_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"linklist",
	linklist_functions,
	ZEND_MINIT(linklist),
	ZEND_MSHUTDOWN(linklist),
	ZEND_RINIT(linklist),
	ZEND_RSHUTDOWN(linklist),
	ZEND_MINFO(linklist),
#if ZEND_MODULE_API_NO >= 20010901
	"1.0",
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LINKLIST
	ZEND_GET_MODULE(linklist)
#endif
