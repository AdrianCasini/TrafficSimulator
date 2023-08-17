#!/usr/bin/perl.exe

#================================================================
# Fichero veh_converter.pl
#================================================================
#  FICHERO: veh_converter.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 14-10-2021
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

$VEH_DELTA = "@ARGV[1]";

$file_output = IO::File->new();
$file_output->open( "./copy_${file}",'>');

foreach $line (@lines)
{
	if ($line =~ m/^veh/)
	{
		# veh0#1632402449#48#3#36.708130#-6.114507#54#231#72#-858993460-858993460
		@data=();
		@data = split /#/, $line;
		$veh_temp = $data[0];
		$veh_temp =~ s/veh//g;
		$veh_temp = $veh_temp + $VEH_DELTA;
		$veh_temp = "veh$veh_temp";

		print $file_output "$veh_temp#$data[1]#$data[2]#$data[3]#$data[4]#$data[5]#$data[6]#$data[7]#$data[8]#$data[9]\n";
	}
}

close ($file_output);




