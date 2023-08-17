#!/usr/bin/perl.exe
#


$only_once = 1;
@files = `ls -t`;
chop(@files);
foreach $file (@files)
{
	if ($file =~ m/txt/)
	{
		print "$file\n";
		@lines= ();
		open  (FILE_READ, "< $file");
		@lines = <FILE_READ>;
		chop(@lines);
		close (FILE_READ);

		`rm $file`;

		open  (FILE_WRITE, "+> $file");
		print FILE_WRITE "#---------------------------------------------------------------------------------------------------------\n";

		$only_once = 1;
		foreach $line (@lines)
		{
			$line =~ s/#/;/g;
			if ($line =~ m/ITERATION/)
			{
	
			}
			elsif(($only_once) && ($line =~ m/INFO/))
			{
				#ITERATION#10000
				#INFO#110#T2#830202000200000#169100#0#10
				print FILE_WRITE "TYPE;VEHICLEINFO\n";
				print FILE_WRITE "FORMAT;STR;TYPE;INT;SPEED;STR;CATEGORY;LI;GPS_DEVICE_ID;INT;TIME_FOLLOWING;INT;GPS_INNACURACY;INT;GPS_POINT_TIMER\n";
				print FILE_WRITE "#---------------------------------------------------------------------------------------------------------\n";
				print FILE_WRITE "$line\n";
				$only_once = 0;

			}
			elsif(($only_once) && ($line =~ m/DB/))
			{
				#ITERATION#10000
				#DB#110#103#1801800
				print FILE_WRITE "TYPE;VEHICLEINFO\n";
				print FILE_WRITE "FORMAT;STR;TYPE;INT;EST_IN;INT;EST_OUT;LI;INT;DB_MOMENT\n";
				print FILE_WRITE "#---------------------------------------------------------------------------------------------------------\n";
				print FILE_WRITE "$line\n";
				$only_once = 0;
			}
			else
			{
				print FILE_WRITE "$line\n";
			}

		
		}

		close (FILE_WRITE);


	}


}