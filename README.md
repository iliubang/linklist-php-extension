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
$list = new Lb\Linklist();

for ($i = 0; $i < 10; $i++) {
    $list->list_add_head("hello liubang " . $i);
}

for ($i = 0; $i < $list->list_element_nums(); $i++) {
    echo $list->list_fetch_index($i);
}
```
