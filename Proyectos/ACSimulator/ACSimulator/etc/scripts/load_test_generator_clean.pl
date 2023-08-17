#!/usr/bin/perl.exe

#================================================================
# Fichero load_test_generator_clean.pl
#================================================================
#  FICHERO: load_test_generator for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 15-09-2021
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================
use IO::File;

$file     = "@ARGV[0]";
$VEH_INIT = "@ARGV[1]";
$LOG_F    = "@ARGV[2]";

$TOTAL_TIME    = 8 * 60 * 60; #segundos
$END_TRIP_TIME = 20 * 60;     #segundos
$TIME_ELAPSED  = 6 * 86400;   #segundos

open (FILE, ">> ./$LOG_F");
$index_veh = $VEH_INIT;
@lines = ();
@lines = `grep "veh$index_veh#" ./$file`;
chop(@lines);
chop(@lines);

$fmt_indx  = sprintf("%04d",$index_veh);
#print "Index:[$fmt_indx]\n";

$COUNT_FILES=0;


my ($dum, $dum, $time_init,  $dum, $dum, $dum, $dum, $dum, $dum,$recorrido) = split /#/, $lines[0];
$last_time = &generate_file($index_veh, 0);
$COUNT_FILES = $COUNT_FILES + $COUNT;

#print FILE "===============================================\n";
#print FILE "$fmt_indx#$recorrido#$last_time\n"; 
@dum=();
@rec=split//, $recorrido;
$est_in = "$rec[0]$rec[1]$rec[2]";
$est_out = "$rec[3]$rec[4]$rec[5]";
$LOOP = 1;

$TIME_LOOP = $last_time - $time_init;
#print FILE "---  $TIME_LOOP\n";
while (($TIME_LOOP) < $TOTAL_TIME )
{

	$time_plus_abs = $END_TRIP_TIME + $last_time;
	$last_time = &generate_file($index_veh, $time_plus_abs);
	$COUNT_FILES = $COUNT_FILES + $COUNT;
	print FILE "$fmt_indx#$recorrido#$last_time\n"; 

	$TIME_LOOP = $last_time - $time_init;
	$LOOP++;

}


#print "Vehiculo:[$index_veh];$est_in;$est_out;Init Time[$time_init];Last Time:[$last_time];LOOP:[$LOOP]\n";
#print FILE "Vehiculo:$index_veh;$est_in;$est_out;$time_init;$last_time;$LOOP;$COUNT_FILES\n";

	

close (FILE);

#-----------------------------------------------------------------------------------------------------------------------
sub generate_file
{
	my ($index_veh, $time_plus_abs)=@_;
	$TIME_END=0;
	my $FILE_VEH;
	
	$VEH_recorid=1;
	$VEH_counter=0;
	$COUNT=0;

	foreach $line (@lines)
	{
		my ($dum, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido) = split /#/,$line;
		#print "[$dum, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido]\n";

		#########################################################
		#    Second away from the original Simulation
		#########################################################
		$time_epoch = $time_epoch +  $TIME_ELAPSED + $time_plus_abs;
		$time_segStart = $time_segStart + $time_plus_abs;

		$TIME_END = $time_segStart;

		#close the file every 6 records
		if ($VEH_counter == 6)
		{
			#print {$FILE_VEH} "\n\t]\n}";
			#close ($FILE_VEH);
			$VEH_counter=0;
			$VEH_recorid=1;
		}

		#open the file
		if ($VEH_counter == 0 )
		{
			#$FILE_VEH = IO::File->new();
			$fmt_indx       = sprintf("%05d",$index_veh);
			#print "--------------------------------------------------------------------------------------------------\n";
			#$FILE_VEH->open( "./Processed/trip_${fmt_indx}_${time_segStart}_${recorrido}.txt",'>');
			print FILE "trip_${fmt_indx}_${time_segStart}_${recorrido}.txt\n";
			$COUNT++;

			#"version": "1.0",
			#"clientReference" : "Driver LT00003 TEST",
			#"psn" : "99999900000003",
			#"vehicleReference" : "VIN_LT00003",
			#"lpn" : "LT00003",
			#"lpCountry" : "E",
			#"transmissionEventId" : "6",
			#"transmissionTime" : "1622529904",
			#"records" :


			#$dum       = sprintf("%05d",$index_veh);
			#$psn       = join ("", "999999000", "$dum");
			#$clref     = join ("", "Driver LT", "$dum");
			#$vhref     = join ("", "VIN_LT", "$dum");
			#$matricula = join ("", "LT", "$dum");

			#print {$FILE_VEH}  "{\n\t\"version\": \"1.0\",\n\t\"clientReference\" : \"$clref\",\n\t\"psn\" : \"$psn\",\n\t\"vehicleReference\" : \"$vhref\",\n\t\"lpn\" : \"$matricula\",\n\t\"lpCountry\" : \"E\",\n\t\"transmissionEventId\" : \"$index_veh\",\n\t\"transmissionTime\" : \"$time_epoch\",\n\t\"records\" :\n\t[\n";
				
			#print {$FILE_VEH} "\t\t{\n\t\t\t\"recordId\": \"$VEH_recorid\",\n\t\t\t\"recordTime\" : \"$time_epoch\",\n\t\t\t\"recordingEventId\" : \"$index_veh\",\n\t\t\t\"latitude\" : \"$lat\",\n\t\t\t\"longitude\" : \"$lon\",\n\t\t\t\"stopDuration\" : \"0\",\n\t\t\t\"drivingDuration\" : \"$time_seg_veh\",\n\t\t\t\"drivingDistance\" : \"$distance\",\n\t\t\t\"hdop\" : \"1\",\n\t\t\t\"heading\" : \"$angle\",\n\t\t\t\"speed\" : \"$speed\",\n\t\t\t\"gpsOdometer\" : \"0\"\n\t\t}";
			$VEH_counter++;
			$VEH_recorid++;
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


			#print {$FILE_VEH} ",\n\t\t{\n\t\t\t\"recordId\": \"$VEH_recorid\",\n\t\t\t\"recordTime\" : \"$time_epoch\",\n\t\t\t\"recordingEventId\" : \"$index_veh\",\n\t\t\t\"latitude\" : \"$lat\",\n\t\t\t\"longitude\" : \"$lon\",\n\t\t\t\"stopDuration\" : \"0\",\n\t\t\t\"drivingDuration\" : \"$time_seg_veh\",\n\t\t\t\"drivingDistance\" : \"$distance\",\n\t\t\t\"hdop\" : \"1\",\n\t\t\t\"heading\" : \"$angle\",\n\t\t\t\"speed\" : \"$speed\",\n\t\t\t\"gpsOdometer\" : \"0\"\n\t\t}";
			$VEH_counter++;
			$VEH_recorid++;
		}

	}

	#close the file definitively
	#print {$FILE_VEH} "\n\t]\n}";
	#close ($FILE_VEH);

	return $TIME_END;
}





