# Line Segment Intersection

## Time Cost Experiments

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_1.png" alt="sweepline_anal1" width="1000" height="567"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_2.png" alt="sweepline_anal2" width="1000" height="479"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/analysis_3.png" alt="sweepline_anal3" width="1000" height="511"/>
</div>

The above plots showed that the comparison of Plane Sweep Algorithm vs Trivial Traversal Algorithm. We can easily find that the time cost by Plane Sweep Algorithm is especially sensitive to #intersections/#segments. The upper one used traversal method to find all the intersections but not the segments they belonged to, while the lower two considered both intersections and the segments relationship.

Only when #intersections/#segments is less than 200% or so could the Plane Sweep Algorithm be an optimal choice. That is to say, for certain problems where segments tend to be far away from each other, Plane Sweep Algrithm should be a better way to find out all the intersections. Otherwise one may use Trivial Traversal Algorithm (Optimized) instead, since it costs less time and space but claims better accuracy, as showed in the bottom plot.

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Sweepline.png" alt="sweepline" width="400" height="390"/>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/Topics/Line%20Segment%20Intersection/Traversal.png" alt="traversal" width="400" height="390"/>
</div>
