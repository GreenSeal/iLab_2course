
g++ SSCC_alg.cpp

mapfile -t tests < <( find ./tests -name *.dat )

IFS=$'\n' tests=($(sort <<<"${tests[*]}")); unset IFS

for test in ${tests[@]}; do
	echo "Executing test $test now..."
	echo "Execution result:" 
	echo "$(./a.out $test)"
	echo "Right answer:"
	echo "$(cat ${test/.dat/.ans})"
	echo ""
done
