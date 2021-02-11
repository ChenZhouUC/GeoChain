# Line Segemnt Intersection

## Time Cost Experiments

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_1.png" alt="sweepline_anal1" width="800" height="386"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_2.png" alt="sweepline_anal2" width="800" height="363"/>
</div>

The above plots showed that the comparison of Plane Sweep Algorithm vs Trivial Traversal Algorithm. We can easily find that the time cost by Plane Sweep Algorithm is especially sensitive to #intersections/#segments.

Only when #intersections/#segments is less than 1 or so could the Plane Sweep Algorithm be an optimal choice. That is to say, for certain problems where segments tend to be far away from each other, Plane Sweep Algrithm should be a better way to find out all the intersections. Otherwise one may use Trivial Traversal Algorithm instead, since it costs less time and claims less space.

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Sweepline.png" alt="sweepline" width="800" height="780"/>
</div>
