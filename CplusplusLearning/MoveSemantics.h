/*
 * 移动语义的最大作用是防止数据的复制。当我们想把一个函数里创造出的数据转移到外部的时候，
 * 传统的做法就是用拷贝构造函数去initilize an object by the return value，而这种
 * “把冰箱里的大象消灭，再在另外一个冰箱里重头创造一头大象”的做法是非常低效的，而Move
 * 语义在这里就是符合逻辑的“把冰箱里的大象移动到另外一个冰箱里面去”。
 * 
 * "It’s the mechanism that enables simple and cheap movement of objects between scopes."
 * 将数据的所有权（handle）在不同的scopes间移交（浅拷贝？），而非创建（深拷贝？）
 * 
 * "The (now) standard-library function forward tells the compiler to treat its argument as an rvalue 
 * reference, so that a T’s move constructor is used to steal that argument (rather than T’s copy
 * constructor). It is basically a cast (explicit type conversion) to an rvalue reference."
 * 
 * "has identity" 和 "can be moved from" 是思考lvalue和rvalue的重要出发点
 * 
 * "Move semantics is a cornerstone of C++'s model of resource management."
 */

#pragma once
