# <center>GeoChain</center> <font size=3><center>A Delicate CPP Package for Computational Geometry</center></font>

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/Concept.png" alt="concept" width="800" height="390" align="center"/>
</div>

## Euclidean Space
### Basic Elements
#### Degree of Freedom
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/Elements.png" alt="elements" width="600" height="190" align="center"/>
</div>
Here the degree of freedom (DOF) for each kind of geometric elements stands for the total meaningful cardinality in the designated Euclidean Space. Please pay attention to the below inequality.
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/DOF.gif" alt="DOF" align="center"/>
</div>
Here we distinguished the parameter DOF(ParDOF) from DOF. ParDOF represents the necessary (optimized) number of parameter we need to describe the certain element, which could be slightly larger than the real DOF.

Now we present some principles which might help calculating DOF.
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/oplus.gif" alt="oplus" align="center"/>
</div>
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/plus.gif" alt="plus" align="center"/>
</div>
Moreover, (x) and [x] could both be viewed as 0. In this scope, (x) means there might be several but finite candidates for each of x parameters, while [x] stands for only one candidate value (most probably 0) for each x parameters.