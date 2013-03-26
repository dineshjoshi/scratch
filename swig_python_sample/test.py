import LibSample
import ctypes;

print LibSample.smpl_sum(5, 100);
print LibSample.smpl_sub(5, 100);

lslist = LibSample.list_new();

p = LibSample.new_intp();

LibSample.intp_assign(p, 5);
print LibSample.intp_value(p);

# print lslist;

node = LibSample.list_insert(lslist, p)

print LibSample.list_node_getint(node)
