# Line Segment Intersection

## Time Cost Experiments

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_1.png" alt="sweepline_anal1" width="1000" />
</div>

The above plot showed the comparison of Plane Sweep Algorithm vs Trivial Traversal Algorithm. We can easily find that the time cost by Plane Sweep Algorithm is especially sensitive to #intersections/#segments. If we only consider all the intersections without figuring out the belonging segments, the time consumed will not be sensitive to #intersections/#segments at all, as the first plot showed. However, to conduct a fair contest, we should better find out all the intersections as well as their belonging segments. Here we provide two traversal algorithms. The first one is trivial without optimization which is named TrivialTraversal, while the second one makes use of topological property of the segment intersection problem which we name it TrivialTraversal-Optimized. The results of the time cost experiments and accuracy tests are presented as followed.

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_2.png" alt="sweepline_anal2" width="1000"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_3.png" alt="sweepline_anal3" width="1000"/>
</div>

Only when #intersections/#segments is less than 200% or so could the Plane Sweep Algorithm be an optimal choice. That is to say, for certain problems where segments tend to be far away from each other, Plane Sweep Algrithm should be a better way to find out all the intersections. Otherwise one may use Trivial Traversal Algorithm (Optimized) instead, since it claims less time and space but achieves better accuracy.

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Sweepline.png" alt="sweepline" width="400" height="390"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Traversal.png" alt="traversal" width="400" height="390"/>
</div>
