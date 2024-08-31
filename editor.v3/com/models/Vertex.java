package com.models;

import java.io.Serializable;

public class Vertex implements Serializable
{
    private static final long serialVersionUID = 1L;
    
    public int id;
    public float x, y;

    public Vertex(int id, float x, float y)
    {
        this.id = id;
        this.x = x;
        this.y = y;
    }
}