# linklist-php-extension
link list datastruct for php

**说明:** 
转眼间，已经正式参加工作一年时间了，也意味着告别学生时代整整一年。在这一年里，从一开始的很不适应到适应，一个人也经历了许许多多的辛酸。回顾这一年，一半在无所事事，一半在碌碌无为。很早就计划着能做点什么，可是却总是很难付诸行动。
那么以此作为留念吧。

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
