#pragma once

namespace FST
{
    struct RELATION
    {
        char symbol;
        short mode;
        RELATION(
            char c = 0x00, 
            short ns = -1
        );
    };

    struct NODE
    {
        short n_relation;
        RELATION* relations;
        NODE();
        NODE(
            short n, 
            RELATION rel, ...
        );
    };

    struct FST
    {
        const char* string; 
        short position;
        short nstates;
        NODE* nodes;
        short* rstates;
        FST(
            const char* s, 
            short ns, 
            NODE n, ...
        ); 
    };

    bool execute(
        FST& fst
    );
};