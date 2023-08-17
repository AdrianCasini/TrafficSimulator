#!/usr/bin/perl.exe

#================================================================
# Fichero simulation.pl
#================================================================
#  FICHERO: simulation.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 19-10-2021
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================


@lines = `cat log_SIM*`; 
chop (@lines);

foreach $line (@lines)
{
	#trip_07901_1185_Andalucia_4.txt
	if ($line =~ m/^trip_/)
	{
		@dum=();
		@dum=split/_/, $line;
		$fmt_seg  = sprintf("%08d",$dum[2]);
		$hash_sim{$fmt_seg}++;

	}
}

open (FILE, "> ./simulacion.csv");
foreach $var (sort keys %hash_sim)
{
	print FILE "$var;$hash_sim{$var}\n";
}
close (FILE);