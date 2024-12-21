package com.models;

import java.util.ArrayList;

public class BSPTree 
{
    public static int MAX_INT = (int)Math.pow(2, 32);
    public static int SUBSECTOR = 0x8000;

    public MapDat Map;
    public BSPNode Head;
    public ArrayList<BSPNode> Subsectors = new ArrayList<BSPNode>();
    private int NextNodeId = 0;
    private int NextWallId = -1;

    private enum Side
    {
        UNDEFINED,
        COINCIDENT,
        INTERSECTING,
        FRONT,
        BACK
    }

    private class DivLine
    {
        public Pair<Float, Float> Position;
        public Pair<Float, Float> Direction;
    }

    public class BSPNode
    {
        public int Id;
        public Pair<BSPNode, BSPNode> Sides;
        public Wall PartitionWall;
        public ArrayList<Wall> Walls;
        public BSPNode() { }
    }

    public BSPTree() { }

    public BSPTree(MapDat map)
    {
        this.Map = map;
        this.Build();
    } 

    public void Build()
    {
        ArrayList<Wall> walls = new ArrayList<Wall>(this.Map.Walls);
        this.Head = BuildNode(walls);
    }

    private BSPNode BuildNode(ArrayList<Wall> walls)
    {
        // Get the splitting wall
        Pair<Wall, Integer> Splitter = PickSplitWall(walls);
        Wall splitWall = Splitter.a;
        int score = Splitter.b;

        BSPNode node = new BSPNode();

        // Base case - the set of walls passed forms a subsector
        if (score == MAX_INT)
        {
            node.Id = this.GetNodeId() + SUBSECTOR;
            node.Walls = walls;
            node.PartitionWall = walls.get(0);

            this.Subsectors.add(node);
            return node;
        }

        Pair<ArrayList<Wall>, ArrayList<Wall>> split = ExecuteSplit(splitWall, walls);

        node.Id = this.GetNodeId();
        node.PartitionWall = splitWall;
        node.Sides = new Pair<BSPNode, BSPNode>(
            this.BuildNode(split.a),
            this.BuildNode(split.b));

        return node;
    }

    private Pair<ArrayList<Wall>, ArrayList<Wall>> ExecuteSplit(Wall splitWall, ArrayList<Wall> walls)
    {
        DivLine divLine = GetDivLine(splitWall);

        ArrayList<Wall> frontWalls = new ArrayList<Wall>();
        ArrayList<Wall> backWalls = new ArrayList<Wall>();

        for (int i = 0; i < walls.size(); i++)
        {
            Wall w = walls.get(i);
                
            Side side = Side.UNDEFINED;
            if (w.Id == splitWall.Id)
                side = Side.COINCIDENT;
            else
                side = WallOnSide(divLine, w);

            switch(side)
            {
                case COINCIDENT:
                    frontWalls.add(w);
                    backWalls.add(w);
                    break;
                case FRONT:
                    frontWalls.add(w);
                    break;
                case BACK:
                    backWalls.add(w);
                    break;
                case INTERSECTING:

                    // Remove wall from list
                    walls.remove(w);
                    this.Map.Walls.remove(w);

                    // Get intersection
                    Pair<Float, Float> intersection = GetIntersection(divLine, w);

                    // Create vertex at intersection and add it to the map
                    Vertex newVert = new Vertex(this.Map.Vertices.size(), intersection.a, intersection.b);

                    // Add vertex to map
                    this.Map.Vertices.add(newVert);

                    // Create two new walls
                    Wall w1 = new Wall(w.Id,
                                        new Pair<Vertex, Vertex>(w.Line.a, newVert),
                                        w.Color,
                                        w.Type,
                                        w.MinHeight, w.MaxHeight,
                                        w.FloorHeight, w.CeilingHeight);
                    Wall w2 = new Wall(this.GetWallId(),
                                        new Pair<Vertex, Vertex>(newVert, w.Line.b),
                                        w.Color,
                                        w.Type,
                                        w.MinHeight, w.MaxHeight,
                                        w.FloorHeight, w.CeilingHeight);

                    // Add walls to map
                    this.Map.Walls.add(w1);
                    this.Map.Walls.add(w2);

                    // Add walls to front and back lists
                    if (WallOnSide(divLine, w1) == Side.FRONT)
                    {
                        frontWalls.add(w1);
                        backWalls.add(w2);
                    }
                    else
                    {
                        frontWalls.add(w2);
                        backWalls.add(w1);
                    }
                    break;

                default:
                    System.err.println("ERROR::UNKNOWN_SIDE - Unknown side for wall with ID " + w.Id);
                    break;
            }
        }
        return new Pair<ArrayList<Wall>, ArrayList<Wall>>(frontWalls, backWalls);
    }

