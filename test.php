<?php
$list = new Lb\Linklist();

for ($i = 0; $i < 100; $i++) {
	$list->list_add_head("hello liubang" . $i);
}

for ($i = 0; $i < $list->list_element_nums(); $i++) {
	echo $list->list_fetch_index($i);
}


class Demo
{
	private $name;

	private $gender;

	public function __construct($name, $gender)
	{
		$this->name = $name;
		$this->gender = $gender;
	}
}


$d = new Demo('liubang', '男');

var_dump($d);

$list->list_add_tail($d);

var_dump($list->list_fetch_tail());