#! /bin/bash

# Use valgrind/massif to collect memory usage information of an
# application running with jsvm-cmpl
#
# the application to be run should be the first argument:
#   $ ./profile-mem-usage.sh test.cmpl
#
# default source repository path

if [ $# -ne 1 ]
then
  echo "Using $0 [app_name.cmpl]"
  exit 1
fi

APP_CMPL=$1
if [ ! -f $APP_CMPL ]; then
  echo "$APP_CMPL doens't exist!"
  exit 1
fi

APP_PATH=$(dirname $APP_CMPL)
APP_NAME=$(basename $APP_CMPL)

# here assume the script is under js2mpl-vm/tests
JSVM_CMPL=../../mapleall/build/maplevm/compact/jsvm-cmpl

if [ ! -f $JSVM_CMPL ]; then 
  echo "$JSVM_CMPL doesn't exist!"
  exit 1
fi

MASSIF_OUT=$APP_PATH/massif.out.$APP_NAME
APP_OUT=$APP_PATH/app.out.$APP_NAME

result=$( valgrind --tool=massif --stacks=yes --massif-out-file=$MASSIF_OUT $JSVM_CMPL $APP_CMPL >& $APP_OUT )

# process APP_CMPL output
binmir_output=$( grep "Binmir Loader" $APP_OUT )
#echo "$binmir_output"

binmir_size=$( echo "$binmir_output" | grep "Bytes binmir loaded into memory" | awk '{ print $3}' )

memmanager_output=$( grep "Memory Manager" $APP_OUT )
#echo "$memmanager_output"

app_heap_size=$( echo "$memmanager_output" | grep "max app heap memory usage is" | awk '{ print $9 }' )
vm_mem_size=$( echo "$memmanager_output" | grep "vm internal memory usage is" | awk '{ print $8 }' )

max_app_stack=0

stack_sizes=$( grep "mem_stacks_B" $MASSIF_OUT | awk -F'[=]' '{ print $2 }' )

for size in $stack_sizes ; do
  ((size > max_app_stack)) && max_app_stack=$size
done
#echo "[Valgrind Massif] maximum stack memory used: $max_app_stack Bytes"

((total_app_size=app_heap_size+vm_mem_size+max_app_stack))
((total_app_size_xip=app_heap_size+vm_mem_size+max_app_stack-binmir_size))

# summary
echo "================================================================="
echo "  summary of memory usage information for MapleJS compact VM"
echo "================================================================="
echo "application name:                     $APP_CMPL"
echo "cmpl size:                            $binmir_size Bytes"
echo "maximum app heap RAM usage:           $app_heap_size Bytes"
echo "maximum vm RAM usage:                 $vm_mem_size Bytes"
echo "maximum app stack RAM usage:          $max_app_stack Bytes"
echo "maximum total app RAM usage:          $total_app_size Bytes"
echo "maximum total app RAM usage with XIP: $total_app_size_xip Bytes"
echo "================================================================="



exit 0
