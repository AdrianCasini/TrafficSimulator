#!/usr/bin/perl.exe

use IO::File;
use Time::Local;


open (FILE, "< ./ap7tripskm.csv");
@lines_ap7 = <FILE>; 
chop (@lines_ap7);
close (FILE);

#102111;57;85,6

foreach $line (@lines_ap7)
{
	($est,$dum,$km)= split/;/,$line;
	$hash_tripkm{"$est"}=$km;

}


#Vehiculo:2001;112;106;234;32325;9;333
print "--------\n";


@lines_trips=`cat log_AP7* | grep Vehiculo`;

open (FILE_OUT, "> ./ap7km_excel.csv");


foreach $line (@lines_trips)
{
	#print "$line\n";

	if ($line =~m/Vehiculo/)
	{
        #print "$line\n";

		($veh,$estin, $estout, $dum,$dum,$cant, $dum)= split/;/, $line;
		$veh =~ s/Vehiculo://;

		print FILE_OUT "$veh;$estin;$estout;$cant;$hash_tripkm{\"$estin$estout\"}\n";
	}
	

}
close (FILE_OUT);
