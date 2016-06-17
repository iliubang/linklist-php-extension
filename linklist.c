#include "php_linklist.h"

static int le_php_list;
static int freed = 0;

static list_head *list_create()
{
	list_head *head;

	head = (list_head *)malloc(sizeof(list_head));
	if (head) {
		head->size = 0;
		head->head = NULL;
		head->tail = NULL;
	}
	return head;
}

static int list_add_head(list_head *head, zval *value)
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

static int list_add_tail(list_head *head, zval *value)
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

static int list_delete_index(list_head *head, int index)
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
	free(curr);
	return 1;
}

static int list_fetch(list_head *head, int index, zval **retval)
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
	*retval = node->value;
	return 1;
}

static int list_length(list_head *head)
{
	if (head)
		return head->size;
	else 
		return 0;
}

static void list_destroy(list_head *head)
{
	list_node *curr, *next;
	curr = head->head;
	while(curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	free(head);
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
	ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
	if(list_add_head(list, value)) {
        //PHPWRITE(Z_STRVAL_P(value), Z_STRLEN_P(value));     
        RETURN_TRUE;
    } 
    RETURN_FALSE;
}

ZEND_FUNCTION(list_fetch_head)
{
    zval *lrc,*retval;
    list_head *list;
    int res;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &lrc) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
    res = list_fetch(list, 0, &retval);
    if (!res) {
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
    ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
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
    ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
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
    ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
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
    ZEND_FETCH_RESOURCE(list, list_head *, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
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
    ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
    if (!freed){
        list_destroy(list);
        freed = 1;
    }
}

ZEND_FUNCTION(list_element_nums)
{
    zval *lrc;
    list_head *list;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &lrc) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE(list, list_head*, &lrc, -1, PHP_LIST_DESC_NAME, le_php_list);
    RETURN_LONG(list_length(list));
}


ZEND_MINIT_FUNCTION(linklist)
{
    le_php_list = zend_register_list_destructors_ex(NULL, NULL, PHP_LIST_DESC_NAME, module_number);
}

zend_module_entry linklist_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER, 
#endif
	"linklist",
	linklist_functions,
	ZEND_MINIT(linklist),
	NULL,
	NULL,
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	"1.0",
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LINKLIST
	ZEND_GET_MODULE(linklist)
#endif
