#!/bin/bash

#set -x

BASE_DIR=$1
SOURCE_DIR=$2

function launch ()
{
  size="$1"
  timesteps="$2"
  dx="$3"
  length="$4"

  lambda="0.02"
  #length=$(echo $timesteps | awk '{print $1 / 5}')
  #length="100"

  ../fdtd3d --time-steps $timesteps --sizex $size --same-size --3d --angle-phi 0 --dx $dx --wavelength $lambda \
    --log-level 0 --save-res --save-tfsf-e-incident --save-as-txt --use-tfsf --tfsf-sizex 2 --same-size-tfsf \
    --courant-factor 0.5 --save-tfsf-h-incident

  retval=$((0))
  error=$((0))

  #norm_exact="0.0"
  #norm_numerical="0.0"

  for line_num in `seq 1 1 $length`; do
    # exact value
    i=$(echo $line_num | awk '{print $1 - 1}')
    n=$(echo $timesteps | awk '{print $1 - 1}')

    exact=$(../exact $lambda $dx $i $n 0.5)

    exact_val_re=$(echo $exact | awk '{printf "%.17g", $1}')
    #exact_val_im=$(echo $exact | awk '{printf "%.17g", $2}')
    exact_val_mod=$(echo $exact | awk '{printf "%.17g", $3}')

    # numerical value
    line=$(sed "${line_num}q;d" current\[$timesteps\]_rank-0_EInc.txt)
    index=$(echo $line | awk '{printf "%.17g", $1}')
    numerical_val_re=$(echo $line | awk '{printf "%.17g", $2}')
    numerical_val_im=$(echo $line | awk '{printf "%.17g", $3}')
    numerical_val_mod=$(echo $numerical_val_re $numerical_val_im | awk '{printf "%.17g", sqrt($1 * $1 + $2 * $2)}')

    #echo "$exact_val_mod | $numerical_val_mod"

    if [ $i -ne $index ]; then
      echo "Incorrect output from fdtd3d"
      retval=$((1))
      break
    fi

    #norm_exact=$(echo $norm_exact $exact_val_mod | awk '{printf "%.17g", $1 + $2 * $2}')
    #norm_numerical=$(echo $norm_numerical $numerical_val_mod | awk '{printf "%.17g", $1 + $2 * $2}')
    #error=$(echo $error $exact_val_mod $numerical_val_mod | awk '{printf "%.17g", $1 + ($2-$3)*($2-$3)}')
    error=$(echo $error $exact_val_re $numerical_val_re | awk '{printf "%.17g", $1 + ($2-$3)*($2-$3)}')
  done

  cp current\[$timesteps\]_rank-0_EInc.txt E$dx.txt
  cp current\[$timesteps\]_rank-0_HInc.txt H$dx.txt

  #norm_exact=$(echo $norm_exact | awk '{printf "%.17g", sqrt($1)}')
  #norm_numerical=$(echo $norm_numerical | awk '{printf "%.17g", sqrt($1)}')

  #echo "!! $norm_exact | $norm_numerical"

  error=$(echo $error | awk '{printf "%.17g", $1}')
  #error=$(echo $norm_exact $norm_numerical | awk 'function abs(v) {return v < 0 ? -v : v} {printf "%.17g", abs($1 - $2)}')

  rm current\[*

  return $retval
}

CUR_DIR=`pwd`
TEST_DIR=$(dirname $(readlink -f $0))
cd $TEST_DIR

size="10"
retval=$((0))

launch $size 501 0.0004 100
error1=$(echo $error)
if [ $? -ne 0 ]; then
  retval=$((1))
fi

launch $size 1001 0.0002 200
error2=$(echo $error)
if [ $? -ne 0 ]; then
  retval=$((1))
fi

echo "$error1 | $error2"

cd $CUR_DIR

exit $retval