    private int EvaluateSplit(Wall splitWall, ArrayList<Wall> walls, int bestScore)
    {
        DivLine divLine = GetDivLine(splitWall);

        int frontCount = 0;
        int backCount = 0;

        for (Wall w : walls)
        {
            Side side = Side.UNDEFINED;
            if (w.Id == splitWall.Id)
                continue;
            
            side = WallOnSide(divLine, w);

            // Update the counter
            if (side == Side.FRONT)
                frontCount++;
            else if (side == Side.BACK)
                backCount++;
            else if (side == Side.INTERSECTING)
            {
                frontCount++;
                backCount++;
            }

            // TODO: look into this, this is an optimization that may or may not work.
            // int maxCount = frontCount < backCount ? backCount : frontCount;
            
            // // Get count of walls that need to be split
            // int newCount = frontCount + backCount - walls.size();

            // ??? This comes from DoomBSP, why the 8?
            // score = maxCount + newCount * 8;

            // get out early, i think this may cause some issues in not getting the best
            // score for a given splitwall
            // if (score > bestScore) 
            //     return score;
        }
        if (frontCount == 0 || backCount == 0)
            return MAX_INT;


        int maxCount = frontCount < backCount ? backCount : frontCount;    
        int newCount = frontCount + backCount - walls.size();
        int score = maxCount + newCount * 8;

        return score;
    }
 
    private Side VertOnSide(DivLine divLine, Vertex v)
    {
        // Get vector normal to divLine
        Pair<Float, Float> normalVect = new Pair<Float, Float>(
            -1 * divLine.Direction.b, divLine.Direction.a);

        // Get vector from divLine.Position to v
        Pair<Float, Float> dv = new Pair<Float, Float>(
            v.x - divLine.Position.a, v.y - divLine.Position.b);

        // Get dot product dv * normalVect
        float dot = (dv.a * normalVect.a) + (dv.b * normalVect.b);

        // Return the correct side
        if (dot == 0)
            return Side.INTERSECTING;
        if (dot > 0)
            return Side.FRONT;
        return  Side.BACK;
    }

    private Side WallOnSide(DivLine divLine, Wall w)
    {
        Side v1Side = VertOnSide(divLine, w.Line.a);
        Side v2Side = VertOnSide(divLine, w.Line.b);

        // Coincident
        if (v1Side == Side.INTERSECTING && v2Side == Side.INTERSECTING)
            return Side.COINCIDENT;
        
        // Both on same side
        if (v1Side == v2Side)
            return v1Side;

        // Intersecting at an edge
        if (v1Side == Side.INTERSECTING)
            return v2Side;
        if (v2Side == Side.INTERSECTING)
            return v1Side;

        // Intersecting somewhere in between
        return Side.INTERSECTING;
    }

    private DivLine GetDivLine(Wall w)
    {
        DivLine line = new DivLine();
        line.Position = new Pair<Float, Float>(
            w.Line.a.x, w.Line.a.y);
        line.Direction = new Pair<Float, Float>(
            w.Line.b.x - w.Line.a.x, w.Line.b.y - w.Line.a.y);
        return line;
    }

    private Pair<Wall, Integer> PickSplitWall(ArrayList<Wall> walls)
    {
        int bestScore = MAX_INT;
        Wall bestWall = null;

        for (Wall w : walls)
        {
            int score = EvaluateSplit(w, walls, bestScore);
            if (score < bestScore)
            {
                bestScore = score;
                bestWall = w;
            }
        }
        return new Pair<Wall, Integer>(bestWall, bestScore);
    }

    private Pair<Float, Float> GetIntersection(DivLine divLine, Wall w)
    {

        // Solving Ax = B for x => x = A^-1 B
        Mat2 A = new Mat2(  -1 * divLine.Direction.a, w.Line.b.x - w.Line.a.x,
                            -1 * divLine.Direction.b, w.Line.b.y - w.Line.a.y);

        Vec2 B = new Vec2(divLine.Position.a - w.Line.a.x, divLine.Position.b - w.Line.a.y);

        // If det(A) == 0, lines are coincident, so return v1
        if (A.Det() == 0)
            return null; //new Pair<Float, Float>(w.Line.a.x, w.Line.a.y);

        Vec2 x = Mat2.Mult(A.Inv(), B);
        float t = x.x0;

        // Calculate intersection based on parameterized line
        return new Pair<Float, Float>(divLine.Position.a + (divLine.Direction.a * t),
            divLine.Position.b + (divLine.Direction.b * t));
    }
   
    private int GetWallId()
    {
        if (this.NextWallId == -1)
        {
            int maxId = -1;
            for (Wall w : this.Map.Walls)
            {
                if (w.Id > this.NextWallId)
                    this.NextWallId = w.Id ;
            }
            this.NextWallId = maxId + 1;
        }
        return this.NextWallId++;
    }

    private int GetNodeId()
    {
        return this.NextNodeId++;
    }
}