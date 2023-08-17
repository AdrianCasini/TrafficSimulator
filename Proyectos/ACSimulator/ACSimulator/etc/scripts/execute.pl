#!/usr/bin/perl.exe
#
#================================================================
# Fichero execute.pl
#================================================================
#  FICHERO: execute.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 22-04-2022
#  AUTOR DISENYO: Another perl masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================


$exe = "@ARGV[0]";

#print "Executing [$exe]\n";
`$exe >>/dev/null &`;


