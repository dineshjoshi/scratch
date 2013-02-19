package Trie;

use strict;
use Data::Dumper;
use Node;

sub new {
    my ($class, $options) = @_;
    my $self = {};
    bless ($self, $class);

    $self->{root} = Node->new('');
    return $self;
}


# add()
# this is supposed to add a word
# to the trie
# algo:
#  curnode = rootnode
#  curnode = curnode->getNodeForChar(c)
#  if c is not last char repeat
#  else
#  curNode->setValue(word); // this should set isEnd = true
sub add {
    my ($self, $word) = @_;
    my @chars = split(//, $word);

    my $curnode = $self->{root};
    for my $char (@chars) {
	$curnode = $curnode->getNodeForChar($char);
    }

    # at this point $curnode should point to
    # the terminal node
    $curnode->setValue($word);
}

sub find {
    my ($self, $word) = @_;
    my @chars = split(//, $word);

    print "Finding $word\n";

    my $curnode = $self->{root};
    for my $char (@chars) {
	$curnode = $curnode->getNodeIfExistsForChar($char);
	if ($curnode == undef) {
	    return;
	}
    }

    return $curnode;
}

sub print {
    my ($self) = @_;
    $self->{root}->print();
}
    

sub getSuggestions {
    my ($self, $prefix) = @_;

    my $search_node = $self->find($prefix);

    return $search_node->getSuggestions();
}



1;
