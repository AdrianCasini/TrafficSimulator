#!/usr/bin/perl.exe
#================================================================
# Fichero getnodesfromroute.pl
#================================================================
#  FICHERO: getnodesfromroute.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 20-07-2021
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;
$DIR_INIT="";
$file = "@ARGV[0]";

if (@ARGV[0] eq "-f")
{
	$file_in = @ARGV[1];
	open (FILE, "< @ARGV[1]");
	($dir, $dum)= split/:/,@ARGV[1];

	$DIR_INIT="${dir}:/TrafficSimulator/Etc/generated_files/route";
}
else
{
	open (FILE, "< ./$file");
	$DIR_INIT=".";
}

if (@ARGV[2] eq "-n")
{

print "jadgjsagdjdjdgjsadgdsdjhsa\n";
	$file_name = @ARGV[3];
}
else
{
	$file_name = "xx";
}

@lines = <FILE>; chop (@lines);close (FILE);

@items=();
foreach $line (@lines){	(@item)=split/"/,$line;}

$str_nodes="";
$seek_nodes=0;
for ($i = 0; $i < scalar(@item); $i++)
{
	if ($item[$i] =~ m/nodes/)
	{
		$seek_nodes=1;
		$str_nodes = join (",", $str_nodes, $item[$i+1]);
	}
	elsif (($item[$i] =~ m/legs/) && ($seek_nodes == 1))
	{
		$i = 1000000;;
	}
}

@nodes=();@nodes = split/,/,$str_nodes;

`rm $DIR_INIT/route_nodes*`;
open (FILE_OUT, "> $DIR_INIT/route_nodes_${file_name}.txt");
foreach $node (@nodes)
{
	$node =~ s/\]|\[|://g;
	print FILE_OUT "$node\n";
}
close(FILE_OUT);

