package Node;

use strict;
use Data::Dumper;

sub new {
    my ($class, $char, $value, $isEnd) = @_;
    my $self = {};
    bless ($self, $class);

    $self->{char}     = $char;
    $self->{value}    = $value || '';
    $self->{isEnd}    = $isEnd || 0;
    $self->{children} = {};
    return $self;
}

sub getNodeForChar {
    my ($self, $char) = @_;

    if (!exists $self->{children}->{$char}) {
	$self->{children}->{$char} = Node->new($char);
    }
    return $self->{children}->{$char};
}

sub getNodeIfExistsForChar {
    my ($self, $char) = @_;

    if (exists $self->{children}->{$char}) {
	return $self->{children}->{$char};
    }

    return;
}

sub setValue {
    my ($self, $value) = @_;
    
    $self->{value} = $value;
    $self->{isEnd} = 1;
}

sub print {
    my ($self) = @_;

    print "c: " . $self->{char} . " v: " . 
	$self->{value} . " ie: " . $self->{isEnd} . "\n";

    for my $child (values(%{$self->{children}})) {
	$child->print();
    }
}


sub getSuggestions {
    my ($self) = @_;

    my @retlist = ();
    if ($self->{isEnd}) {
	push(@retlist, $self->{value});
    }

    if (scalar($self->{children}) == 0) {
	return @retlist;
    }

    for my $child (values %{$self->{children}}) {
	@retlist = (@retlist, @{$child->getSuggestions()});
    }

    return \@retlist;
}
1;
