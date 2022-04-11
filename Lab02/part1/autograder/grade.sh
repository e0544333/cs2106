#!/bin/bash

# Check if we have enough arguments
if [ $# -ne 1 ]; then
echo "Usage: ./grade.sh <filename>"
    exit 1
fi
# Delete temporary files
rm ref/*.out

# Compile the reference program
gcc ref/utils.c ref/$1.c -o ref/$1.out

# Generate reference output files
for i in ref/*.in; do
    ref/$1.out < $i > $i.out
done

# Now mark submissions

#
# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#

# Iterate over every submission directory
    # Compile C code
    # Print compile error message to output file
    # Generate output from C code using *.in files in ref
    # Compare with reference output files  and award 1 mark if they are identical
# print score for student
# print total files marked.

# Iterate through /sub content
echo -n > results.out
echo -e Test date and time: $(date +%A), $(date +%d\ %B\ %Y), $(date +%T) "\n" >> results.out 
score=0
# Find the total number of .in files in ref/ subdirectory
len=$(find ref/ -type f -name "*.in" | wc -l)
total_student_files=0

for i in subs/*; do
    # Compile C code
    gcc $i/utils.c $i/$1.c -o $i/$1.out
    if [ $? -ne 0 ]; then
        # Student gets 0 score
        # Print compile error message to output file
        echo Directory $(basename $i) has a compile error. >> results.out
    else
        # Generate output from C code using *.in files in ref
        file_counter=1
        for j in ref/*.in; do
            $i/$1.out < $j > $i/s$file_counter.out
            let file_counter=file_counter+1
        done

        # Compare with reference output files  and award 1 mark if they are identical
        file_counter=1
        
        while [[ $file_counter -le $len ]]; do
            # ref/s$counter.in.out $i/s$counter.out
            diff --brief <(sort ref/s$file_counter.in.out) <(sort $i/s$file_counter.out)
            comp_value=$?

            if [ $comp_value -eq 0 ]; then
                # do something because they are identical
                let score=score+1
            fi
            let file_counter=file_counter+1
        done
    fi
    # print score for student
    echo Directory $(basename $i) score $score/$len >> results.out
    
    # Reset score to 0
    score=0
    # Increment total student submissions by 1
    let total_student_files=total_student_files+1 
done
# print total files marked.
echo -e "\n"Processed $total_student_files files. >> results.out
