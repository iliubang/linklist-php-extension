<?php

namespace Lb;

class Linklist
{
    /**
     * @var null|resource
     */
    protected $_linklist = NULL;


    /**
     * Linklist constructor.
     */
    public function __construct() {}

    /**
     * @param mixed $element
     * @return NULL|true
     */
    public function add_head($element) {}

    /**
     * @return mixed
     */
    public function fetch_head() {}

    /**
     * @param mixed $element
     * @return NULL|true
     */
    public function add_tail($element) {}

    /**
     * @return mixed
     */
    public function fetch_tail() {}

    /**
     * @param int $index
     * @return mixed
     */
    public function fetch_index($index) {}

    /**
     * @param int $index
     * @return bool
     */
    public function delete_index($index) {}

    /**
     * @return int
     */
    public function element_nums() {}

    /**
     * the destruct method
     */
    public function __destruct() {}

}

