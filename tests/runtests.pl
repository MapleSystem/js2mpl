#!/usr/bin/perl -w
use Cwd;
my $pwd = getcwd;
my $REPORT_DIR="$pwd/../../dex2mpl/tests/";

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
#my $plugindir = "$pwd/output/required";
my $plugindir = "$pwd/output/required";
if(!(-e $plugindir)) {
  mkdir $plugindir;
}

my @failed_mpl_file;
my @failed_mplme_file;
my @failed_mmpl_file;
my @failed_gencmpl_file;
my @failed_printcmpl_file;
my @failed_jsvm_cmpl_file;
my @failed_int_file;
my @successed_file;
chdir $dirname;
$dirname = "./";
opendir (DIR, $dirname ) || die "Error in opening dir $dirname\n";
print("\n====================== run tests =====================\n");
my @count = 0;
my @countMPL = 0;
my @countIPA = 0;
my @countME = 0;
my @countMMPL = 0;
my @countgenCMPL = 0;
my @countprintCMPL = 0;
my @countrunCMPL = 0;
my @countINT = 0;

# process plugin files first
@required = ("plugin/required");
foreach $srcdir (@required) {
  if(-d $srcdir) {
    my $predir = getcwd;
    chdir $srcdir;
    my @files = <*.js>;

    foreach $fullname (@files) {
      $count ++;
      (my $file = $fullname) =~ s/\.[^.]+$//;
      if(defined $ARGV[3]) {
        print "\n$file";
      } else {
        print ".";
      }
      if ($count % 50 == 0) {
        print " $count\n";
      }
      my $flag = 0;
      my $js_file = $file.'.js';
      my $mpl_file = $file.'.mpl';
      my $origmpl_file = $file.'.orig.mpl';
      my $ipampl_file = $file.'.inline.mpl';
#my $optmpl_file = $file.'.hdse.mpl';
      my $optmpl_file = $file.'.ssapre.mpl';
      my $mmpl_file = $file.".mmpl";
      my $cmpl_file = $file.".cmpl";
      my $log_file = $file.'.log';
      my $out_file = $file.'.out';
      my $err_file = $file.'.err';
      my $opt = 0;
      if(defined $ARGV[1]) {
        $opt = $ARGV[1];
      }
      my $ipa = 0;
      if(defined $ARGV[2]) {
        $ipa = $ARGV[2];
      }
      my $option = -plugin;
      system("cp $js_file $plugindir/$js_file");
      my $res = system("$pwd/../build/gnu/js2mpl $plugindir/$js_file $option > $plugindir/$log_file");
      if ($res > 0) {
        print " ==js2mpl===> $file\n";
        $countMPL ++;
        push(@failed_mpl_file, $file);
        $flag ++;
        next;
      }
      system("cp $plugindir/$mpl_file $plugindir/$origmpl_file");
      $res = system("$pwd/../../mapleall/build/gnu/mapleipa/mplipa $plugindir/$mpl_file >> $plugindir/$log_file");
      if ($res > 0) {
        print " ==mplipa===> $file\n";
        $countIPA ++;
        push(@failed_mplipa_file, $file);
        $flag ++;
        next;
      }
      if ($ipa != 0) {
        system("cp $plugindir/$ipampl_file $plugindir/$mpl_file");
      }
      $res = system("$pwd/../../mapleall/build/gnu/mapleme/mplme $plugindir/$mpl_file >> $plugindir/$log_file");
      if ($res > 0) {
        print " ==mplme===> $file\n";
        $countME ++;
        push(@failed_mplme_file, $file);
        $flag ++;
        next;
      }
      if ($opt & 1) {
        system("cp $plugindir/$optmpl_file $plugindir/$mpl_file");
      }
      $res = system("$pwd/../../mapleall/build/gnu/maplebe/be/mplbe $plugindir/$mpl_file >> $plugindir/$log_file");
      if ($res > 0) {
        print " ==mplbe===> $file\n";
        $countMMPL ++;
        push(@failed_mmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/gnu/maplebe/be/mplbe-c $plugindir/$mpl_file >> $plugindir/$log_file");
      if ($res > 0) {
        print " ==mmpl2cmpl===> $file\n";
        $countgenCMPL ++;
        push(@failed_gencmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/gnu/mapleir/cmpl2mmpl $plugindir/$cmpl_file >> $plugindir/$log_file");
      if ($res > 0) {
        print " ==cmpl2mmpl===> $file\n";
        $countprintCMPL ++;
        push(@failed_printcmpl_file, $file);
        $flag ++;
        next;
      }
    }
    chdir $predir;
  }
}

while( ($srcdir = readdir(DIR))){
  if(-d $srcdir and $srcdir ne ".." and $srcdir ne "output" and $srcdir ne "temp") {
    my $predir = getcwd;
    chdir $srcdir;
    my @files = <*.js>;

    foreach $fullname (@files) {
      $count ++;
      (my $file = $fullname) =~ s/\.[^.]+$//;
      if(defined $ARGV[3]) {
        print "\n$file";
      } else {
        print ".";
      }
      if ($count % 50 == 0) {
        print " $count\n";
      }
      my $flag = 0;
      my $js_file = $file.'.js';
      my $mpl_file = $file.'.mpl';
      my $origmpl_file = $file.'.orig.mpl';
      my $ipampl_file = $file.'.inline.mpl';
#my $optmpl_file = $file.'.hdse.mpl';
      my $optmpl_file = $file.'.ssapre.mpl';
      my $mmpl_file = $file.".mmpl";
      my $cmpl_file = $file.".cmpl";
      my $log_file = $file.'.log';
      my $out_file = $file.'.out';
      my $err_file = $file.'.err';
      my $opt = 0;
      if(defined $ARGV[1]) {
        $opt = $ARGV[1];
      }
      my $ipa = 0;
      if(defined $ARGV[2]) {
        $ipa = $ARGV[2];
      }
      my $option = 0;
      system("cp $js_file $tempdir/$js_file");
      my $res = system("$pwd/../build/gnu/js2mpl $tempdir/$js_file $option > $tempdir/$log_file");
      if ($res > 0) {
        print " ==js2mpl===> $file\n";
        $countMPL ++;
        push(@failed_mpl_file, $file);
        $flag ++;
        next;
      }
      system("cp $tempdir/$mpl_file $tempdir/$origmpl_file");
      $res = system("$pwd/../../mapleall/build/gnu/mapleipa/mplipa $tempdir/$mpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==mplipa===> $file\n";
        $countIPA ++;
        push(@failed_mplipa_file, $file);
        $flag ++;
        next;
      }
      if ($ipa != 0) {
        system("cp $tempdir/$ipampl_file $tempdir/$mpl_file");
      }
      $res = system("$pwd/../../mapleall/build/gnu/mapleme/mplme $tempdir/$mpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==mplme===> $file\n";
        $countME ++;
        push(@failed_mplme_file, $file);
        $flag ++;
        next;
      }
      if ($opt & 1) {
        system("cp $tempdir/$optmpl_file $tempdir/$mpl_file");
      }
      $res = system("$pwd/../../mapleall/build/gnu/maplebe/be/mplbe $tempdir/$mpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==mplbe===> $file\n";
        $countMMPL ++;
        push(@failed_mmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/gnu/maplebe/be/mplbe-c $tempdir/$mpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==mplbe-c===> $file\n";
        $countgenCMPL ++;
        push(@failed_gencmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/gnu/mapleir/cmpl2mmpl $tempdir/$cmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==cmpl2mmpl===> $file\n";
        $countprintCMPL ++;
        push(@failed_printcmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("cd $tempdir; $pwd/timeout.sh -t 3 -i 1 -d 0 $pwd/../../mapleall/build/gnu/maplevm/js/jsvm-cmpl/jsvm-cmpl $cmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        print " ==jsvm-cmpl===> $file\n";
        $countrunCMPL ++;
        push(@failed_jsvm_cmpl_file, $file);
        $flag ++;
        next;
      }
      # plugins have to be run in the local directory at moment due to the lacking of search path for required
      #$res = system("$pwd/../../mapleall/build/gnu/maplevm/interpreter $tempdir/$mmpl_file > $tempdir/$out_file");
      $res = system("cd $tempdir; $pwd/../../mapleall/build/gnu/maplevm/js/interpreter $mmpl_file > $tempdir/$out_file");
      system("grep failed $tempdir/$out_file > $tempdir/$err_file");
      my $errfile = "$tempdir/$err_file";
      my $size = -s $errfile;
      #print "res=$res\n";
      #print "size=$size\n";
      if ($res == 0 && $size > 0) {
        print "\n!!warning: string \"failed\" is emited from interpreter. please check the test case $js_file.\n";
      }
      if ($res > 0) {
        print " ==interpreter===> $file\n";
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
print " $count\n";
closedir(DIR);

chdir $REPORT_DIR;

my $reportFile = 'report.txt';
open(my $fh, '>>', $reportFile) or die "Could not open file '$reportFile' $!";
print $fh "$ARGV[0] $ARGV[1] report: \n";

if ((scalar(@failed_mpl_file) + scalar(@failed_mmpl_file) + scalar(@failed_int_file) + scalar(@failed_mplipa_file) + 
     scalar(@failed_mplme_file) +
     scalar(@failed_gencmpl_file) + scalar(@failed_printcmpl_file) + scalar(@failed_jsvm_cmpl_file)) eq 0) {
  print("\n all $count tests passed\n");
  print("======================================================\n");
  print $fh "all $count tests passed\n";
  close $fh;
} else {
  my $countFailed = $countMPL + $countIPA + $countME + $countMMPL + $countINT + $countrunCMPL + $countgenCMPL;
  my $countPassed = $count - $countFailed;
  if(scalar(@successed_file) > 0) {
    print "\n=========================\npassed $countPassed tests:\n\n";
    print $fh "$countPassed testcases passed\n";
    foreach $successed (@successed_file) {
      print $successed."\n";
    }
    print "=========================\n";
    print "    passed $countPassed tests\n";
    print "=========================\n";
  }
  print "\n=========================\nfailed $countFailed tests:\n\n";
  if(scalar(@failed_mpl_file) > 0){
    print("=== failed generation of maple IR: $countMPL tests\n");
    print $fh "$countMPL testcases failed with generation of maple IR\n";
    foreach $failed (@failed_mpl_file) {
      print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_mplipa_file) > 0){
    print("=== failed maple IPA: $countIPA tests\n");
    print $fh "$countIPA testcases failed with maple IPA\n";
    foreach $failed (@failed_mplipa_file) {
      print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_mplme_file) > 0){
    print("=== failed maple ME: $countME tests\n");
    print $fh "$countME testcases failed with maple ME\n";
    foreach $failed (@failed_mplme_file) {
      print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_mmpl_file) > 0){
    print("=== failed generation of machine maple IR: $countMMPL tests\n");
    print $fh "$countMMPL testcases failed with generation of machine maple IR\n";
    foreach $failed (@failed_mmpl_file) {
      print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_int_file) > 0){
    print("=== failed interpretation: $countINT tests\n");
    print $fh "$countINT testcases failed with interpretation\n";
    foreach $failed (@failed_int_file) {
      print $failed."\n";
    }
  }
  if(scalar(@failed_gencmpl_file) > 0) {
    print("=== failed gen-compact maple IR: $countgenCMPL tests\n");
    print $fh "$countgenCMPL testcases failed with gen-compact maple IR\n";
    foreach $failed (@failed_gencmpl_file) {
      print $failed."\n";
    }
  }
  if(scalar(@failed_printcmpl_file) > 0) {
    print("=== failed print-compact maple IR: $countprintCMPL tests\n");
    print $fh "$countprintCMPL testcases failed with print-compact maple IR\n";
    foreach $failed (@failed_printcmpl_file) {
      print $failed."\n";
    }
  }
  if(scalar(@failed_jsvm_cmpl_file) > 0) {
    print("=== failed interprete-compact maple IR: $countrunCMPL tests\n");
    print $fh "$countrunCMPL testcases failed with interprete-compact maple IR\n";
    foreach $failed (@failed_jsvm_cmpl_file) {
      print $failed."\n";
    }
    print "\n";
  }
  print "=========================\n";
  close $fh;
}
