#!/usr/bin/perl
#Written By: Jonathan Russo

#This script contains a variety of functions that are used by the various number of scripts in this directory


my %coefficients;
$coefficients{'snowDepth'}{1}{'a'}=0.0000018317;
$coefficients{'snowDepth'}{1}{'b'}=-0.0000407295;
$coefficients{'snowDepth'}{2}{'a'}=0.0000017689;
$coefficients{'snowDepth'}{2}{'b'}=0.0000652089;
$coefficients{'snowDepth'}{3}{'a'}=0.000001798;
$coefficients{'snowDepth'}{3}{'b'}=0.0000246876;
$coefficients{'voltage'}{1}{'a'}=0.00489;
$coefficients{'voltage'}{1}{'b'}=-4.58267;
$coefficients{'voltage'}{2}{'a'}=0.00489;
$coefficients{'voltage'}{2}{'b'}=-4.58267;
$coefficients{'voltage'}{3}{'a'}=0.00489;
$coefficients{'voltage'}{3}{'b'}=-4.58267;
$coefficients{'soil'}{'a'}=0.0005;
$coefficients{'soil'}{'b'}=-0.2663;
$coefficients{'par'}{'a'}=-0.7;
$coefficients{'par'}{'b'}=.0066;

my %dis2Ground;
$dis2Ground{1}=2.01168;
$dis2Ground{2}=1.70688;
$dis2Ground{3}=1.70688;

#sub connectToDatabase{
#	my $database = shift;
#	my $platform = "mysql";
#	my $host = "kaladin.cems.uvm.edu";
#	my $port = "3306";
#	my $user = "snowcloud_admin";
#	my $pw = "Yq3:1GL8";
#
#	my $dbh = DBI->connect("DBI:$platform:$database:$port;host=$host", $user, $pw) || die "Could not connect to database: $DBI::errstr";
#	return $dbh;
#}
sub connectToDatabase{
	my $database = shift;
	my $platform = "mysql";
	my $host = "127.0.0.1";
	my $port = "9999";
	my $user = "snowcloud_admin";
	my $pw = "Yq3:1GL8";

	my $dbh = DBI->connect("DBI:$platform:$database:$port;host=$host", $user, $pw) || die "Could not connect to database: $DBI::errstr";
	return $dbh;
}

sub getEpochId{
	my $n = shift;
	my $e = shift;
	my $dbh =shift;

	my $query = "SELECT epochID, time FROM HBEF_epoch WHERE node = $n AND epoch = $e";
	my $sth = $dbh->prepare($query);
	$sth->execute();

	my $epochID;
	my $time;
	$sth->bind_columns(undef, \$epochID, \$time);

	$sth->fetch();
	print "node: $n \nepoch: $e \nepochId: $epochID \nTime: $time \n";
	my @array = ($epochID, $time);
	return @array;
}

sub findMedian{
	@_ == 1 or die ('Sub usage: $median = median(\@array);');
	my ($array_ref) = @_;
	my $count = scalar @$array_ref;

	# Sort a COPY of the array, leaving the original untouched
	my @array = sort { $a <=> $b } @$array_ref;
	if ($count % 2) {
		return $array[int($count/2)];
	} else {
		return ($array[$count/2] + $array[$count/2 - 1]) / 2;
	}
}

sub average
{
    my $arrayRef = shift;
    my @array =@$arrayRef;
    my	$total = @array;
    return (sum(@array)/$total);
}

sub convert_temp_c
{
    my $temp = shift;
    return -40 + .01 * $temp;
}

sub speed_of_sound{
    my $temp = shift;
    return 331.3 * (sqrt (1 + ($temp / 273.15)));
}

sub meters_to_inches{
    my $meters = shift;
    return $meters * 39.3700787;
}

sub processVoltage{
	my $node = shift;
	my $medADC = shift;

	my $ans=($coefficients{'voltage'}{$node}{'a'}*$medADC)+$coefficients{'voltage'}{$node}{'b'};
	return $ans;
}

sub processSoil{
	my $medADC =shift;

	my $ans=($coefficients{'soil'}{'a'}*$medADC)+$coefficients{'soil'}{'b'};
	return $ans;
}

sub processPar{
	my $medADC=shift;

	my $ans=($medADC-$coefficients{'par'}{'a'})/$coefficients{'par'}{'b'};
	return $ans;
}

#sub processSnowDepth{
#	my $node = shift;
#	my $temp= shift;
#	my $medADC = shift;
#
#	my $equationAns = ($coefficients{'snowDepth'}{$node}{'a'}*$medADC)+$coefficients{'snowDepth'}{$node}{'b'};
#
#	my $timeOfFlight = $equationAns * &speed_of_sound(convert_temp_c($temp));
#
#	return $dis2Ground{$node} - $timeOfFlight;
#}
sub processSnowDepth{
	my $node = shift;
	my $temp= shift;
	my $medADC = shift;

	my $equationAns = ($coefficients{'snowDepth'}{$node}{'a'}*$medADC)+$coefficients{'snowDepth'}{$node}{'b'};

	my $timeOfFlight = $equationAns * &speed_of_sound($temp);	
	
	return $dis2Ground{$node} - $timeOfFlight; 
}

1;
