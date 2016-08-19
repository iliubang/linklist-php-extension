#include "php_linklist.h"

zend_class_entry *linklist_ce;
static int le_linklist_descriptor;
static int freed = 0;

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_construct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_add_head_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_add_tail_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_fetch_head_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_fetch_tail_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_fetch_index_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_delete_index_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_element_nums_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lb_linklist_destruct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

static void php_linklist_descriptor_dotr(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	if (!freed)
	{
		list_head *head = (list_head *) rsrc->ptr;
		list_destroy(&head);
		freed = 1;
	}
}

static list_head *list_create()
{
	list_head *head;

	head = (list_head *) emalloc(sizeof(list_head));
	if (head)
	{
		head->size  = 0;
		head->head  = NULL;
		head->tail  = NULL;
	}
	return (head);
}


static int list_add_head(list_head * head, zval * value)
{
	list_node *node;
	node = ecalloc(1, sizeof(list_node));
	if (!node)
		return (0);
	MAKE_STD_ZVAL(node->value);
	ZVAL_ZVAL(node->value, value, 1, 0);
	node->prev = NULL;
	node->next = head->head;
	if (head->head)
		head->head->prev = node;
	head->head = node;

	if (!head->tail)
		head->tail = head->head;
	head->size++;

	return (1);
}


static int list_add_tail(list_head * head, zval * value)
{
	list_node *node;
	node = ecalloc(1, sizeof(list_node));
	//node = (list_node *) emalloc(sizeof(list_node));
	if (!node)
		return (0);
	MAKE_STD_ZVAL(node->value);
	ZVAL_ZVAL(node->value, value, 1, 0);
	node->prev = head->tail;
	node->next = NULL;
	if (head->tail)
		head->tail->next = node;
	head->tail = node;
	if (!head->head)
		head->head = head->tail;
	head->size++;
	return (1);
}


static int list_delete_index(list_head * head, int index)
{
	list_node *curr;
	if (index < 0)
	{
		index = (-index) - 1;
		curr  = head->tail;
		while (index > 0 && curr->prev != NULL)
		{
			curr = curr->prev;
			index--;
		}
	}
	else
	{
		curr = head->head;
		while (index > 0 && curr->next != NULL)
		{
			curr = curr->next;
			index--;
		}
	}
	if (!curr || index > 0)
		return (0);
	if (curr->prev)
	{
		curr->prev->next = curr->next;
	}
	else
	{
		head->head = curr->next;
	}
	if (curr->next)
	{
		curr->next->prev = curr->prev;
	}
	else
	{
		head->tail = curr->prev;
	}
	zval_ptr_dtor(&curr->value);
	/* LIUBANG_UNINITIALIZED_ZVAL( curr->value ); */
	efree(curr);
	head->size--;
	return (1);
}


static int list_fetch(list_head * head, int index, zval ** retval)
{
	list_node *node;
	if (index >= 0)
	{
		node = head->head;
		while (node && index > 0)
		{
			node = node->next;
			index--;
		}
	}
	else
	{
		node = head->tail;
		index = (-index) - 1;
		while (node && index > 0)
		{
			node = node->prev;
			index--;
		}
	}
	if (!node || index > 0)
		return (0);
	*retval = node->value;
	return (1);
}


static int list_length(list_head * head)
{
	if (head)
		return (head->size);
	else
		return (0);
}


static void list_destroy(list_head ** head)
{
	list_node *curr, *next;
	curr = (*head)->head;
	while (curr != NULL)
	{
		next = curr->next;
		/*      (*head)->head = next; */
		zval_ptr_dtor(&curr->value);
		efree(curr);
		(*head)->size--;
		curr = next;
	}
	(*head)->head = NULL;
	(*head)->tail = NULL;
	efree(*head);
}


