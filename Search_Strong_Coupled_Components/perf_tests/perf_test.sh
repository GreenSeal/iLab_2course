g++ graph_gen.cpp
./a.out

mapfile -t tests < <( find . -name '*.dat' )

IFS=$'\n' tests=($(sort <<<"${tests[*]}")); unset IFS



for test in ${tests[@]}; do
	num=${test%.dat}
	num=${num#./0}
	num=${num#./}
	num=$((2**$(($((num))+2))))
	echo "$num"
#	START_TIME=$(date +%s)
	time -p ../a.out $test 1> "log.txt"
#	END_TIME=$(date +%s)
#	DIFF=$(( $END_TIME - $START_TIME ))
#	echo "It took $DIFF seconds"
done