#!/usr/bin/perl.exe

#================================================================
# Fichero recorrido.pl
#================================================================
#  FICHERO: recorrido.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 29-09-2021
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;
use Time::Local;

$file = "@ARGV[0]";
open (FILE, "< ./$file");
@lines = <FILE>; 
chop (@lines);
close (FILE);

$RECORRIDO = "@ARGV[1]";

$file_output = IO::File->new();
$file_output->open( "./copy_${file}",'>');

foreach $line (@lines)
{
	if ($line =~ m/^veh/)
	{
		# veh0#1632402449#48#3#36.708130#-6.114507#54#231#72#-858993460-858993460
		@data=();
		@data = split /#/, $line;
		$data[9] = $RECORRIDO;


		print $file_output "$data[0]#$data[1]#$data[2]#$data[3]#$data[4]#$data[5]#$data[6]#$data[7]#$data[8]#$data[9]\n";
	}
}

close ($file_output);




