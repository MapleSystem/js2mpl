#!/usr/bin/perl -w
my $tempdir = "./todo-tests/temp";
if(!(-e $tempdir)) {
  mkdir $tempdir;
}

my @failed_mpl_file;
my @failed_mmpl_file;
my @failed_int_file;
my @successed_file;
$dirname = "./todo-tests";
chdir $dirname;
#$dirname = "./";
#opendir ( DIR, $dirname ) || die "Error in opening dir $dirname\n";
#print("\n============= regression tests ================\n");
#while( ($filename = readdir(DIR))){
#  if(-d $filename and $filename ne "." and $filename ne "..") {
#    use Cwd;
#    my $predir = getcwd;
#    chdir $filename;
    my @files = <*.js>;

    foreach $fullname (@files) {
      (my $file = $fullname) =~ s/\.[^.]+$//;
      print ".";
      my $flag = 0;
      my $js_file = $file.'.js';
      my $mpl_file = $file.'.mpl';
      my $mmpl_file = $file.".mmpl";
      my $log_file = $file.'.log';
      #my $option = $ARGV[0];
      my $option = 0;
      system("cp $js_file ./temp/$js_file");
      my $res = system("../../build/js2mpl ./temp/$js_file $option > ./temp/$log_file");
      if ($res > 0) {
        push(@failed_mpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("../../../mapleall/maplebe/build/be/mplbe ./temp/$mpl_file >> ./temp/$log_file");
      if ($res > 0) {
        push(@failed_mmpl_file, $file);
        $flag ++;
        next;
      }
      $res = system("../../../mapleall/maplevm/build/interpreter32 ./temp/$mmpl_file >> ./temp/$log_file");
      if ($res > 0) {
        push(@failed_int_file, $file);
        $flag ++;
        next;
      }
      if ($flag eq 0) {
        push(@successed_file, $file);
        system("rm -f ./temp/$js_file");
        system("rm -f ./temp/$mpl_file");
        system("rm -f ./temp/$mmpl_file");
        system("rm -f ./temp/$log_file");
        next;
      }
    }
#    chdir $predir;
#  }
#}
#closedir(DIR);

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
