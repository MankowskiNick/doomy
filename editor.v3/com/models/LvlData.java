package com.models;

import java.io.Serializable;
import java.util.ArrayList;

public class LvlData implements Serializable
{
    public ArrayList<Vertex> Vertices;
    public ArrayList<Wall> Walls;
    private static final long serialVersionUID = 1L;

    public LvlData() 
    { 
        this(new ArrayList<Vertex>(), new ArrayList<Wall>());
    }

    public LvlData(ArrayList<Vertex> vertices, ArrayList<Wall> walls)
    {
        this.Vertices = vertices;
        this.Walls = walls;
    }
}