#!/usr/bin/perl.exe

open (FILE, "> ways");
@files = `ls ../ways_bines/`;

foreach $file (@files)
{
    $file_clean = $file;
    $file_clean =~ s/way_//;
    #print "$file_clean\n";
    print FILE "$file_clean\n";
}
close (FILE);

