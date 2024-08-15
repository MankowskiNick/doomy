package com.models;

public class Wall
{
    public int Id;

    public Pair<Vertex, Vertex> Line;

    public float MinHeight;
    public float MaxHeight;

    public float FloorHeight;
    public float CeilingHeight;

    public int[] Color;

    public WallType Type;

    public Wall(
        int id, 
        Pair<Vertex, Vertex> line, 
        int[] color, 
        WallType type,
        float minHeight,
        float maxHeight,
        float floorHeight,
        float ceilingHeight
    )
    {
        this.Id = id;
        this.Line = line;
        this.Color = color;
        this.Type = type;
        this.MinHeight = minHeight;
        this.MaxHeight = maxHeight;
        this.FloorHeight = floorHeight;
        this.CeilingHeight = ceilingHeight;
    }

    public enum WallType 
    {
        PORTAL,
        WALL
    }
}