<?php
$list = new Lb\Linklist();

var_dump($list);

for ($i = 0; $i < 10; $i++) {
    $list->list_add_head("hello liubang " . $i);
}

echo $list->list_fetch_index(2);

for ($i = 0; $i < $list->list_element_nums(); $i++) {
    //echo $list->list_fetch_index($i);
}