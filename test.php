<?php


$list = list_create();


for ($i = 0; $i < 5; $i++) {
    //var_dump(list_add_head($list, "elements"));
    list_add_head($list, "elements");
}

echo list_element_nums($list);

var_dump(list_fetch_head($list));

var_dump(list_destroy($list));

