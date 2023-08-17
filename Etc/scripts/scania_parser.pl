#!/usr/bin/perl.exe

#================================================================
# Fichero scania_parser.pl
#================================================================
#  FICHERO: scania_parser.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 07-09-2021
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

$DRIVER="";
$DRIVER_OLD="--";
$START=1;
$INDEX_VEH=1;
$DISTANCE=0;

foreach $line (@lines)
{

	if ($line =~ m/^2021/)
	{

		@data=();
		@data = split /;/, $line;
		$DRIVER = $data[2];
		$DATE   = $data[0];
		$LAT    = $data[7];
		$LON    = $data[8];
		$DIST_RAW = $data[1];
		$SPEED  = $data[6];

		$LAT    =~ s/,/./g;
		$LON    =~ s/,/./g;

		
		if ($START)
		{
			$DRIVER_OLD = $DRIVER;
			$START=0;
			$DISTANCE_INIT = $DIST_RAW;
		}

		if  ($DRIVER ne $DRIVER_OLD)
		{
			print "\n\n\n\n--XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX--\n\n\n";
			@lines=();
			break;
		}

		$DIST = - ($DIST_RAW - $DISTANCE_INIT) * 1000;


		#print "--$line--\n";
		print "$DRIVER  --  $DATE -- $LAT - $LON - ($DIST_RAW) $DIST\n";


		$hash_veh{$INDEX_VEH}{"$DATE#$LAT#$LON"}="$DRIVER#$DIST#$SPEED";



	}


}





for ($i = 0; $i < 10000; $i++)
{
	$hash_veh_counter{$i}=0;
	$hash_veh_recorid{$i}=1;
	$hash_veh_line_counter{$i}=0;
}

$index_paq_counter=0;
$index_paq = sprintf("%04d",$index_paq_counter);
foreach $index_veh (reverse sort keys %hash_veh)
{
	#print "--$index_veh--------\n";

	foreach $counter (sort keys %{$hash_veh{$index_veh}})
	{
		#print "\t--$counter--------\n";
	
		my ($dum, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido);
		my ($driver, $distance, $speed) = split /#/, $hash_veh{$index_veh}{$counter};
		$angle = 0;
		$time_segStart = 0;
		$recorrido="XX";
		$time_seg_veh=0;
		my ($speed, @dum) = split / /, $speed;
		


		($date_veh, $lat, $lon) = split /#/, $counter;

		my ($YYMMDD, $hhmmss) = split / /, $date_veh;
		my ($YY, $MM, $DD) = split /-/, $YYMMDD;
		$MM = $MM -1;
		my ($hh, $mm, $ss) = split /:/, $hhmmss;


		$time_epoch = timelocal( $ss, $mm, $hh, $DD, $MM, $YY );
		$time_segStart = timelocal( $ss, $mm, $hh, $DD, $MM, $YY );
		#$time_seg_veh = $time_segStart - $time_segStart_old;
		
		#print "--$time-----$date_veh---\n";


		#########################################################
		#    Second away from the original Simulation
		#########################################################
		#$time_epoch = $time_epoch +  2 * 86400 + 50000;


		#close the file every 6 records
		if ($hash_veh_counter{$index_veh} == 6)
		{
			print {$hash_veh_FILE{$index_veh}} "\n\t]\n}";
			close ($hash_veh_FILE{$index_veh});
			$hash_veh_counter{$index_veh}=0;
			$hash_veh_recorid{$index_veh}=1;
		}

		#open the file
		if ($hash_veh_counter{$index_veh} == 0 )
		{
			$index_paq_counter++;
			$index_paq = sprintf("%04d",$index_paq_counter);
			$hash_veh_FILE{$index_veh} = IO::File->new();
			$hash_veh_FILE{$index_veh}->open( "./Processed/trip_${index_veh}_${index_paq}_${recorrido}.txt",'>');

			#"version": "1.0",
			#"clientReference" : "Driver LT00003 TEST",
			#"psn" : "99999900000003",
			#"vehicleReference" : "VIN_LT00003",
			#"lpn" : "LT00003",
			#"lpCountry" : "E",
			#"transmissionEventId" : "6",
			#"transmissionTime" : "1622529904",
			#"records" :


			$dum       = sprintf("%05d",$index_veh);
			$psn       = join ("", "999999000", "$dum");
			$clref     = join ("", "Driver LT", "$dum");
			$vhref     = join ("", "VIN_LT", "$dum");
			$matricula = join ("", "LT", "$dum");

			print {$hash_veh_FILE{$index_veh}}  "{\n\t\"version\": \"1.0\",\n\t\"clientReference\" : \"$clref\",\n\t\"psn\" : \"$psn\",\n\t\"vehicleReference\" : \"$vhref\",\n\t\"lpn\" : \"$matricula\",\n\t\"lpCountry\" : \"E\",\n\t\"transmissionEventId\" : \"$index_veh\",\n\t\"transmissionTime\" : \"$time_epoch\",\n\t\"records\" :\n\t[\n";
				
			print {$hash_veh_FILE{$index_veh}} "\t\t{\n\t\t\t\"recordId\": \"$hash_veh_recorid{$index_veh}\",\n\t\t\t\"recordTime\" : \"$time_epoch\",\n\t\t\t\"recordingEventId\" : \"$index_veh\",\n\t\t\t\"latitude\" : \"$lat\",\n\t\t\t\"longitude\" : \"$lon\",\n\t\t\t\"stopDuration\" : \"0\",\n\t\t\t\"drivingDuration\" : \"$time_seg_veh\",\n\t\t\t\"drivingDistance\" : \"$distance\",\n\t\t\t\"hdop\" : \"1\",\n\t\t\t\"heading\" : \"$angle\",\n\t\t\t\"speed\" : \"$speed\",\n\t\t\t\"gpsOdometer\" : \"0\"\n\t\t}";
			$hash_veh_counter{$index_veh}++;
			$hash_veh_recorid{$index_veh}++;
		}
		else
		{
			
		
			#     	{
			#     		"recordId": "1",
			#     		"recordTime" : "1618498327",
			#     		"recordingEventId" : "0",
			#     		"latitude" : "41.639358",
			#     		"longitude" : "2.362247",
			#     		"stopDuration" : "0",
			#     		"drivingDuration" : "10",
			#     		"drivingDistance" : "85",
			#     		"hdop" : "1",
			#     		"heading" : "185",
			#     		"speed" : "100",
			#     		"gpsOdometer" : "0"
			#     	}


			print {$hash_veh_FILE{$index_veh}} ",\n\t\t{\n\t\t\t\"recordId\": \"$hash_veh_recorid{$index_veh}\",\n\t\t\t\"recordTime\" : \"$time_epoch\",\n\t\t\t\"recordingEventId\" : \"$index_veh\",\n\t\t\t\"latitude\" : \"$lat\",\n\t\t\t\"longitude\" : \"$lon\",\n\t\t\t\"stopDuration\" : \"0\",\n\t\t\t\"drivingDuration\" : \"$time_seg_veh\",\n\t\t\t\"drivingDistance\" : \"$distance\",\n\t\t\t\"hdop\" : \"1\",\n\t\t\t\"heading\" : \"$angle\",\n\t\t\t\"speed\" : \"$speed\",\n\t\t\t\"gpsOdometer\" : \"0\"\n\t\t}";
			$hash_veh_counter{$index_veh}++;
			$hash_veh_recorid{$index_veh}++;
		}

	}

	#close the file definitively
	print {$hash_veh_FILE{$index_veh}} "\n\t]\n}";
	close ($hash_veh_FILE{$index_veh});

}

