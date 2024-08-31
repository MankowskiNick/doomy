package com.models;

import java.io.Serializable;
import java.util.ArrayList;

public class MapDat implements Serializable 
{
    private static final long serialVersionUID = 1L;

    public ArrayList<Vertex> Vertices;
    public ArrayList<Wall> Walls;
    

    public MapDat() 
    {
        this(new ArrayList<Vertex>(), new ArrayList<Wall>());
    }

    public MapDat(ArrayList<Vertex> vertices, ArrayList<Wall> walls) {
        this.Vertices = vertices;
        this.Walls = walls;
    }
}