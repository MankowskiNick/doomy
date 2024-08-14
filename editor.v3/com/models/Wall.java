package com.models;

public class Wall
{
    int Id;

    Pair<Vertex, Vertex> Line;

    float MinHeight;
    float MaxHeight;

    float FloorHeight;
    float CeilingHeight;

    int[] Color;

    WallType Type;

    Wall(
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