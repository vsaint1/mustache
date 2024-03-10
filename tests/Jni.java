/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */
package tests;


class Jni {
 
    static {
        System.loadLibrary("mustache.dll");
    }
    public native void getProcessId();

    public static void main(String[] args) {
        new Jni().getProcessId();
    }
 }