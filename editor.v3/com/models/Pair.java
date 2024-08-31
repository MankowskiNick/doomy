package com.models;

import java.io.Serializable;

public class Pair<T, V> implements Serializable
{
    private static final long serialVersionUID = 1L;

    public T a;
    public V b;

    public Pair(T a, V b)
    {
        this.a = a;
        this.b = b;
    }
}