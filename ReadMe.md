Directory Structure
===================
```
  +-- AETG 2 (Solution File)
       |
       +-- Readme.md (this file)
       |
       +-- Bin (AETG_2[_x64][D].exe)
       |
       +-- AETG_Project_2 (AETG Source Code) (Project file)
       |
       +-- Data (output data files)
       |   
       +-- Documentation (project implementation documentation)
       |
       +-- Obj (compile time intermediaries objects)
 ```       
 AETG_Project_2 Overview
========================================================================

Assignment CSCE 5420 Software Engineering:

In this assignment, you will extend your homework 1 submission to provide 3-way
combinational coverage.

1. For this assignment, you are not required to take strings as input. You 
   may simply give every factor and every level a unique numerical identifier.
2. You should prompt the user for the number of factors and levels so that the 
   grader can easily grade your program.
3. You may implement the program in your choice of programming language.
4. Your program may end up using too much time or memory for larger inputs, so 
   you will need to be careful in your implementation. The papers do not describe 
   the exact implementation details for the exact data structures that they use. 
   It is expected that you can effectively make decisions that lead to a fast 
   and efficient implementation.
5. There is randomization in your algorithm. You will run your program 100 times 
   for each input and report the average size and execution time for each input. 
   You should also use 50 candidates as done in the AETG literature.
6. You will report the results of your algorithm for several inputs listed in 
   the table on the next page. Of course, it is possible that your results will 
   slightly vary from their reported results since there is randomization in the 
   algorithm. However, if your results are off by more than 20% for any inputs 
   larger than 3^4, you have a bug in your program.
7. You should strive to implement an efficient solution. To receive credit for 
   this assignment, your algorithm cannot run for longer than 20 minutes to find 
   a solution.
8. You are only required to implement a solution for up to 3-way coverage.

AETG algorithm overview:
-------------------------------------------------------------------------------

Assume that we have a system with k test parameters and that the i-th parameter 
has li different values. Assume that we have already selected r test cases. We 
select the r+1 by first generating M different candidate test cases and then 
choosing one that covers the most new pairs. Each candidate test case is 
selected by the following greedy algorithm.

1. Choose a parameter f and a value l for f such that the parameter value 
   appears in the greatest number of uncovered pairs.

2. Let f1 = f. Then choose a random order for the remaining parameters. 
   Then we have an order for all k parameters f1, ... fk.

3. Assume that values have been selected for parameters f1, ..., fj. For 
   1 <= i <= j, let the selected value for fi be called vi. Then choose a value 
   vj+1 for fj+1 as follows. For each possible value v for fj, find the number of 
   new pairs in the set of pairs { fj+1 = v and fi = vi for 1 <= i <= j }. Then 
   let vj+1 be one of the values that appeared in the greatest number of new pairs.

Note that in this step, each parameter value is considered only once for 
inclusion in a candidate test case. Also, that when choosing a value for 
parameter fj+1, the possible values are compared with only the j values already 
chosen for parameters f1, ..., fj.

 Implementation
==============================================================================
1.  The overall greatest challenge in extending the previous assignment of AETG 2-way coverage 
    to 3-way coverage was in the implementation of a systematic way to generate the set of 
    uncovered (UC) 3-way tuples.  Ultimately, this was handled through a combinatorial 
    algorithm.
2.  Since the tuple is being stored in an unordered set (or a hash-set), with the values of the
    tuple being used in the hashing algorithm, the nature of extending the tuple from 2 elements
    to 3 elements required some tweaking & fine tuning to the algorithm.  This was accomplished 
    by equal parts research and experimentation.
3.  Another fundamental change from the previous assignment was in moving the target platform
    from x32 to x64.

 Reference(s)
===============================================================================

 * "Automated Combinatorial Testing for Software", National Institute for Standards and Technology,
   http://csrc.nist.gov/groups/SNS/acts/coverage_measure.html

 * RC Bryce and CJ Colbourn, "A density-based greedy algorithm for higher strength covering arrays", 
   Softw. Test. Verif. Reliab. 2009

 * DM Cohen, SR Dalal, ML Fredman, and GC Patton, "The AETG System: An Approach 
   to Testing Based on Combinatorial Design", Appeared in IEEE Transactions On 
   Software Engineering, volume 23, number 7, July 1997, pages 437-444, 
   http://aetgweb.appcomsci.com/papers/1997-tse.html#heur

 * Manchester, Bryce, Sampath, Samant, Kuhn, Kacker, "Applying higher strength 
   combinatorial criteria to test case prioritization: a case study", July 27, 2012

 * Grindal, Offutt, Andler, "Combination Testing Strategies: A Survey",
   November 16th, 2004, http://csrc.nist.gov/acts/grindal-offutt-andler.pdf

 * George, "Constructing Covering Arrays", Universitat Politecnica De Valencia,
   https://riunet.upv.es/bitstream/handle/10251/17027/tesisUPV3917.pdf?sequence=1

 * Lavavej, "rand() Considered Harmful", Microsoft Corp,
   http://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful

 * Kuhn, Kacker, Yu, Lei, "Introduction to Combinatorial Testing", 2010

 * James Bach, Patrick Schroeder, "Pairwise Testing: A Best Practice That Isn't", 
   http://www.satisfice.com

 * Ellims, Ince, Petre, "AETG vs. Man: an Assessment of the Effectiveness of 
   Combinatorial Test Data Generation", 2008, Technical Report 2007/08, ISSN
   1744-1986

 * Bryce, Colbourn, "One-Test-at-a-Time Heuristic Search for Interaction Test Suites",
   GECCO'07, July 7-11, 2007 

 * Bryce, Colbourn, "Test Prioritization for Pairwise Interaction Coverage", A-MOST'05,
   May 15-21, 2005

 * Richard Kuhn, Raghu N. Kacker, Yu Lei. "Practical Combinatorical Testing", 
   National Institute for Standards and Technology, NIST Special Publication 800-142, 
   October 2010, http://csrc.nist.gov/groups/SNS/acts/documents/SP800-142-101006.pdf

 * Richard Kuhn, Raghu Kacker, Yu Lei, "Measuring and Specifying Combinatorial Coverage 
   of Test Input Configurations", National Institute for Standards and Technology, 
   Innovations in Systems and Software Engineering vol. 12, no. 4, December 2016, pp. 249-261, 
   https://csrc.nist.gov/publications/detail/journal-article/2016/measuring--specifying-combinatorial-coverage-test-input-configs


