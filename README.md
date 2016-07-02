# linklist-php-extension
link list datastruct for php

**说明:** 没有什么实际意义,纯粹练手玩玩而已.

### methods
```php
add_head();
fetch_head();
add_tail();
fetch_tail();
fetch_index();
delete_index();
element_nums();
```

### Demo

```php
<?php
$list = new Lb\Linklist();

for ($i = 0; $i < 10; $i++) {
    $list->add_head("hello liubang" . $i);
}

for ($i = 0; $i < $list->element_nums(); $i++) {
    echo $list->fetch_index($i) , PHP_EOL;
}

$list->delete_index(3);
echo $list->element_nums();
echo PHP_EOL;


for ($i = 0; $i < $list->element_nums(); $i++) {
    echo $list->fetch_index($i) , PHP_EOL;
}

$list->add_tail(array(
    'userId' => 1034285,
    'userName' => '东北狠人'
));


var_dump($list->fetch_tail());


class Demo {
    public $name;
    public $gender;
    public function __construct($name, $gender) {
        $this->name = $name;
        $this->gender = $gender;
    }
}
$d = new Demo("刘邦", "男");
$list->add_head($d);

var_dump($list->fetch_head());
```
