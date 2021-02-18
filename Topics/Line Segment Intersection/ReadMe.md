# Line Segment Intersection

## Time Cost Experiments

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_1.png" alt="sweepline_anal1" width="1000" />
</div>

The above plot showed the comparison of Plane Sweep Algorithm vs Trivial Traversal Algorithm. We can easily find that the time cost by Plane Sweep Algorithm is especially sensitive to #intersections/#segments. If we only consider all the intersections without figuring out the belonging segments, the time consumed will not be sensitive to #intersections/#segments at all, as the first plot showed. However, to conduct a fair contest, we should better find out all the intersections as well as their belonging segments. Here we provide two traversal algorithms. The first one is trivial without optimization which is named TrivialTraversal, while the second one makes use of topological property of the segment intersection problem which we name it OptimizedTraversal. The results of the time cost experiments and accuracy tests are presented as followed.

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_2.png" alt="sweepline_anal2" width="1000"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_3.png" alt="sweepline_anal3" width="1000"/>
</div>

Only when #intersections/#segments is less than 100% or so could the Plane Sweep Algorithm be an optimal choice. That is to say, for certain problems where segments tend to be far away from each other, Plane Sweep Algrithm should be a better way to find out all the intersections. Otherwise one may use Optimized Traversal Algorithm instead, since it claims less time and space but achieves better accuracy.

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Sweepline.png" alt="sweepline" width="400" height="390"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Traversal.png" alt="traversal" width="400" height="390"/>
</div>

## Computational Precision

The mentioned 3 algorithms (TrivialTraversal, OptimizedTraversal, PlaneSweeper) are all theoretically correct that could find out all the segment intersections as well as their belonging relationships, as long as the problem is not ill-conditioned, i.e. there exist two segments share more than one intersections. However, in the computational view, all the algorithms suffer from computational precision limit which is influential and crucial to the algorithm feasibility.

Here in this problem, the error exist mainly at decision of point within segments as well as decision of terminals. In consideration of algorithm complexity, the results from TrivialTraversal may have a reasonable error estimation, so as OptimizedTraversal since it only utilize the topological properties. PlaneSweeper should be the hardest one to master for it makes full use of geometric properties and strongly depend on them. This explanation coincides with the above plot of accuracy test.

For now I have not got time to figured out a perfect solution for the error. Maybe I will consider this further in the future.

## Remarks

For algorithm proof and further explanations, please FEEL FREE to contact me at chenzhou@uchicago.edu. I would regularly check my mailbox and reply if it is worthy and necessary. Please DO NOT ask me details about the algorithms (i.e. algorithm steps) because you can easily find out the answer by yourself through my codes and referenced books.
