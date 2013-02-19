use TST;

use strict;
use Data::Dumper;
use Time::HiRes qw (gettimeofday);

my $tst = TST->new();

my @words = ('ca', 'cut', 'at','attr', 'ats', 'c', 'crab', 'cat');

for my $word (@words) {
    $tst->add($word);
}

# my $fh;
# open ($fh, "/usr/share/dict/words");

# my $c=0;
# while (<$fh>) {
#     chomp($_);
#     $tst->add($_);

#     if ($c % 10000 == 1) {
# 	print $c . "\n";
#     }
#     $c++;
# }

my $prefix = 'ca';

my ($s_sec, $s_us) = gettimeofday();
my $results = $tst->search($prefix);
my ($e_sec, $e_us) = gettimeofday();
print Dumper($results);

print "$s_sec.$s_us\n";
print "$e_sec.$e_us\n";
# print Dumper($tst->getRoot());
