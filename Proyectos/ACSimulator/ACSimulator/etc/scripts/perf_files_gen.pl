#!/usr/bin/perl.exe

#================================================================
# Fichero perf_files_gen
#================================================================
#  FICHERO: perf_files_gen for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-04-2021
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

$file = "@ARGV[0]";
open (FILE, "< ./$file");
@lines = <FILE>; 
chop (@lines);
close (FILE);
$TOTAL_VEH=6000;

%hash_veh = ();

for ($i = 0; $i < $TOTAL_VEH; $i++)
{
	$hash_veh_counter{$i}=0;
	$hash_veh_recorid{$i}=1;
	$hash_veh_line_counter{$i}=0;
}


foreach $line (@lines)
{
	chop($line);
	($index_veh, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido) = split /#/, $line;
	$index_veh =~ s/veh//g;

	$index_veh = $index_veh + 1;

	$counter= sprintf("%04d",$hash_veh_line_counter{$index_veh});
	$hash_veh{$index_veh}{$counter}=$line;
	$hash_veh_line_counter{$index_veh}++;
}

foreach $index_veh (sort keys %hash_veh)
{
	print "--$index_veh--------\n";
	if ($index_veh < $TOTAL_VEH)
	{
		foreach $counter (sort keys %{$hash_veh{$index_veh}})
		{
			print "\t--$counter--------\n";
	
			my ($dum, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido) = split /#/, $hash_veh{$index_veh}{$counter};


			#########################################################
			#    Second away from the original Simulation
			#########################################################
			$time_epoch = $time_epoch + 4 * 86400 - 20000;


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
				$hash_veh_FILE{$index_veh} = IO::File->new();
				$hash_veh_FILE{$index_veh}->open( "./Processed/trip_${index_veh}_${time_segStart}_${recorrido}.txt",'>');

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
	}# (< $TOTAL_VEH)
}






