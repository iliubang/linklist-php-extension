<?php


$list = new Lb\Linklist();

$list->list_add_head("hello liubang");
$list->list_add_head("hello linger");
echo $list->list_fetch_head();
echo $list->list_fetch_tail();