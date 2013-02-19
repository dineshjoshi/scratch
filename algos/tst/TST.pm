package TST;

use Data::Dumper;
use strict;

sub new {
    my ($class, $options) = @_;
    my $self = {};

    $self->{root} = create_node('', 0);

    bless ($self, $class);
    return $self;
}

sub create_node {
    my ($char, $isEnd) = @_;
    my $r_hash = {splitchar => $char, isEnd => $isEnd, left => undef, right => undef, middle => undef};
    return $r_hash;
}

sub _add {
    my ($self, $inode, $word, $char_pos) = @_;
    my $node = \$_[1];

    my @aword = split(//, $word);
    my $char  = $aword[$char_pos];

    my $isEnd = ($char_pos == length($word)-1);
    if (!defined ${$node}) {
	${$node} = create_node($char, $isEnd);
	if ($isEnd) {
	    return $node;
	}
    }

    if (${$node}->{splitchar} eq $char) {
	if ($isEnd) {
	    ${$node}->{isEnd} = 1;
	} else {
	    $self->_add(${$node}->{middle}, $word, $char_pos+1);
	}
    } 
    elsif (ord($char) < ord(${$node}->{splitchar})) {
    	$self->_add(${$node}->{left}, $word, $char_pos);
    } 
    elsif (ord($char) > ord(${$node}->{splitchar})) {
    	$self->_add(${$node}->{right}, $word, $char_pos);
    }
    return $node;
}

sub add {
    my ($self, $word) = @_;
    $self->_add($self->{root}->{middle}, $word, 0);
}

sub _dfs {
    my ($self, $node, $word, $aref) = @_;
    
    if (!defined $node) {
	return;
    }
    my $tword = $word . $node->{splitchar};

    if ($node->{isEnd} == 1) {
	push (@{$aref}, $tword);
    }

    $self->_dfs($node->{middle}, $tword, $aref);
    $self->_dfs($node->{left}, $word, $aref);
    $self->_dfs($node->{right}, $word, $aref);
}

sub dfs {
    my ($self, $node, $word, $aref) = @_;
    
    if (!defined $node) {
	return;
    }
    my $tword = $word . $node->{splitchar};

    if ($node->{isEnd} == 1) {
	push (@{$aref}, $tword);
    }
    $self->_dfs($node->{middle}, $tword, $aref);
}



sub search {
    my ($self, $prefix) = @_;
    my @aprefix = split(//, $prefix);

    my $root = $self->{root};
    for my $prefix_char (@aprefix) {
    	$root = $self->find($root->{middle}, $prefix_char);
	
    	if (!defined $root) {
    	    return;
    	}
    }    

    # found prefix, do a DFS to look for autocomplete entries
    my @suggestions = ();
    chop $prefix;
    $self->dfs($root, $prefix, \@suggestions);

    return \@suggestions;
}

sub find {
    my ($self, $root, $char) = @_;
    if (! defined($root)) {
	return;
    }

    if ($root->{splitchar} eq $char) {
	return $root;
    }

    if (ord($char) < ord($root->{splitchar})) {
	return $self->find($root->{left}, $char);
    } elsif (ord($char) > ord($root->{splitchar})) {
	return $self->find($root->{right}, $char);
    }

    return;
}

sub getRoot {
    my ($self) = @_;
    return $self->{root};
}

1;
