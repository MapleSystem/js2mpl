#!/usr/bin/perl -w
use Cwd;
my $pwd = getcwd;

if(!(defined $ARGV[0])) {
  print "------------------------------------------------\n";
  print "usage: testing.pl test_dir [pattern]\n";
  print "to test all *closure*.js in regression-tests:\n";
  print "       testing.pl regression-tests closure\n";
  print "------------------------------------------------\n";
  exit;
}

$dirname = "./$ARGV[0]";
my $tempdir = "$pwd/temp";
if(!(-e $tempdir)) {
  mkdir $tempdir;
}

my @failed_mpl_file;
my @failed_mmpl_file;
my @failed_int_file;
my @successed_file;
chdir $dirname;
$dirname = "./";
opendir (DIR, $dirname ) || die "Error in opening dir $dirname\n";
print("\n==================== run tests ================\n");
while( ($filename = readdir(DIR))){
  if(-d $filename and $filename ne ".." and $filename ne "temp") {
    my $predir = getcwd;
    chdir $filename;
    my @files = <*.js>;
    if(defined $ARGV[1]) {
      @files = <*$ARGV[1]*.js>;
    }

    foreach $fullname (@files) {
      (my $file = $fullname) =~ s/\.[^.]+$//;
      print ".";
      if(defined $ARGV[1]) {
        print "$file\n";
      }
      my $flag = 0;
      my $js_file = $file.'.js';
      my $mpl_file = $file.'.mpl';
      my $mmpl_file = $file.".mmpl";
      my $log_file = $file.'.log';
      #my $option = $ARGV[0];
      my $option = 0;
      system("cp $js_file $tempdir/$js_file");
      my $res = system("$pwd/../build/js2mpl $tempdir/$js_file $option > $tempdir/$log_file");
      if ($res > 0) {
        push(@failed_mpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplebe/be/mplbe $tempdir/$mpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        push(@failed_mmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("$pwd/../../mapleall/build/maplevm/interpreter32 $tempdir/$mmpl_file >> $tempdir/$log_file");
      if ($res > 0) {
        push(@failed_int_file, $file);
        $flag ++;
        next;
      }
      if ($flag eq 0) {
        push(@successed_file, $file);
        system("rm -f $tempdir/$js_file");
        system("rm -f $tempdir/$mpl_file");
        system("rm -f $tempdir/$mmpl_file");
        system("rm -f $tempdir/$log_file");
        next;
      }
    }
    chdir $predir;
  }
}
closedir(DIR);

if ((scalar(@failed_mpl_file) + scalar(@failed_mmpl_file) + scalar(@failed_int_file)) eq 0) {
  print("\n all ok\n");
  print("===============================================\n");
} else {
  if(scalar(@successed_file) > 0) {
    print "\n=========================\nsuccessed file:\n";
    foreach $successed (@successed_file) {
    print $successed."\n";
    }
    print "=========================\n";
  }
  print "\n=========================\nfailed file:\n";
  if(scalar(@failed_mpl_file) > 0){
    print("failed generate mapleir:\n");
    foreach $failed (@failed_mpl_file) {
    print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_mmpl_file) > 0){
    print("failed generate binary:\n");
    foreach $failed (@failed_mmpl_file) {
    print $failed."\n";
    }
    print "\n";
  }
  if(scalar(@failed_int_file) > 0){
    print("failed execute:\n");
    foreach $failed (@failed_int_file) {
    print $failed."\n";
    }
  }
  print "=========================\n";
}
