<?php
$list = new Lb\Linklist();

var_dump($list);

for ($i = 0; $i < 10; $i++) { 
	$list->add_head("hello liubang" . $i);
}

$list->delete_index(3);
//echo $list->element_nums();
//echo PHP_EOL;

for ($i = 0; $i < $list->element_nums(); $i++) {
	echo $list->fetch_index($i), PHP_EOL;
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

//var_dump($d);

$list->add_head($d);

var_dump($list->fetch_head());
