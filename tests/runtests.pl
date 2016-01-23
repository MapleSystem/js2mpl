#!/usr/bin/perl -w
use Cwd;
my $pwd = getcwd;

if(!(defined $ARGV[0])) {
  print "------------------------------------------------\n";
  print "usage: runtests.pl test_dir [pattern]\n";
  print "to test all *closure*.js in regression-tests:\n";
  print "       runtests.pl regression-tests closure\n";
  print "------------------------------------------------\n";
  exit;
}

$dirname = "./$ARGV[0]";
my $tempdir = "$pwd/output";
if(!(-e $tempdir)) {
  mkdir $tempdir;
}

my @failed_mpl_file;
my @failed_mmpl_file;
my @failed_gencmpl_file;
my @failed_printcmpl_file;
my @failed_jsvm_cmpl_file;
my @failed_gencmpl_v2_file;
my @failed_printcmpl_v2_file;
my @failed_jsvm_cmpl_v2_file;
my @failed_int_file;
my @successed_file;
chdir $dirname;
$dirname = "./";
opendir (DIR, $dirname ) || die "Error in opening dir $dirname\n";
print("\n====================== run tests =====================\n");
my @count = 0;
my @countMPL = 0;
my @countMMPL = 0;
my @countgenCMPL = 0;
my @countprintCMPL = 0;
my @countrunCMPL = 0;
my @countgenCMPLv2 = 0;
my @countprintCMPLv2 = 0;
my @countrunCMPLv2 = 0;
my @countINT = 0;
while( ($filename = readdir(DIR))){
  if(-d $filename and $filename ne ".." and $filename ne "output" and $filename ne "temp") {
    my $predir = getcwd;
    chdir $filename;
    my @files = <*.js>;
    if(defined $ARGV[1]) {
      @files = <*$ARGV[1]*.js>;
    }

    foreach $fullname (@files) {
      $count ++;
      (my $file = $fullname) =~ s/\.[^.]+$//;
      print ".";
      if ($count % 50 == 0) {
        print " $count\n";
      }
      if(defined $ARGV[1]) {
        print "$file\n";
      }
      my $flag = 0;
      my $js_file = $file.'.js';
      my $mpl_file = $file.'.mpl';
      my $mmpl_file = $file.".mmpl";
      my $cmpl_file = $file.".cmpl";
      my $log_file = $file.'.log';
      my $out_file = $file.'.out';
      my $err_file = $file.'.err';
      #my $option = $ARGV[0];
      my $option = 0;
      system("cp $js_file $tempdir/$js_file");
      my $res = system("$pwd/../build/js2mpl $tempdir/$js_file $option > $tempdir/$log_file");
      if ($res > 0) {
        print " ==js2mpl===> $file\n";
        $countMPL ++;
        push(@failed_mpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplebe/be/mplbe $tempdir/$mpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==mplbe===> $file\n";
        $countMMPL ++;
        push(@failed_mmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/cmpl-v1/gencmpl-v1 $tempdir/$mmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==gencmpl-v1===> $file\n";
        $countgenCMPL ++;
        push(@failed_gencmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/cmpl-v1/printcmpl-v1 $tempdir/$cmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==printcmpl-v1===> $file\n";
        $countprintCMPL ++;
        push(@failed_printcmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/compact/jsvm-cmpl-v1 $tempdir/$cmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==jsvm-cmpl-v1===> $file\n";
        $countrunCMPL ++;
        push(@failed_jsvm_cmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/cmpl/gencmpl $tempdir/$mmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==gencmpl-v2===> $file\n";
        $countgenCMPLv2 ++;
        push(@failed_gencmpl_v2_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/cmpl/printcmpl $tempdir/$cmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==printcmpl-v2===> $file\n";
        $countprintCMPLv2 ++;
        push(@failed_printcmpl_v2_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/interpreter32 $tempdir/$mmpl_file > $tempdir/$out_file");
      system("grep failed $tempdir/$out_file > $tempdir/$err_file");
      my $errfile = "$tempdir/$err_file";
      my $size = -s $errfile;
      #print "res=$res\n";
      #print "size=$size\n";
      if ($res == 0 && $size > 0) {
        print "\n!!warning: string \"failed\" is emited from interpreter. please check the test case $js_file.\n";
      }
      if ($res > 0) {
        print " ==interpreter32===> $file\n";
        $countINT ++;
        push(@failed_int_file, $file);
        $flag ++;
        next;
      }
      if ($flag eq 0) {
        push(@successed_file, $file);
        system("rm -f $tempdir/$js_file");
        system("rm -f $tempdir/$mpl_file");
        system("rm -f $tempdir/$mmpl_file");
        system("rm -f $tempdir/$cmpl_file");
        system("rm -f $tempdir/$log_file");
        system("rm -f $tempdir/$out_file");
        system("rm -f $tempdir/$err_file");
        next;
      }
    }
    chdir $predir;
  }
}
closedir(DIR);

if ((scalar(@failed_mpl_file) + scalar(@failed_mmpl_file) + scalar(@failed_int_file) +
     scalar(@failed_gencmpl_file) + scalar(@failed_printcmpl_file) + scalar(@failed_jsvm_cmpl_file)) eq 0) {
  print("\n all $count tests passed\n");
  print("======================================================\n");
} else {
  my $countFailed = $countMPL + $countMMPL + $countINT + $countrunCMPL;
  my $countPassed = $count - $countFailed;
  if(scalar(@successed_file) > 0) {
    print "\n=========================\npassed $countPassed tests:\n\n";
    foreach $successed (@successed_file) {
      print $successed."\n";
    }
    print "=========================\n";
  }
  print "\n=========================\nfailed $countFailed tests:\n\n";
  if(scalar(@failed_mpl_file) > 0){
    print("=== failed generation of maple IR: $countMPL tests\n");
    foreach $failed (@failed_mpl_file) {
      print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_mmpl_file) > 0){
    print("=== failed generation of machine maple IR: $countMMPL tests\n");
    foreach $failed (@failed_mmpl_file) {
      print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_int_file) > 0){
    print("=== failed interpretation: $countINT tests\n");
    foreach $failed (@failed_int_file) {
      print $failed."\n";
    }
  }
  if(scalar(@failed_gencmpl_file) > 0) {
    print("=== failed gen-compact maple IR: $countgenCMPL tests\n");
    foreach $failed (@failed_gencmpl_file) {
      print $failed."\n";
    }
  }
  if(scalar(@failed_printcmpl_file) > 0) {
    print("=== failed print-compact maple IR: $countprintCMPL tests\n");
    foreach $failed (@failed_printcmpl_file) {
      print $failed."\n";
    }
  }
  if(scalar(@failed_jsvm_cmpl_file) > 0) {
    print("=== failed interprete-compact maple IR: $countrunCMPL tests\n");
    foreach $failed (@failed_jsvm_cmpl_file) {
      print $failed."\n";
    }
    print "\n";
  }
  print "=========================\n";
}
