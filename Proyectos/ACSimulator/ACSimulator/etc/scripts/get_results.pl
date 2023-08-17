#!/usr/bin/perl.exe
#

#================================================================
# Fichero perf_files_gen
#================================================================
#  FICHERO: perf_files_gen for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-04-2021
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

@files=`ls 1000/trip_*`;
chop(@files);

foreach $file (@files)
{
	#trip_0_19_117119.txt
	($dum, $veh, $dum, $trip_code)=split/_/,$file;
	$trip_code=~s/\.txt//g;
	$hash_files{$trip_code}{$veh}=1;
}



foreach $trip_code (sort keys %hash_files)
{
	foreach $veh (sort keys %{$hash_files{$trip_code}})
	{
		#print "$trip_code:[$hash_files{$veh}{$trip_code}]\n";

		$hash_trip{$trip_code}++;
	}
}


foreach $trip_code (sort keys %hash_trip)
{
	print "$trip_code:[$hash_trip{$trip_code}]\n";
}


