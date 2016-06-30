<?php
$list = new Lb\Linklist();

//var_dump($list);

for ($i = 0; $i < 5; $i++) {
    $list->list_add_tail("hello liubang" . $i);
    //echo $i , "\n";
}

//echo $list->list_fetch_tail();

//die;

for ($i = 0; $i < $list->list_element_nums(); $i++)  {
    echo $i . "===";
    echo $list->list_fetch_index($i);
    echo PHP_EOL;
}
//var_dump($list->list_fetch_head());

