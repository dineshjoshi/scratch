use LibSample;

$list = LibSample::list_new;

for($i=0; $i < 10; $i++) {
    $ip = LibSample::new_intp;
    LibSample::intp_assign($ip, 10+$i);
    $node = LibSample::list_insert($list, $ip);
}

$node = $list;
for($i=0; $i < 10; $i++) {
    $node = LibSample::list_next($node);
    print LibSample::list_node_getint($node) . "\n";
}

