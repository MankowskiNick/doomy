package com.handlers;

import java.io.*;

public class FileHandler<T>
{
    public FileHandler() { }
    
    public boolean WriteObject(String filename, T contents)
    {
        try
        {
            FileOutputStream fileOutStream = new FileOutputStream(filename);
            try (
                ObjectOutputStream objectOutputStream = new ObjectOutputStream(fileOutStream)
            ) 
            {
                objectOutputStream.writeObject(contents);
            }
            return true;
        }
        catch(Exception ex)
        {
            System.out.println("ERROR: Exception caught in the WriteObject method.\n" + ex.getMessage());
            return false;
        }
    }
    
    @SuppressWarnings("unchecked")
    public T ReadObject(String filename)
    {
        try
        {
            FileInputStream fileInputStream = new FileInputStream(filename);
            try (ObjectInputStream objectInputStream = new ObjectInputStream(fileInputStream)) {
                return (T)objectInputStream.readObject();
            }
        }
        catch(Exception ex)
        {
            System.out.println("ERROR: Exception caught in the ReadObject method.\n" + ex.getMessage());
            return null;
        }
    }
}