package com.models;

public class Vec2 {
    public float x0;
    public float x1;

    public Vec2(float x0, float x1) {
        this.x0 = x0;
        this.x1 = x1;
    }
    
    public Vec2 Add(Vec2 other) {
        return new Vec2(this.x0 + other.x0, this.x1 + other.x1);
    }

    public Vec2 Scale(float scalar) {
        return new Vec2(this.x0 * scalar, this.x1 * scalar);
    }
}