static zend_function_entry linklist_methods[] =
{
	PHP_ME(lb_linklist, __construct, lb_linklist_construct_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(lb_linklist, __destruct, lb_linklist_destruct_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(lb_linklist, add_head, lb_linklist_add_head_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(lb_linklist, fetch_head, lb_linklist_fetch_head_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(lb_linklist, add_tail, lb_linklist_add_tail_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(lb_linklist, fetch_tail, lb_linklist_fetch_tail_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(lb_linklist, fetch_index, lb_linklist_fetch_index_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(lb_linklist, delete_index, lb_linklist_delete_index_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(lb_linklist, element_nums, lb_linklist_element_nums_arginfo, ZEND_ACC_PUBLIC)
	{
		NULL, NULL, NULL
	}
};


/** {{{ proto Lb\Linklist::__construct(void)
 */
PHP_METHOD(lb_linklist, __construct)
{
	list_head *list;
	list = list_create();
	zval *list_res;
	MAKE_STD_ZVAL(list_res);
	if (!list)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to create a linklist");
		RETURN_NULL();
	}
	else
	{
		ZEND_REGISTER_RESOURCE(list_res, list, le_linklist_descriptor);
		zend_update_property(linklist_ce, getThis(), ZEND_STRL( LIUBANG_LINKLIST_PROPERTY_NAME ), list_res TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto Lb\Linklist::add_head(mixed)
 */
PHP_METHOD( lb_linklist, add_head )
{
	zval        *value;
	zval        *lrc;
	list_head   *list;
	/* 解析用户传来的参数 */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &value) == FAILURE)
	{
		RETURN_FALSE;
	}
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to add an element to linklist");
		RETURN_FALSE;
	}
	if (list_add_head(list, value))
	{
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */


/** {{{ proto Lb\Linklist::fetch_head(void)
 */
PHP_METHOD(lb_linklist, fetch_head)
{
	zval        *retval, *lrc;
	list_head   *list;
	int     res;
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to fetch head element");
		RETURN_FALSE;
	}
	res = list_fetch(list, 0, &retval);
	if (!res)
	{
		RETURN_NULL();
	}
	else
	{
		RETURN_ZVAL(retval, 1, 0);
	}
}
/* }}} */


/** {{{ proto Lb\Linlist::add_tail(mixed)
 */
PHP_METHOD(lb_linklist, add_tail)
{
	zval        *value;
	zval        *lrc;
	list_head   *list;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &value) == FAILURE)
	{
		RETURN_FALSE;
	}
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		php_error_docref( NULL TSRMLS_CC, E_WARNING,
		                  "Unable to add an element to linklist" );
		RETURN_FALSE;
	}
	list_add_tail(list, value);
	RETURN_TRUE;
}
/* }}} */


/** {{{ proto Lb\Linklist::fetch_tail(void)
 */
PHP_METHOD(lb_linklist, fetch_tail)
{
	zval *lrc, *retval;
	list_head *list;
	int res;
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
		                 "Unable to fetch tail element");
		RETURN_FALSE;
	}
	res = list_fetch(list, list_length(list) - 1, &retval);
	if (!res)
	{
		RETURN_NULL();
	}
	RETURN_ZVAL(retval, 1, 0);
}
/* }}} */


/** {{{ proto Lb\Linklist::fetch_index(int)
 */
PHP_METHOD(lb_linklist, fetch_index)
{
	zval *lrc, *retval;
	list_head *list;
	long index;
	int res;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE)
	{
		RETURN_FALSE;
	}
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		php_error_docref( NULL TSRMLS_CC, E_WARNING,
		                  "Unable to fetch an element" );
		RETURN_FALSE;
	}
	res = list_fetch(list, index, &retval);
	if (!res)
	{
		RETURN_NULL();
	}
	RETURN_ZVAL(retval, 1, 0);
}
/* }}} */


/** {{{ proto Lb\Linklist::delete_index(int)
 */
PHP_METHOD(lb_linklist, delete_index)
{
	zval        *lrc;
	list_head   *list;
	long        index;
	if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &index ) == FAILURE)
	{
		RETURN_FALSE;
	}
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		RETURN_FALSE;
	}
	if (list_delete_index(list, index))
	{
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */


/** {{{ proto Lb\Linklist::element_nums(void)
 */
PHP_METHOD( lb_linklist, element_nums )
{
	zval        *lrc;
	list_head   *list;
	/* 读取对象属性,为一个资源类型 */
	lrc = zend_read_property(linklist_ce, getThis(), ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), 0 TSRMLS_CC);
	ZEND_FETCH_RESOURCE_NO_RETURN(list, list_head *, &lrc, -1, PHP_LINKLIST_DESCRIPTOR_NAME, le_linklist_descriptor);
	if (!list)
	{
		RETURN_FALSE;
	}
	RETURN_LONG(list_length(list));
}
/* }}} */


/** {{{ proto Lb\Linklist::__destruct(void)
 */
PHP_METHOD( lb_linklist, __destruct )
{
}
/* }}} */


PHP_MINIT_FUNCTION(linklist)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Lb\\Linklist", linklist_methods);
	linklist_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_null(linklist_ce, ZEND_STRL(LIUBANG_LINKLIST_PROPERTY_NAME), ZEND_ACC_PROTECTED TSRMLS_CC);
	le_linklist_descriptor = zend_register_list_destructors_ex(
	                             php_linklist_descriptor_dotr,
	                             NULL,
	                             PHP_LINKLIST_DESCRIPTOR_NAME,
	                             module_number );
	return (SUCCESS);
}

PHP_MSHUTDOWN_FUNCTION(linklist)
{
	return (SUCCESS);
}

PHP_RINIT_FUNCTION(linklist)
{
	return (SUCCESS);
}

PHP_RSHUTDOWN_FUNCTION(linklist)
{
	return (SUCCESS);
}

PHP_MINFO_FUNCTION(linklist)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "php link list support", "enable");
	php_info_print_table_row(2, "Version", PHP_LINKLIST_VERSION);
	php_info_print_table_row(2, "Author", PHP_LINKLIST_AUTHOR);
	php_info_print_table_end();
}

zend_module_entry linklist_module_entry =
{
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"linklist",
	NULL,
	PHP_MINIT( linklist ),
	PHP_MSHUTDOWN( linklist ),
	PHP_RINIT( linklist ),
	PHP_RSHUTDOWN( linklist ),
	PHP_MINFO( linklist ),
#if ZEND_MODULE_API_NO >= 20010901
	"1.0",
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LINKLIST
ZEND_GET_MODULE( linklist )
#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
