# Question 2 Matrix Vector Operation 

 I've completed the sequential algorithm. Noticed that there was a lot of LCC Missess. It was noticeable because of the allocation of the matrix. So i tried to split the matrix to smaller parts, that reduced the LCC Missess but didn't improve the execution time. 

UPDATE: I found out that the we have some reduction when it comes to execution time when we increase the number of splits, and the best one so far is when we split each row and allocate it alone. I couldn't test more than that because I will need to change the algo to make that possible .  


UPDATE 28/12/2023: Next idea is to draw a graph based on the cache miss rate and time execution
and i need to understand why in some col and row number the cache miss rate is the lowest (done)

- [ ] Need to plot the boxplot for missess instead of missrates ? 

- [ ] plot the cpu time based on the S (N+M) to find the best way to use N and M to get the best performance for S.




## Analysis

from comparing the atomic implementation with the reverse atomic implementation we can see that the atomic reversed performes better on average with cols lower than M<=2^4. But for all the other cases the best is to have lower N than lower M in the atomic implementation, better than any other case. 

core 1 for atomic has better performance in lower or equal 3 for rows for 4 -> 2 cores, from 5 to 8 3 cores are better. than after 8 the 4cores performs better. 

there is also difference between the average of cpu time for rows compared to cols in the low numbers. the difference gets smaller when used for less cores. Which is logical, as the time of context switching increases the time of the execution.  

* just got throught the atom vs atom reversed comparaison and atomic cores , atomic cpu pgns.




