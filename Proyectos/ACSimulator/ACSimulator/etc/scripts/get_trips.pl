#!/usr/bin/perl.exe

#================================================================
# Fichero get_trips.pl
#================================================================
#  FICHERO: get_trips for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 14-10-2021
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

$file = "@ARGV[0]";
open (FILE, "< ./$file");
@lines = <FILE>; 
chop (@lines);
close (FILE);


%hash_veh = ();


#IN  ==> [6833] Indice Gral: [6833]  Mnt:[1801031] Est Ent:[105] Est Sal:[112] Vel:[120] Ctg:[T2]
#IN  ==> [6834] Indice Gral: [6834]  Mnt:[1801031] Est Ent:[102] Est Sal:[109] Vel:[120] Ctg:[T2]
#IN  ==> [6835] Indice Gral: [6835]  Mnt:[1801031] Est Ent:[105] Est Sal:[112] Vel:[120] Ctg:[T2]

$FILE_OUT = IO::File->new();
$FILE_OUT->open( "AP7_trips.csv",'>');
foreach $line (@lines)
{
	if ($line =~ m/IN  ==>/)
	{
		my ($driver, $dum, $dum, $est_ent, $est_sal, @dum) = split /:/, $line;
		$est_ent =~ s/\s+//g;
		$est_ent =~ s/(\[|\]|EstSal)//g;

		$est_sal =~ s/\s+//g;
		$est_sal =~ s/(\[|\]|Vel)//g;

		$driver =~ s/\s+//g;
		$driver =~ s/(\[|\]|IndiceGral|IN==>)//g;
		

		print $FILE_OUT "$driver;$est_ent;$est_sal\n";

		$hash_cant{"$est_ent$est_sal"}++;

	}
}
close ($FILE_OUT);

$FILE_OUT_2 = IO::File->new();
$FILE_OUT_2->open( "AP7_trips_cant.csv",'>');
foreach $est (keys %hash_cant)
{
	print $FILE_OUT_2 "$est;$hash_cant{$est}\n";
}
close ($FILE_OUT_2);