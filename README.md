# linklist-php-extension
link list datastruct for php

**说明:** 没有什么实际意义,纯粹练手玩玩而已.

### methods
```php
list_add_head();
list_fetch_head();
list_add_tail();
list_fetch_tail();
list_fetch_index();
list_delete_index();
list_element_nums();
```

### Demo

```php
<?php
$list = new Lb\Linklist();

for ($i = 0; $i < 10; $i++) {
    $list->list_add_head("hello liubang" . $i);
}

for ($i = 0; $i < $list->list_element_nums(); $i++) {
    echo $list->list_fetch_index($i) , PHP_EOL;
}

$list->list_delete_index(3);
echo $list->list_element_nums();
echo PHP_EOL;


for ($i = 0; $i < $list->list_element_nums(); $i++) {
    echo $list->list_fetch_index($i) , PHP_EOL;
}

$list->list_add_tail(array(
    'userId' => 1034285,
    'userName' => '东北狠人'
));


var_dump($list->list_fetch_tail());


class Demo {
    public $name;
    public $gender;
    public function __construct($name, $gender) {
        $this->name = $name;
        $this->gender = $gender;
    }
}
$d = new Demo("刘邦", "男");
$list->list_add_head($d);

var_dump($list->list_fetch_head());
```

### TODO
[] 已知bug: 往链表中存储object类型的数据,读取数据的时候会引起段错误.