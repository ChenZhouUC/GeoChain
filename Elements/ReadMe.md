## Euclidean Space
### Basic Elements
#### Degree of Freedom
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/Elements.png" alt="elements" width="600" height="190" align="center"/>
</div>
In the scope of GeoChain, the degree of freedom (DOF) of each geometric element stands for the minimum meaningful dimension which is necessary for description and parameterization in the designated Euclidean Space. In order to distinguish DOF from parameter DOF (ParDOF), please pay strong attention to the below inequality:
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/DOF.gif" alt="DOF" align="center"/>
</div>
We define ParDOF as the optimized number of parameters we need to describe a certain kind of element, which might be slightly larger than its real DOF.

Since DOF is more like an objectve characteristic of a certain element, we have some principles which could help calculating DOF:
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/oplus.gif" alt="oplus" align="center"/>
</div>
<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/plus.gif" alt="plus" align="center"/>
</div>
Moreover, (x) and [x] could both be viewed as 0 in calculation. Here (x) means there might be several but finite candidates for each of x parameters, while [x] stands for only one candidate value (most probably 0) for each of x parameters.

#### Class Inheritance

According to the design of GeoChain, we devide geometric elements by the degrees of freedom occupied by one of their instances in the Euclidean Space, respectively:

```
Euclidean Space
├── Dim = 0 ── Point()
│
├── Dim = 1 ── Line()
│                └── HalfLine()
│                        └── Segment()
│==(Not Implemented Yet)==
├── Dim = 2 ── Plane()
│                └── HalfPlane()
│                        └── Fragment()
```

The above structure shows the inheritance of the geometric classes.

#### Visualization

<div align=center>
<img src="https://github.com/ChenZhouUC/GeoChain/blob/master/assets/ele1D2V.png" alt="ele1D2V" align="center"/>
</div>