PHP_ARG_ENABLE(linklist,
	[Whether to enable the "linklist" extension],
	[--enable-linklist		Enable the "linklist" extension support]
)

if test $PHP_LINKLIST != "no"; then
	PHP_SUBST(LINKLIST_SHARED_LIBADD)
	PHP_NEW_EXTENSION(linklist, linklist.c, $ext_shared)
fi
