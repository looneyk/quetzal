
Intersection intersection(Segment segmentA, Segment segmentB)
{
    Vector u = segmentA.endpoint(1) - segmentA.endpoint(0);
    Vector v = segmentB.endpoint(1) - segmentB.endpoint(0);
    Vector w = segmentA.endpoint(0) - segmentB.endpoint(0);
    float  D = perp(u,v);

    // test if they are parallel (includes either being a point)
    if (vector_eq0(cross(u, v))) // segmentA and segmentB are parallel
    {
        if (perp(u,w) != 0 || perp(v,w) != 0)  {
            return 0;                    // they are NOT collinear
        }
        // they are collinear or degenerate
        // check if they are degenerate  points
        float du = dot(u,u);
        float dv = dot(v,v);
        if (du==0 && dv==0) {            // both segments are points
            if (segmentA.endpoint(0) !=  segmentB.endpoint(0))         // they are distinct  points
                 return 0;
            *I0 = segmentA.endpoint(0);                 // they are the same point
            return 1;
        }
        if (du==0) {                     // segmentA is a single point
            if  (inSegment(segmentA.endpoint(0), segmentB) == 0)  // but is not in segmentB
                 return 0;
            *I0 = segmentA.endpoint(0);
            return 1;
        }
        if (dv==0) {                     // segmentB a single point
            if  (inSegment(segmentB.endpoint(0), segmentA) == 0)  // but is not in segmentA
                 return 0;
            *I0 = segmentB.endpoint(0);
            return 1;
        }
        // they are collinear segments - get  overlap (or not)
        float t0, t1;                    // endpoints of segmentA in eqn for segmentB
        Vector w2 = segmentA.endpoint(1) - segmentB.endpoint(0);
        if (v.x != 0) {
                 t0 = w.x / v.x;
                 t1 = w2.x / v.x;
        }
        else {
                 t0 = w.y / v.y;
                 t1 = w2.y / v.y;
        }
        if (t0 > t1) {                   // must have t0 smaller than t1
                 float t=t0; t0=t1; t1=t;    // swap if not
        }
        if (t0 > 1 || t1 < 0) {
            return 0;      // NO overlap
        }
        t0 = t0<0? 0 : t0;               // clip to min 0
        t1 = t1>1? 1 : t1;               // clip to max 1
        if (t0 == t1) {                  // intersect is a point
            *I0 = segmentB.endpoint(0) +  t0 * v;
            return 1;
        }

        // they overlap in a valid subsegment
        *I0 = segmentB.endpoint(0) + t0 * v;
        *I1 = segmentB.endpoint(0) + t1 * v;
        return 2;
    }

    // the segments are skew and may intersect in a point
    // get the intersect parameter for segmentA
    float     sI = perp(v,w) / D;
    if (sI < 0 || sI > 1)                // no intersect with segmentA
        return 0;

    // get the intersect parameter for segmentB
    float     tI = perp(u,w) / D;
    if (tI < 0 || tI > 1)                // no intersect with segmentB
        return 0;

    *I0 = segmentA.endpoint(0) + sI * u;                // compute segmentA intersect point
    return 1;
}

// inSegment(): determine if a point is inside a segment
//    Input:  a point P, and a collinear segment S
//    Return: 1 = P is inside S
//            0 = P is  not inside S
bool inSegment(Point P, Segment S)
{
    if (S.endpoint(0).x != S.endpoint(1).x) {    // S is not  vertical
        if (S.endpoint(0).x <= P.x && P.x <= S.endpoint(1).x)
            return true;
        if (S.endpoint(0).x >= P.x && P.x >= S.endpoint(1).x)
            return true;
    }
    else {    // S is vertical, so test y  coordinate
        if (S.endpoint(0).y <= P.y && P.y <= S.endpoint(1).y)
            return true;
        if (S.endpoint(0).y >= P.y && P.y >= S.endpoint(1).y)
            return true;
    }
    return false;
}

/*
Also,
https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect

#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product  (2D)
 
int intersect2D_2Segments( Segment S1, Segment S2, Point* I0, Point* I1 )
{
    Vector    u = S1.P1 - S1.P0;
    Vector    v = S2.P1 - S2.P0;
    Vector    w = S1.P0 - S2.P0;
    float     D = perp(u,v);

    // test if  they are parallel (includes either being a point)
    if (fabs(D) < SMALL_NUM) {           // S1 and S2 are parallel
        if (perp(u,w) != 0 || perp(v,w) != 0)  {
            return 0;                    // they are NOT collinear
        }
        // they are collinear or degenerate
        // check if they are degenerate  points
        float du = dot(u,u);
        float dv = dot(v,v);
        if (du==0 && dv==0) {            // both segments are points
            if (S1.P0 !=  S2.P0)         // they are distinct  points
                 return 0;
            *I0 = S1.P0;                 // they are the same point
            return 1;
        }
        if (du==0) {                     // S1 is a single point
            if  (inSegment(S1.P0, S2) == 0)  // but is not in S2
                 return 0;
            *I0 = S1.P0;
            return 1;
        }
        if (dv==0) {                     // S2 a single point
            if  (inSegment(S2.P0, S1) == 0)  // but is not in S1
                 return 0;
            *I0 = S2.P0;
            return 1;
        }
        // they are collinear segments - get  overlap (or not)
        float t0, t1;                    // endpoints of S1 in eqn for S2
        Vector w2 = S1.P1 - S2.P0;
        if (v.x != 0) {
                 t0 = w.x / v.x;
                 t1 = w2.x / v.x;
        }
        else {
                 t0 = w.y / v.y;
                 t1 = w2.y / v.y;
        }
        if (t0 > t1) {                   // must have t0 smaller than t1
                 float t=t0; t0=t1; t1=t;    // swap if not
        }
        if (t0 > 1 || t1 < 0) {
            return 0;      // NO overlap
        }
        t0 = t0<0? 0 : t0;               // clip to min 0
        t1 = t1>1? 1 : t1;               // clip to max 1
        if (t0 == t1) {                  // intersect is a point
            *I0 = S2.P0 +  t0 * v;
            return 1;
        }

        // they overlap in a valid subsegment
        *I0 = S2.P0 + t0 * v;
        *I1 = S2.P0 + t1 * v;
        return 2;
    }

    // the segments are skew and may intersect in a point
    // get the intersect parameter for S1
    float     sI = perp(v,w) / D;
    if (sI < 0 || sI > 1)                // no intersect with S1
        return 0;

    // get the intersect parameter for S2
    float     tI = perp(u,w) / D;
    if (tI < 0 || tI > 1)                // no intersect with S2
        return 0;

    *I0 = S1.P0 + sI * u;                // compute S1 intersect point
    return 1;
}

// inSegment(): determine if a point is inside a segment
//    Input:  a point P, and a collinear segment S
//    Return: 1 = P is inside S
//            0 = P is  not inside S
int
inSegment( Point P, Segment S)
{
    if (S.P0.x != S.P1.x) {    // S is not  vertical
        if (S.P0.x <= P.x && P.x <= S.P1.x)
            return 1;
        if (S.P0.x >= P.x && P.x >= S.P1.x)
            return 1;
    }
    else {    // S is vertical, so test y  coordinate
        if (S.P0.y <= P.y && P.y <= S.P1.y)
            return 1;
        if (S.P0.y >= P.y && P.y >= S.P1.y)
            return 1;
    }
    return 0;
}
*/