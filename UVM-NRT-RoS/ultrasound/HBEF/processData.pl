#!/usr/bin/perl

#Snowcloud Project
#Coded By: Jonathan Russo

#This script will pull the raw data from the database, process it and put it back into a new table in the database.

use strict;
use List::Util qw(sum);
use DBI;
use DBD::mysql;

#get functions
require 'functions.pl';

#connect to database
my $dbh= connectToDatabase('snowcloud');

#initialize variables
my %epochTemps;
my %epochSonar;
my %epochVoltage;
my %epochSoil;
my %epochPar;

##################################
#Pull neccesary data from database
##################################

#TEMP

#get temp vals from database
my $query = "SELECT epoch.node, epoch.epoch, rawData.data FROM rawData, epoch WHERE rawData.sensor= 5 AND rawData.epochID=epoch.epochID ORDER BY epoch.node, epoch.epoch";
my $sth = $dbh->prepare($query);
$sth->execute();

my $node;
my $data;
my $epoch;
$sth->bind_columns(undef, \$node, \$epoch, \$data);

while($sth->fetch()){
	if(!$epochTemps{$node}{$epoch}){
		$epochTemps{$node}{$epoch}=();
		push @{$epochTemps{$node}{$epoch}}, $data;
	}else{
		push @{$epochTemps{$node}{$epoch}}, $data;
	}
}

#average temps in each epoch
for(my $n=0; $n<=3; $n++){
	next if $n=~/1/;
#	print "Node: $n\n";
	foreach my $epoch (sort {$a<=>$b} keys %{$epochTemps{$n}}){
#		print "Epoch:$epoch\n";
		foreach my $vals (@{$epochTemps{$n}{$epoch}}){
#			print "temp:$vals\n";
		}
		$epochTemps{$n}{$epoch}=average(\@{$epochTemps{$n}{$epoch}});
#		print "average: " . $epochTemps{$n}{$epoch} . "\n";

		#get Epoch ID for this epoch
		my $epochID = getEpochId($n, $epoch, $dbh);

		#insert into DB
		my $convertedTemps = convert_temp_c($epochTemps{$n}{$epoch});
		my $query ="INSERT INTO liveProcessedData (epochID, implementationID,
data, transformID) VALUES ($epochID, 5, $convertedTemps, 3)";
		my $sth = $dbh->prepare($query);
		$sth->execute();
	}
}

#Sonar Data

#get from database
$query = "SELECT epoch.node, epoch.epoch, rawData.data FROM rawData, epoch WHERE rawData.sensor= 0 AND rawData.epochID=epoch.epochID ORDER BY epoch.node, epoch.epoch";
my $sth = $dbh->prepare($query);
$sth->execute();

my $node;
my $data;
my $epoch;
$sth->bind_columns(undef, \$node, \$epoch, \$data);

while($sth->fetch()){
	if(!$epochSonar{$node}{$epoch}){
		$epochSonar{$node}{$epoch}=();
		push @{$epochSonar{$node}{$epoch}}, $data;
	}else{
		push @{$epochSonar{$node}{$epoch}}, $data;
	}
}

#process sonar data
for(my $n=0; $n<=3; $n++){
	next if $n=~/1/;
#	print "Node: $n\n";
	foreach my $epoch (sort {$a<=>$b} keys %{$epochSonar{$n}}){
#		print "Epoch: $epoch\n";
		my $ans=&meters_to_inches(&processSnowDepth($n, $epochTemps{$n}{$epoch}, &findMedian(\@{$epochSonar{$n}{$epoch}})));

		#print $ans . "\n";
		if($ans !=0){
			my $epochID = getEpochId($n, $epoch, $dbh);

			#insert into DB
			my $query ="INSERT INTO liveProcessedData (epochID,
implementationID, data, transformID) VALUES ($epochID, 0, $ans, 2)";
			my $sth = $dbh->prepare($query);
			$sth->execute();
		}
	}
}

#Voltage Data

#get from database
$query = "SELECT epoch.node, epoch.epoch, rawData.data FROM rawData, epoch WHERE rawData.sensor= 6 AND rawData.epochID=epoch.epochID ORDER BY epoch.node, epoch.epoch";
my $sth = $dbh->prepare($query);
$sth->execute();

my $node;
my $data;
my $epoch;
$sth->bind_columns(undef, \$node, \$epoch, \$data);

