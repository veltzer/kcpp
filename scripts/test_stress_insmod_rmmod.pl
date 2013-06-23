#!/usr/bin/perl -w

# this is a stress testing script which runs many insmod/rmmod combinations
# to check that there are no problems in loading/unloading the driver...
#					Mark Veltzer

use strict;
use diagnostics;

my($module)=$ARGV[0];
my($count)=$ARGV[1];
print "module is [$module]\n";
print "count is [$count]\n";

sub my_system($) {
	my($cmd)=$_[0];
	print "doing [$cmd]\n";
	my($res)=system($cmd);
	if($res!=0) {
		die("failed to do cmd [$cmd] with result [$res]");
	}
	#print "res is [$res]\n";
}

for(my($i)=0;$i<$count;$i++) {
	my_system("sudo insmod $module");
	my_system("sudo rmmod $module");
}
