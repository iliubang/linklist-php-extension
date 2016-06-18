<?php


$list = list_create();

for ($i = 0; $i < 10; $i++) {
    list_add_head($list, "test_" . $i);
}

var_dump(list_fetch_head($list));

//echo PHP_EOL . list_element_nums($list) . PHP_EOL;