while($sth->fetch()){
	if(!$epochVoltage{$node}{$epoch}){
		$epochVoltage{$node}{$epoch}=();
		push @{$epochVoltage{$node}{$epoch}}, $data;
	}else{
		push @{$epochVoltage{$node}{$epoch}}, $data;
	}
}

#process voltage data
for(my $n=0; $n<=3; $n++){
	next if $n=~/1/;
#	print "Node: $n\n";
	foreach my $epoch (sort {$a<=>$b} keys %{$epochVoltage{$n}}){
#		print "Epoch: $epoch\n";
		my $ans= &processVoltage($n, &findMedian(\@{$epochVoltage{$n}{$epoch}}));
		if($ans !=0){
			my $epochID = getEpochId($n, $epoch, $dbh);

			#insert into DB
			my $query ="INSERT INTO liveProcessedData (epochID,
implementationID, data, transformID) VALUES ($epochID, 6, $ans, 1)";
			my $sth = $dbh->prepare($query);
			$sth->execute();
		}
	}
}

#Soil Data

#get from database
$query = "SELECT epoch.node, epoch.epoch, rawData.data, rawData.sensor FROM rawData, epoch WHERE (rawData.sensor= 3 OR rawData.sensor=4) AND rawData.epochID=epoch.epochID ORDER BY epoch.node, epoch.epoch";
my $sth = $dbh->prepare($query);
$sth->execute();

my $node;
my $data;
my $epoch;
my $sensor;
$sth->bind_columns(undef, \$node, \$epoch, \$data, \$sensor);

while($sth->fetch()){
	if(!$epochSoil{$node}{$epoch}{$sensor}){
		$epochSoil{$node}{$epoch}{$sensor}=();
		push @{$epochSoil{$node}{$epoch}{$sensor}}, $data;
	}else{
		push @{$epochSoil{$node}{$epoch}{$sensor}}, $data;
	}
}

#Process Soil Data
for(my $n=4; $n<=6; $n++){
	next if $n=~/1/;
#	print "Node: $n\n";
	foreach my $epoch (sort {$a<=>$b} keys %{$epochSoil{$n}}){
#		print "Epoch: $epoch\n";
		foreach my $sensorID (sort {$a<=>$b} keys %{$epochSoil{$n}{$epoch}}){
		my $ans= &processSoil(&findMedian(\@{$epochSoil{$n}{$epoch}{$sensorID}}));
			if($ans !=0){
				my $epochID = getEpochId($n, $epoch, $dbh);

				#insert into DB
				my $query ="INSERT INTO liveProcessedData (epochID,
implementationID, data, transformID) VALUES ($epochID, $sensorID, $ans, 4)";
				my $sth = $dbh->prepare($query);
				$sth->execute();
			}
		}
	}
}

#Par Data

#get from database
$query = "SELECT epoch.node, epoch.epoch, rawData.data, rawData.sensor FROM rawData, epoch WHERE (rawData.sensor= 1 OR rawData.sensor=2) AND rawData.epochID=epoch.epochID ORDER BY epoch.node, epoch.epoch";
my $sth = $dbh->prepare($query);
$sth->execute();

my $node;
my $data;
my $epoch;
my $sensor;
$sth->bind_columns(undef, \$node, \$epoch, \$data, \$sensor);

while($sth->fetch()){
	if(!$epochPar{$node}{$epoch}{$sensor}){
		$epochPar{$node}{$epoch}{$sensor}=();
		push @{$epochPar{$node}{$epoch}{$sensor}}, $data;
	}else{
		push @{$epochPar{$node}{$epoch}{$sensor}}, $data;
	}
}

#process par data
for(my $n=0; $n<=6; $n++){
	next if $n=~/1/;
#	print "Node: $n\n";
	foreach my $epoch (sort {$a<=>$b} keys %{$epochPar{$n}}){
#		print "Epoch: $epoch\n";
		foreach my $sensorID (sort {$a<=>$b} keys %{$epochPar{$n}{$epoch}}){
			my $ans= &processPar(&findMedian(\@{$epochPar{$n}{$epoch}{$sensorID}}));
			if($ans !=0){
				my $epochID = getEpochId($n, $epoch, $dbh);

				#insert into DB
				my $query ="INSERT INTO liveProcessedData (epochID,
implementationID, data, transformID) VALUES ($epochID, $sensorID, $ans, 5)";

				my $sth = $dbh->prepare($query);
				$sth->execute();
			}
		}
	}
}
