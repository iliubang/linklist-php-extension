<?php


$list = list_create();

var_dump($list);

for ($i = 0; $i < 111; $i++) {
    //var_dump(list_add_head($list, "elements"));
    list_add_head($list, "elements");
}

var_dump(list_fetch_index($list, 3));

