./test-generic-task-descriptor
TEST1=$?
./test-periodic-task-descriptor
TEST2=$?
./test-prob-periodic-task-descriptor
TEST3=$?
./test-task-factory
TEST4=$?
./test-optimisation
TEST5=$?
./test-standard-distr
TEST6=$?
./test-distr-factory
TEST7=$?
./test-parser
TEST8=$?
./test-integrated-optimisation
TEST9=$?
./test-integrated-optimisation-beta
TEST10=$?

if [ $TEST1 -eq 1 ]
then
    echo "test-generic-descriptor ok"
else
    echo "test-generic-descriptor failed"
fi

if [ $TEST2 -eq 1 ]
then
    echo "test-periodic-descriptor ok"
else
    echo "test-periodic-descriptor failed"
fi

if [ $TEST3 -eq 1 ]
then
    echo "test-prob-periodic-descriptor ok"
else
    echo "test-prob-periodic-descriptor failed"
fi

if [ $TEST4 -eq 1 ]
then
    echo "test-task-factory ok"
else
    echo "test-task-factory failed"
fi

if [ $TEST5 -eq 1 ]
then
    echo "test-optimisation ok"
else
    echo "test-optimisation failed"
fi

if [ $TEST6 -eq 1 ]
then
    echo "test-standard-distr ok"
else
    echo "test-standard-distr failed"
fi

if [ $TEST7 -eq 1 ]
then
    echo "test-distr-factory ok"
else
    echo "test-distr-factory failed"
fi

if [ $TEST8 -eq 1 ]
then
    echo "test-parser ok"
else
    echo "test-parser failed"
fi

if [ $TEST9 -eq 1 ]
then
    echo "test-integrated-optimisation ok"
else
    echo "test-integrated-optimisation failed"
fi

if [ $TEST10 -eq 1 ]
then
    echo "test-integrated-optimisation-beta ok"
else
    echo "test-integrated-optimisation-beta failed"
fi
