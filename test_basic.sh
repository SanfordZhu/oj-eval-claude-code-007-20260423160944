#!/bin/bash

# Simple test script for BASIC interpreter

echo "Testing QUIT command..."
echo "QUIT" | ./code
echo "Exit code: $?"
echo

echo "Testing LIST command..."
echo -e "LIST\nQUIT" | ./code
echo

echo "Testing REM and LIST..."
echo -e "10 REM test line\nLIST\nQUIT" | ./code
echo

echo "Testing LET and PRINT..."
echo -e "10 LET x = 42\n20 PRINT x\nRUN\nQUIT" | ./code
echo

echo "Testing arithmetic..."
echo -e "10 LET a = 10\n20 LET b = 3\n30 PRINT a + b\n40 PRINT a - b\n50 PRINT a * b\n60 PRINT a / b\n70 PRINT a % b\nRUN\nQUIT" | ./code
echo

echo "Testing IF-THEN..."
echo -e "10 LET x = 5\n20 IF x > 3 THEN 40\n30 PRINT 999\n40 PRINT 100\nRUN\nQUIT" | ./code
echo

echo "Testing INPUT..."
echo -e "10 INPUT x\n20 PRINT x\nRUN\n12345\nQUIT" | ./code
echo

echo "Testing GOTO..."
echo -e "10 PRINT 1\n20 GOTO 40\n30 PRINT 2\n40 PRINT 3\nRUN\nQUIT" | ./code
echo

echo "Testing CLEAR..."
echo -e "10 PRINT 1\nLIST\nCLEAR\nLIST\nQUIT" | ./code
echo

echo "Testing complex program..."
echo -e "10 REM Factorial program\n20 LET n = 5\n30 LET result = 1\n40 IF n <= 1 THEN 80\n50 LET result = result * n\n60 LET n = n - 1\n70 GOTO 40\n80 PRINT result\nRUN\nQUIT" | ./code