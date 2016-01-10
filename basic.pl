#!/usr/bin/perl -w

# Script description
# copyright

my $scriptname  = $0;
$scriptname =~ s|.*/||;
my $HELP =  <<ENDHELP

  name:
    $scriptname

  synopsis:
    $scriptname [-debug] [options]

  description:
    A script to ... 

  options:
    -debug           enable debug
    -help            print out this help message

ENDHELP
;

use Getopt::Long;
my $debug = 0;
GetOptions("debug"   => \$debug,
           "help"      => sub {print $HELP; exit(1)},
    );


