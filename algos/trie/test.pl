use Trie;
use strict;
use Data::Dumper;

my $t = Trie->new();

$t->add('test');
$t->add('ted');
$t->add('tea');
$t->add('team');
$t->add('tester');

$t->print();


if ($t->find("tea") != undef) {
    print "Found!\n";
} else {
    print "NOT found!\n";
}


print Dumper($t->getSuggestions('tea'));
