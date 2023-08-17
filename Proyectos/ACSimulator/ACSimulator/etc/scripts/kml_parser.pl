#!/usr/bin/perl.exe

#================================================================
# Fichero km_parser.pl
#================================================================
#  FICHERO: km_parser for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-09-2022
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

$file = "@ARGV[0]";
open (FILE, "< $file");
@lines = <FILE>; 
chop (@lines);
close (FILE);

#print "lon,lat\n";
#<gx:coord>2.18069184 41.4193733 98</gx:coord>
foreach $line (@lines)
{
	#print "--$line\n";


	
	if ($line =~ m/<gx\:coord>/)
	{
		$line =~ s/\s*<gx\:coord>//g;
		$line =~ s/<\/gx\:coord>\s*//g;
		($lon, $lat, $speed) = split /\s/, $line;

		print "$lat,$lon\n";
	}

}
