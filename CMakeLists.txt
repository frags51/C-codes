cmake_minimum_required(VERSION 3.6)
project(C)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        a2oj/2.c
        a2oj/3.c
        a2oj/4.c
        a2oj/5.c
        a2oj/6.c
        a2oj/7.c
        a2oj/8.c
        a2oj/a.c
        a2oj/test.c
        adv/order/order.c
        adv/concat.c
        adv/file.c
        adv/struct1.c
        adv/test.c
        assignment/A1/a.c
        assignment/A1/b.c
        assignment/A1/c.c
        assignment/A1/d.c
        assignment/A2/cs16btech11038a.c
        assignment/A2/cs16btech11038b.c
        assignment/A2/cs16btech11038c.c
        assignment/A3/blah.c
        assignment/A3/cs16btech11038a.c
        assignment/A3/cs16btech11038b.c
        assignment/A4/abc.c
        assignment/A4/cs16btech11038a.c
        assignment/A4/cs16btech11038b.c
        assignment/A4/expr_test.c
        assignment/A4/testa.c
        hrank/1.c
        hrank/2.c
        other/bezout.c
        other/determ.c
        other/fact.c
        other/i.c
        other/max.c
        other/numrev.c
        strings/ardel.c
        strings/arr.c
        strings/dup.c
        strings/spiral.c
        strings/strindex.c
        strings/strindex_2.c
        strings/strlen.c
        strings/strrev.c
        tic-tac-toe/tic.c
        abc.c
        abc.cpp
        new_test.c
        prog.c
        test.c
        test2.c
        test3.c)

add_executable(C ${SOURCE_FILES})